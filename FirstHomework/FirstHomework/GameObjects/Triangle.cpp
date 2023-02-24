#include "Triangle.h"

#include "../Game.h"

#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "../ThirdParty/SimpleMath.h"
#include "../DisplayWin32.h"

Triangle::Triangle()
{
	
}

void Triangle::Init()
{
    GameObject::Init();

	Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	ID3DBlob* ErrorCode = nullptr;
	
	HRESULT Result = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
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
			MessageBox(Game::Instance()->GetDisplay().GetHWnd(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

    const D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderByteCode;
	
	Result = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&PixelShaderByteCode,
		&ErrorCode);
	
	Game::Instance()->GetDevice()->CreateVertexShader(
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		nullptr, VertexShader.GetAddressOf());

	Game::Instance()->GetDevice()->CreatePixelShader(
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
	
	Game::Instance()->GetDevice()->CreateInputLayout(
		InputElements,
		2,
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		InputLayout.GetAddressOf());

    constexpr DirectX::XMFLOAT4 Points[8] =
	{
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};


	D3D11_BUFFER_DESC VertexBufDesc = {};
	VertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufDesc.CPUAccessFlags = 0;
	VertexBufDesc.MiscFlags = 0;
	VertexBufDesc.StructureByteStride = 0;
	VertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * static_cast<UINT>(std::size(Points));

	D3D11_SUBRESOURCE_DATA VertexData = {};
	VertexData.pSysMem = Points;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	
	Game::Instance()->GetDevice()->CreateBuffer(&VertexBufDesc, &VertexData, VertexBuffer.GetAddressOf());

	Strides[0] = 32;
	Offsets[0] = 0;
	
    const int Indeces[] = { 0,1,2, 1,0,3 };
	D3D11_BUFFER_DESC IndexBufDesc = {};
	IndexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufDesc.CPUAccessFlags = 0;
	IndexBufDesc.MiscFlags = 0;
	IndexBufDesc.StructureByteStride = 0;
	IndexBufDesc.ByteWidth = sizeof(int) * static_cast<UINT>(std::size(Indeces));

	D3D11_SUBRESOURCE_DATA IndexData = {};
	IndexData.pSysMem = Indeces;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;
	
	Game::Instance()->GetDevice()->CreateBuffer(&IndexBufDesc, &IndexData, IndexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC ConstBufDesc = {};
	ConstBufDesc.Usage = D3D11_USAGE_DEFAULT;
	ConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstBufDesc.CPUAccessFlags = 0;
	ConstBufDesc.MiscFlags = 0;
	ConstBufDesc.StructureByteStride = 0;
	ConstBufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4);
	Game::Instance()->GetDevice()->CreateBuffer(&ConstBufDesc, nullptr, ConstantBuffer.GetAddressOf());
}

void Triangle::Update()
{
	GameObject::Update();

	DirectX::SimpleMath::Vector4 SrcData = {1.0f, 0.0f, 0.0f, 0.0f};
	Game::Instance()->GetContext()->UpdateSubresource(ConstantBuffer.Get(), 0, nullptr, &SrcData, 0, 0);
}

void Triangle::Draw()
{
	GameObject::Draw();

	Game::Instance()->GetContext()->IASetInputLayout(InputLayout.Get());
	Game::Instance()->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Game::Instance()->GetContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Game::Instance()->GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), Strides, Offsets);
	Game::Instance()->GetContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
	Game::Instance()->GetContext()->PSSetShader(PixelShader.Get(), nullptr, 0);
	
	Game::Instance()->GetContext()->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
	
	Game::Instance()->GetContext()->DrawIndexed(6, 0, 0);
}
