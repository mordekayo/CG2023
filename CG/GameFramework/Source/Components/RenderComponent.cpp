#include "RenderComponent.h"

#include <iostream>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include "Utils/SimpleMath.h"
#include "DisplayWin32.h"
#include "Game.h"
#include "Camera/Camera.h"
#include "GameObjects/GameObject.h"
#include "Render/RenderSystem.h"

#include <WICTextureLoader.h>


FRenderComponent::FRenderComponent(std::wstring PathToShader)
{
	ShaderFileName = PathToShader;
	bUseTexture = false;
}

FRenderComponent::FRenderComponent(std::wstring PathToShader, std::wstring PathToTexture)
{
	ShaderFileName = PathToShader;
	TextureFileName = PathToTexture;
	bUseTexture = true;
}

FRenderComponent::~FRenderComponent()
{
    
}

void FRenderComponent::Init()
{
	FObjectComponent::Init();

		Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	ID3DBlob* ErrorCode = nullptr;
	
	HRESULT Result = D3DCompileFromFile(ShaderFileName.c_str(),
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
	
	Result = D3DCompileFromFile(ShaderFileName.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&PixelShaderByteCode,
		&ErrorCode);

	FGame::GetRenderSystem()->GetDevice()->CreateVertexShader(
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		nullptr, VertexShader.GetAddressOf());

	FGame::GetRenderSystem()->GetDevice()->CreatePixelShader(
		PixelShaderByteCode->GetBufferPointer(),
		PixelShaderByteCode->GetBufferSize(),
		nullptr, PixelShader.GetAddressOf());

	D3D11_INPUT_ELEMENT_DESC InputElements[2];

	const D3D11_INPUT_ELEMENT_DESC InputPositionElementDesc =
		{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	};
	InputElements[0] = InputPositionElementDesc;
	
	if (bUseTexture)
	{
		const D3D11_INPUT_ELEMENT_DESC InputTexcoordinatesElementDesc =
		{
			"TEXCOORD", //
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		};
		InputElements[1] = InputTexcoordinatesElementDesc;
	}
	else
	{
		const D3D11_INPUT_ELEMENT_DESC InputColorElementDesc =
		{
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
		};
		InputElements[1] = InputColorElementDesc;
	}
	
	FGame::GetRenderSystem()->GetDevice()->CreateInputLayout(
	InputElements,
	2,
	VertexShaderByteCode->GetBufferPointer(),
	VertexShaderByteCode->GetBufferSize(),
	InputLayout.GetAddressOf()
);
	
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
	
	FGame::GetRenderSystem()->GetDevice()->CreateBuffer(&VertexBufDesc, &VertexData, VertexBuffer.GetAddressOf());

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
	
	FGame::GetRenderSystem()->GetDevice()->CreateBuffer(&IndexBufDesc, &IndexData, IndexBuffer.GetAddressOf());
	
	D3D11_BUFFER_DESC ConstBufDesc = {};
	ConstBufDesc.Usage = D3D11_USAGE_DEFAULT;
	ConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstBufDesc.CPUAccessFlags = 0;
	ConstBufDesc.MiscFlags = 0;
	ConstBufDesc.StructureByteStride = 0;
	ConstBufDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	FGame::GetRenderSystem()->GetDevice()->CreateBuffer(&ConstBufDesc, nullptr, &ConstantBuffer);

	if (bUseTexture)
	{
		DirectX::CreateWICTextureFromFile(
			FGame::GetRenderSystem()->GetDevice().Get(),
			FGame::GetRenderSystem()->GetContext().Get(),
			TextureFileName.c_str(),
			Texture.GetAddressOf(),
			TextureView.GetAddressOf()
		);

		D3D11_SAMPLER_DESC SamplerDesc = {};
		SamplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		// SamplerDesc.BorderColor[0] = 1.0f;
		// SamplerDesc.BorderColor[1] = 0.0f;
		// SamplerDesc.BorderColor[2] = 0.0f;
		// SamplerDesc.BorderColor[3] = 1.0f;
		SamplerDesc.MaxLOD = INT_MAX;

		FGame::Instance()->GetRenderSystem()->GetDevice()->CreateSamplerState(&SamplerDesc, SamplerState.GetAddressOf());
	}

	CD3D11_RASTERIZER_DESC RastDesc = {};
	RastDesc.CullMode = D3D11_CULL_BACK;
	RastDesc.FillMode = D3D11_FILL_SOLID;
	RastDesc.FrontCounterClockwise = true;

	FGame::GetRenderSystem()->GetDevice()->CreateRasterizerState(&RastDesc, RasterizerState.GetAddressOf());
}

void FRenderComponent::Update()
{
	FObjectComponent::Update();
	
	const DirectX::XMMATRIX OwnerWorldViewProjectionMatrix = FGame::Instance()->GetCurrentCamera()->GetViewProjectionMatrix(Owner->GetWorldTransform());
	
	const DirectX::XMMATRIX ScaledMatrix = DirectX::XMMatrixScaling(
			static_cast<float>(FGame::Instance()->GetDisplay().GetScreenHeight())/
			static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth()),
			1.0f,
			1.0f);

	const DirectX::XMMATRIX Transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(ScaledMatrix, OwnerWorldViewProjectionMatrix));

	FGame::GetRenderSystem()->GetContext()->UpdateSubresource(ConstantBuffer, 0, nullptr, &Transform, 0, 0);
}

void FRenderComponent::Draw()
{
	FGame::GetRenderSystem()->GetContext()->RSSetState(RasterizerState.Get());
	
    FGame::GetRenderSystem()->GetContext()->IASetInputLayout(InputLayout.Get());
    FGame::GetRenderSystem()->GetContext()->IASetPrimitiveTopology(Topology);
    FGame::GetRenderSystem()->GetContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    FGame::GetRenderSystem()->GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), Strides, Offsets);
    FGame::GetRenderSystem()->GetContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
    FGame::GetRenderSystem()->GetContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

	if (bUseTexture)
	{
		FGame::GetRenderSystem()->GetContext()->PSSetShaderResources(0, 1, TextureView.GetAddressOf()); //
		FGame::GetRenderSystem()->GetContext()->PSSetSamplers(0, 1, SamplerState.GetAddressOf()); //
	}
	
    FGame::GetRenderSystem()->GetContext()->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	
    FGame::GetRenderSystem()->GetContext()->DrawIndexed(static_cast<UINT>(Indices.size()), 0, 0);
}

void FRenderComponent::SetPoints(std::vector<DirectX::SimpleMath::Vector4>&& NewPoints)
{
	Points = std::move(NewPoints);
}

void FRenderComponent::SetIndices(std::vector<int>&& NewIndices)
{
	Indices = std::move(NewIndices);
}

void FRenderComponent::SetTopology(D3D11_PRIMITIVE_TOPOLOGY NewTopology)
{
	Topology = NewTopology;
}

void FRenderComponent::AddCube(float radius)
{
	Points = {
		DirectX::SimpleMath::Vector4(   radius,   radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4(radius * 2, radius * 2, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector4( - radius, - radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4( 0.0f,   0.0f,  0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(   radius, - radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4(radius * 2,  0.0f,  0.0f, 0.0f),
		DirectX::SimpleMath::Vector4( - radius,   radius, 0.0f, 1.0f), DirectX::SimpleMath::Vector4( 0.0f,  radius * 2, 0.0f, 0.0f)
	};
	Indices = { 0, 1, 2, 1, 0, 3 };
}
void FRenderComponent::AddSphere(float radius, int sliceCount, int stackCount, DirectX::XMFLOAT4 color)
{
	if (bUseTexture)
	{
		Points.push_back({ 0.0f, radius, 0.0f, 1.0f });
		Points.push_back({ 0.0f, 0.0f, 0.0f, 0.0f, });

		const float phiStep = DirectX::XM_PI / static_cast<float>(stackCount);
		const float thetaStep = DirectX::XM_2PI / static_cast<float>(sliceCount);

		for (int i = 1; i <= stackCount - 1; i++) {
			const float phi = static_cast<float>(i) * phiStep;

			for (int j = 0; j <= sliceCount; j++) {
				const float theta = static_cast<float>(j) * thetaStep;

				DirectX::SimpleMath::Vector4 tempPoint = {};
				DirectX::SimpleMath::Vector4 tempTexCoords = {};

				tempPoint.x = radius * sinf(phi) * cosf(theta);
				tempPoint.y = radius * cosf(phi);
				tempPoint.z = radius * sinf(phi) * sinf(theta);
				tempPoint.w = 1.0f;

				tempTexCoords.x = theta / DirectX::XM_2PI;
				tempTexCoords.y = phi / DirectX::XM_2PI;

				Points.push_back(tempPoint);
				Points.push_back(tempTexCoords);
			}
		}

		Points.push_back({ 0.0f, -radius, 0.0f, 1.0f });
		Points.push_back({ 0.0f, 1.0f, 0.0f, 0.0f, });

		for (int i = 1; i <= sliceCount; i++)
		{
			Indices.push_back(0);
			Indices.push_back(i + 1);
			Indices.push_back(i);
		}

		int baseIndex = 1;
		const int ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; i++)
		{
			for (int j = 0; j < sliceCount; j++)
			{
				Indices.push_back(baseIndex + i * ringVertexCount + j);
				Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		const int southPoleIndex = static_cast<int>(Points.size()) / 2 - 1;

		baseIndex = southPoleIndex - ringVertexCount;

		for (int i = 0; i < sliceCount; i++)
		{
			Indices.push_back(southPoleIndex);
			Indices.push_back(baseIndex + i);
			Indices.push_back(baseIndex + i + 1);
		}
	}
	else
	{
		int g = 0;
		Points.push_back(DirectX::XMFLOAT4(0, radius, 0, 1));
		Points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		auto phiStep = DirectX::XM_PI / stackCount;
		auto thetaStep = DirectX::XM_2PI / sliceCount;
		DirectX::XMFLOAT4 colorBase[] = { { 1.0f, 0.3f, 0.3f, 1.0f }, { 0.5f, 1.0f, 0.5f, 1.0f }, { 0.7f, 0.7f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.9f, 1.0f } };
		for (int i = 1; i <= stackCount - 1; i++)
		{
			auto phi = i * phiStep;
			for (int j = 0; j <= sliceCount; j++)
			{
				if (((j / 20 == g)) && (g < 4))
				{
					color = colorBase[g++];
				}
				auto theta = j * thetaStep;
				Points.push_back(
					DirectX::XMFLOAT4(
						radius * sin(phi) * cos(theta),
						radius * cos(phi),
						radius * sin(phi) * sin(theta),
						1.0f)
				);
				Points.push_back(color);
			}
			g = 0;
		}
		Points.push_back(DirectX::XMFLOAT4(0, -radius, 0, 1));
		Points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		for (int i = 1; i <= sliceCount; i++)
		{
			Indices.push_back(0);
			Indices.push_back(i + 1);
			Indices.push_back(i);
		}
		auto baseIndex = 1;
		auto ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; i++)
		{
			for (int j = 0; j < sliceCount; j++)
			{
				Indices.push_back(baseIndex + i * ringVertexCount + j);
				Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		const int southPoleIndex = static_cast<int>(Indices.size()) - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (int i = 0; i < sliceCount; i++)
		{
			Indices.push_back(southPoleIndex);
			Indices.push_back(baseIndex + i);
			Indices.push_back(baseIndex + i + 1);
		}
	}
}
void FRenderComponent::AddGrid(int GridSize, float CellSize, DirectX::SimpleMath::Color Color)
{
	for ( int Row = 0; Row < GridSize; ++Row )
	{
		for ( int Column = 0; Column < GridSize; ++Column)
		{
			Points.push_back({static_cast<float>(Column) * CellSize, 0.0f, static_cast<float>(Row) * CellSize, 1.0f});
		}
	}

	int i = 0;

	for ( int Row=0; Row < GridSize-1; ++Row )
	{
		if ((Row & 1) == 0)
		{
			// even rows
			for ( int Column = 0; Column < GridSize; ++Column )
			{
				Indices.push_back(Column + Row * GridSize);
				Indices.push_back(Column + (Row + 1) * GridSize);
			}
		}
		else
		{
			// odd rows
			for ( int Column = GridSize - 1; Column > 0; --Column)
			{
				Indices.push_back(Column + (Row+1) * GridSize);
				Indices.push_back(Column - 1 + Row * GridSize);
			}
		}
	}
}

void FRenderComponent::SetMeshPlane(float Size)
{
	
	Points =
	{
		DirectX::SimpleMath::Vector4(   Size, 0.0f, Size, 1.0f), DirectX::SimpleMath::Vector4(Size * 2, 0.0f, Size * 2, 0.0f),
		DirectX::SimpleMath::Vector4( Size, 0.0f, -Size, 1.0f), DirectX::SimpleMath::Vector4( 0.0f,   0.0f,  0.0f, 0.0f),
		DirectX::SimpleMath::Vector4(   -Size, 0.0f, -Size, 1.0f), DirectX::SimpleMath::Vector4(Size * 2,  0.0f,  0.0f, 0.0f),
		DirectX::SimpleMath::Vector4( -Size, 0.0f, Size, 1.0f), DirectX::SimpleMath::Vector4( 0.0f, 0.0f, Size * 2, 0.0f)
	};
	Indices = { 0, 1, 2, 2, 3, 0 };
}

void FRenderComponent::AddMesh(float scaleRate, std::string MeshPath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(MeshPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!pScene) { return; }

	ProcessNode(pScene->mRootNode, pScene, scaleRate);
}

void FRenderComponent::ProcessNode(aiNode* node, const aiScene* scene, float scaleRate)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, scaleRate);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, scaleRate);
	}
}

void FRenderComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene, float scaleRate)
{
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		DirectX::SimpleMath::Vector4 textureCoordinate = {};

		if (mesh->mTextureCoords[0])
		{
			textureCoordinate.x = (float)mesh->mTextureCoords[0][i].x;
			textureCoordinate.y = (float)mesh->mTextureCoords[0][i].y;
		}

		Points.push_back({ mesh->mVertices[i].x * scaleRate, mesh->mVertices[i].y * scaleRate, mesh->mVertices[i].z * scaleRate, 1.0f});
		Points.push_back(textureCoordinate);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			Indices.push_back(face.mIndices[j]);
		}
	}
}
