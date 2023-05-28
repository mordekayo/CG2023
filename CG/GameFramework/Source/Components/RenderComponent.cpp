#include "RenderComponent.h"

#include <WICTextureLoader.h>
#include "Game.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Render/RenderSystem.h"
#include "GameObjects/GameObject.h"
#include "Light/DirectionalLightComponent.h"
#include "Light/PointLightComponent.h"
#include "Render/ShadowsRenderSystem.h"
#include "Render/GBuffer.h"

struct alignas(16) CameraData
{
	DirectX::SimpleMath::Matrix ViewMatrix;
	DirectX::SimpleMath::Matrix ProjectionMatrix;
	DirectX::SimpleMath::Matrix ModelMatrix;
};

struct alignas(16) MaterialData
{
	DirectX::SimpleMath::Vector4 AmbientConstant;
	DirectX::SimpleMath::Vector4 DiffuseReflectionCoefficient;
	DirectX::SimpleMath::Vector4 AbsorptionCoef;
};

struct DirectionalLightData
{
	DirectX::SimpleMath::Vector4 Color;
	DirectX::SimpleMath::Vector4 Direction;
};
struct PointLightData
{
	DirectX::SimpleMath::Vector4 Color;
	DirectX::SimpleMath::Vector4 ConstLinearQuadCount;
	DirectX::SimpleMath::Vector4 Position;
};

struct alignas(16) LightData
{
	MaterialData Material;
	DirectionalLightData DirectionalLight;
	PointLightData PointLights;
};

struct alignas(16) ShadowData
{
	DirectX::SimpleMath::Matrix ViewProjMats[4];
	float distances[4];
};

FRenderComponent::FRenderComponent(FMeshComponent* MeshComponent)
{
	this->MeshComponent = MeshComponent;
}

void FRenderComponent::Initialize()
{
	FGame::Instance()->GetRenderSystem()->RenderComponents.push_back(this);

	ConstBuffer = new ID3D11Buffer * [3];

	D3D11_BUFFER_DESC FirstConstBufferDesc = {};
	FirstConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	FirstConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	FirstConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	FirstConstBufferDesc.MiscFlags = 0;
	FirstConstBufferDesc.StructureByteStride = 0;
	FirstConstBufferDesc.ByteWidth = sizeof(CameraData);
	auto Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&FirstConstBufferDesc, nullptr, &ConstBuffer[0]);
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC SecondConstBufferDesc = {};
	SecondConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	SecondConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	SecondConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	SecondConstBufferDesc.MiscFlags = 0;
	SecondConstBufferDesc.StructureByteStride = 0;
	SecondConstBufferDesc.ByteWidth = sizeof(LightData);
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&SecondConstBufferDesc, nullptr, &ConstBuffer[1]);
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC ThirdConstBufferDesc = {};
	ThirdConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ThirdConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ThirdConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ThirdConstBufferDesc.MiscFlags = 0;
	ThirdConstBufferDesc.StructureByteStride = 0;
	ThirdConstBufferDesc.ByteWidth = sizeof(ShadowData);
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&ThirdConstBufferDesc, nullptr, &ConstBuffer[2]);
	assert(SUCCEEDED(result));
}

void FRenderComponent::DrawOpaque()
{
	const CameraData cameraData
	{
		FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetView(),
		FGame::Instance()->CurrentCamera->GetProjection(),
		GameObject->TransformComponent->GetModel(),
	};
	D3D11_MAPPED_SUBRESOURCE firstMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &firstMappedResource);
	memcpy(firstMappedResource.pData, &cameraData, sizeof(CameraData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[0], 0);

	FGame::Instance()->GetRenderSystem()->Context->OMSetBlendState(FGame::Instance()->GetRenderSystem()->OpaqueBlendState.Get(), nullptr, 0xffffffff); ///
	FGame::Instance()->GetRenderSystem()->Context->OMSetDepthStencilState(FGame::Instance()->GetRenderSystem()->OpaqueDepthStencilState.Get(), 0);                    ///

	FGame::Instance()->GetRenderSystem()->Context->PSSetShaderResources(0, 1, MeshComponent->TextureView.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->Context->PSSetSamplers(0, 1, FGame::Instance()->GetRenderSystem()->SamplerState.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->Context->RSSetState(FGame::Instance()->GetRenderSystem()->CullFrontRasterizerState.Get()); ///
	FGame::Instance()->GetRenderSystem()->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT strides[]{ 48 };
	UINT offsets[]{ 0 };
	FGame::Instance()->GetRenderSystem()->Context->IASetVertexBuffers(0, 1, MeshComponent->VertexBuffer.GetAddressOf(), strides, offsets);
	FGame::Instance()->GetRenderSystem()->Context->IASetInputLayout(FGame::Instance()->GetRenderSystem()->OpaqueInputLayout.Get()); ///
	FGame::Instance()->GetRenderSystem()->Context->IASetIndexBuffer(MeshComponent->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetShader(FGame::Instance()->GetRenderSystem()->OpaqueVertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->PSSetShader(FGame::Instance()->GetRenderSystem()->OpaquePixelShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->GSSetShader(nullptr, nullptr, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetConstantBuffers(0, 1, &ConstBuffer[0]);
	FGame::Instance()->GetRenderSystem()->Context->PSSetConstantBuffers(0, 1, &ConstBuffer[0]);

	FGame::Instance()->GetRenderSystem()->Context->DrawIndexed(MeshComponent->Indices.size(), 0, 0);
}