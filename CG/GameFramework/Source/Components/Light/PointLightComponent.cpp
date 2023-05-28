#include "PointLightComponent.h"

#include "Game.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "GameObjects/GameObject.h"
#include "Render/GBuffer.h"
#include "Render/RenderSystem.h"

struct alignas(16) CameraData
{
    DirectX::SimpleMath::Matrix  camView;
    DirectX::SimpleMath::Matrix  camProjection;
    DirectX::SimpleMath::Vector4 camPosition;
};

struct alignas(16) PointLightData
{
	DirectX::SimpleMath::Matrix  ModelMatrix;
	DirectX::SimpleMath::Vector4 Color;
	DirectX::SimpleMath::Vector4 ConstLinearQuadCount;
	DirectX::SimpleMath::Vector4 Position;
	DirectX::SimpleMath::Vector4 SphereRadius;
};

FPointLightComponent::FPointLightComponent(float InConstant, float InLinear, float InQuadratic)
{
    Constant  = InConstant;
    Linear = InLinear;
    Quadratic = InQuadratic;
}

void FPointLightComponent::Initialize()
{
	D3D11_BUFFER_DESC VertexBufDesc = {};
	VertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufDesc.CPUAccessFlags = 0;
	VertexBufDesc.MiscFlags = 0;
	VertexBufDesc.StructureByteStride = 0;
	VertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(Points);
	
	D3D11_SUBRESOURCE_DATA VertexData = {};
	VertexData.pSysMem = Points.data();
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	
	auto Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&VertexBufDesc, &VertexData, VertexBuffer.GetAddressOf());
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC IndexBufDesc = {};
	IndexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufDesc.CPUAccessFlags = 0;
	IndexBufDesc.MiscFlags = 0;
	IndexBufDesc.StructureByteStride = 0;
	IndexBufDesc.ByteWidth = sizeof(int) * std::size(Indices);
	
	D3D11_SUBRESOURCE_DATA IndexData = {};
	IndexData.pSysMem = Indices.data();
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&IndexBufDesc, &IndexData, IndexBuffer.GetAddressOf());
	assert(SUCCEEDED(result));

	ConstBuffer = new ID3D11Buffer * [2];

	D3D11_BUFFER_DESC FirstConstBufferDesc = {};
	FirstConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	FirstConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	FirstConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	FirstConstBufferDesc.MiscFlags = 0;
	FirstConstBufferDesc.StructureByteStride = 0;
	FirstConstBufferDesc.ByteWidth = sizeof(CameraData);
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&FirstConstBufferDesc, nullptr, &ConstBuffer[0]);
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC SecondConstBufferDesc = {};
	SecondConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	SecondConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	SecondConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	SecondConstBufferDesc.MiscFlags = 0;
	SecondConstBufferDesc.StructureByteStride = 0;
	SecondConstBufferDesc.ByteWidth = sizeof(PointLightData);
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&SecondConstBufferDesc, nullptr, &ConstBuffer[1]);
	assert(SUCCEEDED(result));
}

void FPointLightComponent::Draw()
{
	const CameraData cameraData
	{
		FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetView(),
		FGame::Instance()->CurrentCamera->GetProjection(),
		DirectX::SimpleMath::Vector4(FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetPosition())
	};
	D3D11_MAPPED_SUBRESOURCE firstMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &firstMappedResource);
	memcpy(firstMappedResource.pData, &cameraData, sizeof(CameraData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[0], 0);

	PointLightData LightData
	{
		GameObject->TransformComponent->GetModel(),
		DirectX::SimpleMath::Vector4(Color),
		DirectX::SimpleMath::Vector4(1.0f, 0.09f, 0.032f, 1.0f),
		DirectX::SimpleMath::Vector4(GameObject->TransformComponent->GetPosition()),
		DirectX::SimpleMath::Vector4(SphereRadius, 0.0f, 0.0f, 0.0f)
	};
	D3D11_MAPPED_SUBRESOURCE secondMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &secondMappedResource);
	memcpy(secondMappedResource.pData, &LightData, sizeof(LightData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[1], 0);

	ID3D11ShaderResourceView* resources[] = {
		FGame::Instance()->GetRenderSystem()->GBuffer->DiffuseSRV,
		FGame::Instance()->GetRenderSystem()->GBuffer->NormalSRV,
		FGame::Instance()->GetRenderSystem()->GBuffer->WorldPositionSRV
	};
	FGame::Instance()->GetRenderSystem()->Context->PSSetShaderResources(0, 3, resources);

	//POINT
	FGame::Instance()->GetRenderSystem()->Context->RSSetState(FGame::Instance()->GetRenderSystem()->CullFrontRasterizerState.Get());
	FGame::Instance()->GetRenderSystem()->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	FGame::Instance()->GetRenderSystem()->Context->OMSetDepthStencilState(FGame::Instance()->GetRenderSystem()->LightingGreaterDepthStencilState.Get(), 0);

	UINT strides[]{ 16 };
	UINT offsets[]{ 0 };
	FGame::Instance()->GetRenderSystem()->Context->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), strides, offsets);
	FGame::Instance()->GetRenderSystem()->Context->IASetInputLayout(FGame::Instance()->GetRenderSystem()->PointLightsInputLayout.Get()); ///
	FGame::Instance()->GetRenderSystem()->Context->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetShader(FGame::Instance()->GetRenderSystem()->PointLightsVertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->PSSetShader(FGame::Instance()->GetRenderSystem()->PointLightsPixelShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->GSSetShader(nullptr, nullptr, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetConstantBuffers(0, 2, ConstBuffer);
	FGame::Instance()->GetRenderSystem()->Context->PSSetConstantBuffers(0, 2, ConstBuffer);

	FGame::Instance()->GetRenderSystem()->Context->DrawIndexed(Indices.size(), 0, 0);
}

void FPointLightComponent::PoiAddMesh(float SphereRadius, const std::string& ObjectFileName)
{
	this->SphereRadius = SphereRadius;
	
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ObjectFileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!Scene)
	{
		return;
	}

	ProcessNode(Scene->mRootNode, Scene, SphereRadius);
}
void FPointLightComponent::ProcessNode(aiNode* Node, const aiScene* Scene, float SphereRadius)
{
	for (UINT i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		ProcessMesh(mesh, Scene, SphereRadius);
	}

	for (UINT i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene, SphereRadius);
	}
}
void FPointLightComponent::ProcessMesh(aiMesh* Mesh, const aiScene* Scene, float SphereRadius)
{
	for (UINT i = 0; i < Mesh->mNumVertices; i++)
	{
		Points.push_back({ Mesh->mVertices[i].x * SphereRadius, Mesh->mVertices[i].y * SphereRadius, Mesh->mVertices[i].z * SphereRadius, 1.0f });	
	}

	for (UINT i = 0; i < Mesh->mNumFaces; i++)
	{
		const aiFace Face = Mesh->mFaces[i];
		for (INT j = Face.mNumIndices - 1; j >= 0; j--)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}
}
