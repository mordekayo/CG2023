#include "RenderComponent.h"

#include <iostream>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include "../Utils/SimpleMath.h"
#include "../DisplayWin32.h"
#include "../Game.h"
#include "../GameObjects/GameObject.h"

FRenderComponent::~FRenderComponent()
{
    
}

void FRenderComponent::Init()
{
	FObjectComponent::Init();

		Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	ID3DBlob* ErrorCode = nullptr;
	
	//"././GameFramework/Shaders/MyVeryFirstShader.hlsl"
	HRESULT Result = D3DCompileFromFile(L"../GameFramework/Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&VertexShaderByteCode,
		&ErrorCode);

	if (FAILED(Result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (ErrorCode)
		{
			const char* CompileErrors = (char*)(ErrorCode->GetBufferPointer());

			std::cout << CompileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(FGame::Instance()->GetDisplay().GetHWnd(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

    const D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 0.0f, 0.0f, 1.0f)", nullptr, nullptr };

	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderByteCode;
	
	Result = D3DCompileFromFile(L"../GameFramework/Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&PixelShaderByteCode,
		&ErrorCode);

	FGame::Instance()->GetDevice()->CreateVertexShader(
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		nullptr, VertexShader.GetAddressOf());

	FGame::Instance()->GetDevice()->CreatePixelShader(
		PixelShaderByteCode->GetBufferPointer(),
		PixelShaderByteCode->GetBufferSize(),
		nullptr, PixelShader.GetAddressOf());

    constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] =
	{
		D3D11_INPUT_ELEMENT_DESC
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	
	FGame::Instance()->GetDevice()->CreateInputLayout(
		InputElements,
		2,
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		InputLayout.GetAddressOf());
	
	Strides[0] = 32;
	Offsets[0] = 0;

	D3D11_BUFFER_DESC VertexBufDesc = {};
	VertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufDesc.CPUAccessFlags = 0;
	VertexBufDesc.MiscFlags = 0;
	VertexBufDesc.StructureByteStride = 0;
	VertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * static_cast<UINT>(std::size(LocalPoints));

	D3D11_SUBRESOURCE_DATA VertexData = {};
	VertexData.pSysMem = LocalPoints.data();
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	
	FGame::Instance()->GetDevice()->CreateBuffer(&VertexBufDesc, &VertexData, VertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC IndexBufDesc = {};
	IndexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufDesc.CPUAccessFlags = 0;
	IndexBufDesc.MiscFlags = 0;
	IndexBufDesc.StructureByteStride = 0;
	IndexBufDesc.ByteWidth = sizeof(int) * static_cast<UINT>(std::size(Indicies));

	D3D11_SUBRESOURCE_DATA IndexData = {};
	IndexData.pSysMem = Indicies.data();
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;
	
	FGame::Instance()->GetDevice()->CreateBuffer(&IndexBufDesc, &IndexData, IndexBuffer.GetAddressOf());
	
	D3D11_BUFFER_DESC ConstBufDesc = {};
	ConstBufDesc.Usage = D3D11_USAGE_DEFAULT;
	ConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstBufDesc.CPUAccessFlags = 0;
	ConstBufDesc.MiscFlags = 0;
	ConstBufDesc.StructureByteStride = 0;
	ConstBufDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	FGame::Instance()->GetDevice()->CreateBuffer(&ConstBufDesc, nullptr, &ConstantBuffer);
}

void FRenderComponent::Update()
{
	FObjectComponent::Update();
	
	const DirectX::XMFLOAT4 OwnerTransform = Owner->GetTransform();
	
	const DirectX::XMMATRIX ScaledMatrix = DirectX::XMMatrixScaling(
			static_cast<float>(FGame::Instance()->GetDisplay().GetScreenHeight())/
			static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth()),
			1.0f,
			1.0f);

	const DirectX::XMMATRIX TranslatedMatrix = DirectX::XMMatrixTranslation(OwnerTransform.x, OwnerTransform.y, OwnerTransform.z);

	const DirectX::XMMATRIX Transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(ScaledMatrix, TranslatedMatrix));

	FGame::Instance()->GetContext()->UpdateSubresource(ConstantBuffer, 0, nullptr, &Transform, 0, 0);
}

void FRenderComponent::Draw()
{
    FGame::Instance()->GetContext()->IASetInputLayout(InputLayout.Get());
    FGame::Instance()->GetContext()->IASetPrimitiveTopology(Topology);
    FGame::Instance()->GetContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    FGame::Instance()->GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), Strides, Offsets);
    FGame::Instance()->GetContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
    FGame::Instance()->GetContext()->PSSetShader(PixelShader.Get(), nullptr, 0);
	
    FGame::Instance()->GetContext()->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	
    FGame::Instance()->GetContext()->DrawIndexed(Indicies.size(), 0, 0);
}

void FRenderComponent::SetPoints(std::vector<DirectX::XMFLOAT4>&& NewPoints)
{
	LocalPoints = std::move(NewPoints);
}

void FRenderComponent::SetIndicies(std::vector<int>&& NewIndicies)
{
	Indicies = std::move(NewIndicies);
}

void FRenderComponent::SetTopology(D3D11_PRIMITIVE_TOPOLOGY NewTopology)
{
	Topology = NewTopology;
}
