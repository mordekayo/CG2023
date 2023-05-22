#include "RenderComponent.h"

#include <WICTextureLoader.h>
#include "Game.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Render/RenderSystem.h"
#include "GameObjects/GameObject.h"
#include "Light/DirectionalLightComponent.h"
#include "Render/ShadowsRenderSystem.h"

FRenderComponent::FRenderComponent(FMeshComponent* MeshComponent)
{
	this->MeshComponent = MeshComponent;
}

struct alignas(16) CameraData
{
	DirectX::SimpleMath::Matrix ViewMatrix;
	DirectX::SimpleMath::Matrix ProjectionMatrix;
	DirectX::SimpleMath::Matrix ModelMatrix;
	DirectX::SimpleMath::Vector3 CameraPosition;
};

struct alignas(16) LightData
{
	DirectX::SimpleMath::Vector4 LightDirection;
	DirectX::SimpleMath::Vector4 AmbientConstant;
	DirectX::SimpleMath::Vector4 DiffuseReflectionCoefficient;
	DirectX::SimpleMath::Vector4 AbsorptionCoef;
};

struct alignas(16) ShadowData
{
	DirectX::SimpleMath::Matrix ViewProjMats[4];
	float distances[4];
};

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

void FRenderComponent::Draw()
{
	const CameraData CameraData
	{
		FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetView(),
		FGame::Instance()->CurrentCamera->GetProjection(),
		GameObject->TransformComponent->GetModel(),
		FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetPosition()
	};
	D3D11_MAPPED_SUBRESOURCE FirstMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &FirstMappedResource);
	memcpy(FirstMappedResource.pData, &CameraData, sizeof(CameraData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[0], 0);

	const LightData lightData
	{
		FGame::Instance()->CurrentLight->Direction,
		MeshComponent->Ambient,
		MeshComponent->Diffuse,
		MeshComponent->Specular
	};
	D3D11_MAPPED_SUBRESOURCE SecondMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &SecondMappedResource);
	memcpy(SecondMappedResource.pData, &lightData, sizeof(LightData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[1], 0);

	const ShadowData LightShadowData
	{
		{
			FGame::Instance()->CurrentLight->LightViewProjectionMatrices.at(0), FGame::Instance()->CurrentLight->LightViewProjectionMatrices.at(1),
			FGame::Instance()->CurrentLight->LightViewProjectionMatrices.at(2), FGame::Instance()->CurrentLight->LightViewProjectionMatrices.at(3)
		}, //
		{
			FGame::Instance()->CurrentLight->ShadowCascadeLevels.at(0),         FGame::Instance()->CurrentLight->ShadowCascadeLevels.at(1),
			FGame::Instance()->CurrentLight->ShadowCascadeLevels.at(2),         FGame::Instance()->CurrentLight->ShadowCascadeLevels.at(3)
		} //
	};
	D3D11_MAPPED_SUBRESOURCE ThirdMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[2], 0, D3D11_MAP_WRITE_DISCARD, 0, &ThirdMappedResource);
	memcpy(ThirdMappedResource.pData, &LightShadowData, sizeof(ShadowData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[2], 0);

	FGame::Instance()->GetRenderSystem()->Context->PSSetShaderResources(0, 1, MeshComponent->TextureView.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->Context->PSSetSamplers(0, 1, FGame::Instance()->GetRenderSystem()->SamplerState.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->Context->PSSetShaderResources(1, 1, FGame::Instance()->CurrentLight->TextureResourceView.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->Context->PSSetSamplers(1, 1, FGame::Instance()->GetRenderShadowsSystem()->sSamplerState.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->Context->RSSetState(FGame::Instance()->GetRenderSystem()->RasterizerState.Get());
	FGame::Instance()->GetRenderSystem()->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	UINT strides[] { 48 };
	UINT offsets[] { 0 };
	FGame::Instance()->GetRenderSystem()->Context->IASetVertexBuffers(0, 1, MeshComponent->VertexBuffer.GetAddressOf(), strides, offsets);
	FGame::Instance()->GetRenderSystem()->Context->IASetInputLayout(FGame::Instance()->GetRenderSystem()->InputLayout.Get());
	FGame::Instance()->GetRenderSystem()->Context->IASetIndexBuffer(MeshComponent->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetShader(FGame::Instance()->GetRenderSystem()->VertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->PSSetShader(FGame::Instance()->GetRenderSystem()->PixelShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->GSSetShader(nullptr, nullptr, 0);
	
	FGame::Instance()->GetRenderSystem()->Context->VSSetConstantBuffers(0, 3, ConstBuffer);
	FGame::Instance()->GetRenderSystem()->Context->PSSetConstantBuffers(0, 3, ConstBuffer);
	
	FGame::Instance()->GetRenderSystem()->Context->DrawIndexed(MeshComponent->Indices.size(), 0, 0);
}