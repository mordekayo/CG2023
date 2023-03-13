#include "RenderComponent.h"

#include <iostream>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include "Utils/SimpleMath.h"
#include "DisplayWin32.h"
#include "Game.h"
#include "Camera/Camera.h"
#include "GameObjects/GameObject.h"

#include <WICTextureLoader.h>

FRenderComponent::FRenderComponent(std::string ShaderFileName)
{
	bUseTexture = false;
	this->ShaderFileName = ShaderFileName;
}

FRenderComponent::FRenderComponent(std::string ShaderFileName, std::string TextureFileName)
{
	bUseTexture = true;
	this->TextureFileName = TextureFileName;
	this->ShaderFileName = ShaderFileName;
}

FRenderComponent::~FRenderComponent()
{
    
}

void FRenderComponent::Init()
{
	FObjectComponent::Init();

	Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	ID3DBlob* ErrorCode = nullptr;
	
	std::wstring FileName(ShaderFileName.begin(), ShaderFileName.end());

	HRESULT Result = D3DCompileFromFile(FileName.c_str(),
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
			MessageBox(FGame::Instance()->GetDisplay().GetHWnd(), FileName.c_str(), FileName.c_str(), MB_OK);
		}

		return;
	}

    const D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 0.0f, 0.0f, 1.0f)", nullptr, nullptr };

	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderByteCode;
	
	Result = D3DCompileFromFile(FileName.c_str(),
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

	if(bUseTexture) 
	{
		D3D11_INPUT_ELEMENT_DESC InputElements[] = {
		 D3D11_INPUT_ELEMENT_DESC {
		  "POSITION",
		  0,
		  DXGI_FORMAT_R32G32B32A32_FLOAT,
		  0,
		  0,
		  D3D11_INPUT_PER_VERTEX_DATA,
		  0
		 },
		 D3D11_INPUT_ELEMENT_DESC {
		  "TEXCOORD", //
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
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC InputElements[] = {
		 D3D11_INPUT_ELEMENT_DESC {
		  "POSITION",
		  0,
		  DXGI_FORMAT_R32G32B32A32_FLOAT,
		  0,
		  0,
		  D3D11_INPUT_PER_VERTEX_DATA,
		  0
		 },
		 D3D11_INPUT_ELEMENT_DESC {
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
	}
	
	Strides[0] = 32;
	Offsets[0] = 0;

	D3D11_BUFFER_DESC VertexBufDesc = {};
	VertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufDesc.CPUAccessFlags = 0;
	VertexBufDesc.MiscFlags = 0;
	VertexBufDesc.StructureByteStride = 0;
	VertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * static_cast<UINT>(std::size(Points));

	D3D11_SUBRESOURCE_DATA VertexData = {};
	VertexData.pSysMem = Points.data();
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	
	FGame::Instance()->GetDevice()->CreateBuffer(&VertexBufDesc, &VertexData, VertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC IndexBufDesc = {};
	IndexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufDesc.CPUAccessFlags = 0;
	IndexBufDesc.MiscFlags = 0;
	IndexBufDesc.StructureByteStride = 0;
	IndexBufDesc.ByteWidth = sizeof(int) * static_cast<UINT>(std::size(Indices));

	D3D11_SUBRESOURCE_DATA IndexData = {};
	IndexData.pSysMem = Indices.data();
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

	if (bUseTexture)
	{
		std::wstring textureName(TextureFileName.begin(), TextureFileName.end());

		auto res = DirectX::CreateWICTextureFromFile(
			FGame::Instance()->GetDevice().Get(),
			FGame::Instance()->GetContext().Get(),
			textureName.c_str(),
			Texture.GetAddressOf(),
			TextureView.GetAddressOf()
		);

		D3D11_SAMPLER_DESC samplerStateDesc = {};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		FGame::Instance()->GetDevice()->CreateSamplerState(&samplerStateDesc, SamplerState.GetAddressOf());
	}

	CD3D11_RASTERIZER_DESC RastDesc = {};
	RastDesc.CullMode = D3D11_CULL_NONE;
	RastDesc.FillMode = D3D11_FILL_WIREFRAME;

	FGame::Instance()->GetDevice()->CreateRasterizerState(&RastDesc, RasterizerState.GetAddressOf());

	FGame::Instance()->GetContext()->RSSetState(RasterizerState.Get());
}

void FRenderComponent::Update()
{
	FObjectComponent::Update();
	
	const DirectX::XMMATRIX OwnerWorldViewProjectionMatrix = FGame::Instance()->GetCamera()->GetViewProjectionMatrix(Owner->GetWorldTransform());
	
	const DirectX::XMMATRIX ScaledMatrix = DirectX::XMMatrixScaling(
			static_cast<float>(FGame::Instance()->GetDisplay().GetScreenHeight())/
			static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth()),
			1.0f,
			1.0f);

	const DirectX::XMMATRIX Transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(ScaledMatrix, OwnerWorldViewProjectionMatrix));

	FGame::Instance()->GetContext()->UpdateSubresource(ConstantBuffer, 0, nullptr, &Transform, 0, 0);
}

void FRenderComponent::Draw()
{
	FGame::Instance()->GetContext()->ClearState();
	FGame::Instance()->GetContext()->RSSetState(RasterizerState.Get());
	FGame::Instance()->GetContext()->RSSetViewports(1, &Viewport);

	Viewport.Width = static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth());
	Viewport.Height = static_cast<float>(FGame::Instance()->GetDisplay().GetScreenHeight());
	Viewport.MinDepth = 0;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;

	if (bUseTexture)
	{
		FGame::Instance()->GetContext()->PSSetShaderResources(0, 1, TextureView.GetAddressOf());
		FGame::Instance()->GetContext()->PSSetSamplers(0, 1, SamplerState.GetAddressOf());
	}
    FGame::Instance()->GetContext()->IASetInputLayout(InputLayout.Get());
    FGame::Instance()->GetContext()->IASetPrimitiveTopology(Topology);
    FGame::Instance()->GetContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    FGame::Instance()->GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), Strides, Offsets);
    FGame::Instance()->GetContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
    FGame::Instance()->GetContext()->PSSetShader(PixelShader.Get(), nullptr, 0);
	
    FGame::Instance()->GetContext()->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	
    FGame::Instance()->GetContext()->DrawIndexed(Indices.size(), 0, 0);
}

void FRenderComponent::SetTopology(D3D11_PRIMITIVE_TOPOLOGY NewTopology)
{
	Topology = NewTopology;
}

void FRenderComponent::AddCube(float radius)
{
	Points = 
	{ 
		DirectX::SimpleMath::Vector4(radius,   radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4(radius, radius, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(-radius, -radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4(0.0f,   0.0f,  0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(radius, -radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4(radius,  0.0f,  0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(-radius,   radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4(0.0f,  radius, 0.0f, 0.0f)
	};
	Indices = { 0, 1, 2, 1, 0, 3 };
}

void FRenderComponent::AddMesh(float Scale, std::string PathToFile)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(PathToFile, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!pScene) 
	{
		return; 
	}

	ProcessNode(pScene->mRootNode, pScene, Scale);
}

void FRenderComponent::ProcessNode(aiNode* Node, const aiScene* Scene, float Scale)
{
	for (UINT i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		ProcessMesh(mesh, Scene, Scale);
	}

	for (UINT i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene, Scale);
	}
}

void FRenderComponent::ProcessMesh(aiMesh* Mesh, const aiScene* Scene, float Scale)
{
	for (UINT i = 0; i < Mesh->mNumVertices; i++)
	{
		DirectX::SimpleMath::Vector4 textureCoordinate = {};

		if (Mesh->mTextureCoords[0])
		{
			textureCoordinate.x = (float)Mesh->mTextureCoords[0][i].x;
			textureCoordinate.y = (float)Mesh->mTextureCoords[0][i].y;
		}

		Points.push_back({ Mesh->mVertices[i].x * Scale, Mesh->mVertices[i].y * Scale, Mesh->mVertices[i].z * Scale, 1.0f });
		Points.push_back(textureCoordinate);
	}

	for (UINT i = 0; i < Mesh->mNumFaces; i++)
	{
		aiFace face = Mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			Indices.push_back(face.mIndices[j]);
		}
	}
}
