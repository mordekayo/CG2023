#include "MeshComponent.h"
#include "Game.h"
#include "Render/RenderSystem.h"
#include <WICTextureLoader.h>

FMeshComponent::FMeshComponent(const std::string& TextureFileName)
{
	this->TextureFileName = TextureFileName;
}

void FMeshComponent::Initialize()
{
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
	auto result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&VertexBufDesc, &VertexData, VertexBuffer.GetAddressOf());
	assert(SUCCEEDED(result));

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
	result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&IndexBufDesc, &IndexData, IndexBuffer.GetAddressOf());
	assert(SUCCEEDED(result));

	std::wstring TextureName(TextureFileName.begin(), TextureFileName.end());
	result = DirectX::CreateWICTextureFromFile(
		FGame::Instance()->GetRenderSystem()->Device.Get(),
		FGame::Instance()->GetRenderSystem()->Context.Get(),
		TextureName.c_str(),
		Texture.GetAddressOf(),
		TextureView.GetAddressOf()
	);
	assert(SUCCEEDED(result));
}

void FMeshComponent::AddPlane(float PlaneSize)
{
	Points =
	{
		Vector4(PlaneSize,   PlaneSize, 0.0f, 1.0f), Vector4(PlaneSize, PlaneSize, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(-PlaneSize, -PlaneSize, 0.0f, 1.0f), Vector4(0.0f,       0.0f,    0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(PlaneSize, -PlaneSize, 0.0f, 1.0f), Vector4(PlaneSize,   0.0f,    0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(-PlaneSize,   PlaneSize, 0.0f, 1.0f), Vector4(0.0f,     PlaneSize, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f)
	};
	Indices = { 0, 1, 2, 1, 0, 3 };
}
void FMeshComponent::AddMesh(float ScaleRate, const std::string& ObjectFileName)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ObjectFileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!Scene)
	{
		return;
	}

	ProcessNode(Scene->mRootNode, Scene, ScaleRate);
}
void FMeshComponent::ProcessNode(aiNode* Node, const aiScene* Scene, float ScaleRate)
{
	for (UINT i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
		ProcessMesh(Mesh, Scene, ScaleRate);
	}

	for (UINT i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene, ScaleRate);
	}
}
void FMeshComponent::ProcessMesh(aiMesh* Mesh, const aiScene* Scene, float ScaleRate)
{
	for (UINT i = 0; i < Mesh->mNumVertices; i++)
	{
		DirectX::SimpleMath::Vector4 TextureCoordinate = {};

		if (Mesh->mTextureCoords[0])
		{
			TextureCoordinate.x = (float)Mesh->mTextureCoords[0][i].x;
			TextureCoordinate.y = (float)Mesh->mTextureCoords[0][i].y;
			TextureCoordinate.z = 0;
			TextureCoordinate.w = 0;
		}

		DirectX::XMFLOAT4 Normal;
		Normal.x = Mesh->mNormals[i].x;
		Normal.y = Mesh->mNormals[i].y;
		Normal.z = Mesh->mNormals[i].z;
		Normal.w = 0;

		Points.push_back({ Mesh->mVertices[i].x * ScaleRate, Mesh->mVertices[i].y * ScaleRate, Mesh->mVertices[i].z * ScaleRate, 1.0f });
		Points.push_back(TextureCoordinate);
		Points.push_back(Normal);
	}

	for (UINT i = 0; i < Mesh->mNumFaces; i++)
	{
		aiFace Face = Mesh->mFaces[i];

		for (UINT j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}
}