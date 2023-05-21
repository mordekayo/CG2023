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

FRenderComponent::FRenderComponent(FMeshComponent* modelComponent)
{
	this->modelComponent = modelComponent;
}

struct alignas(16) CameraData
{
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;
	DirectX::SimpleMath::Matrix model;
	DirectX::SimpleMath::Vector3 cameraPosition;
};
struct RemLightData
{
	DirectX::SimpleMath::Vector4 Direction;
	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;
};
struct alignas(16) LightData
{
	RemLightData RemLight;
};

struct alignas(16) ShadowData
{
	DirectX::SimpleMath::Matrix viewProjMats[4]; //
	float distances[4];     //
};

void FRenderComponent::Initialize()
{
	FGame::Instance()->GetRenderSystem()->renderComponents.push_back(this);

	constBuffer = new ID3D11Buffer * [3];

	D3D11_BUFFER_DESC firstConstBufferDesc = {};
	firstConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	firstConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	firstConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	firstConstBufferDesc.MiscFlags = 0;
	firstConstBufferDesc.StructureByteStride = 0;
	firstConstBufferDesc.ByteWidth = sizeof(CameraData);
	auto result = FGame::Instance()->GetRenderSystem()->device->CreateBuffer(&firstConstBufferDesc, nullptr, &constBuffer[0]);
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC secondConstBufferDesc = {};
	secondConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	secondConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	secondConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	secondConstBufferDesc.MiscFlags = 0;
	secondConstBufferDesc.StructureByteStride = 0;
	secondConstBufferDesc.ByteWidth = sizeof(LightData);
	result = FGame::Instance()->GetRenderSystem()->device->CreateBuffer(&secondConstBufferDesc, nullptr, &constBuffer[1]);
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC thirdConstBufferDesc = {};
	thirdConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	thirdConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	thirdConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	thirdConstBufferDesc.MiscFlags = 0;
	thirdConstBufferDesc.StructureByteStride = 0;
	thirdConstBufferDesc.ByteWidth = sizeof(ShadowData);
	result = FGame::Instance()->GetRenderSystem()->device->CreateBuffer(&thirdConstBufferDesc, nullptr, &constBuffer[2]);
	assert(SUCCEEDED(result));
}

void FRenderComponent::Update(float deltaTime)
{	

}

void FRenderComponent::Draw()
{
	const CameraData cameraData
	{
		FGame::Instance()->currentCamera->gameObject->TransformComponent->GetView(),
		FGame::Instance()->currentCamera->GetProjection(),
		gameObject->TransformComponent->GetModel(),
		FGame::Instance()->currentCamera->gameObject->TransformComponent->GetPosition()
	};
	D3D11_MAPPED_SUBRESOURCE firstMappedResource;
	FGame::Instance()->GetRenderSystem()->context->Map(constBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &firstMappedResource);
	memcpy(firstMappedResource.pData, &cameraData, sizeof(CameraData));
	FGame::Instance()->GetRenderSystem()->context->Unmap(constBuffer[0], 0);

	const LightData lightData
	{
		RemLightData
		{
			FGame::Instance()->currentLight->direction,
			modelComponent->ambient,
			modelComponent->diffuse,
			modelComponent->specular
		}
	};
	D3D11_MAPPED_SUBRESOURCE secondMappedResource;
	FGame::Instance()->GetRenderSystem()->context->Map(constBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &secondMappedResource);
	memcpy(secondMappedResource.pData, &lightData, sizeof(LightData));
	FGame::Instance()->GetRenderSystem()->context->Unmap(constBuffer[1], 0);

	const ShadowData lightShadowData
	{
		{
			FGame::Instance()->currentLight->lightViewProjectionMatrices.at(0), FGame::Instance()->currentLight->lightViewProjectionMatrices.at(1),
			FGame::Instance()->currentLight->lightViewProjectionMatrices.at(2), FGame::Instance()->currentLight->lightViewProjectionMatrices.at(3)
		}, //
		{
			FGame::Instance()->currentLight->shadowCascadeLevels.at(0),         FGame::Instance()->currentLight->shadowCascadeLevels.at(1),
			FGame::Instance()->currentLight->shadowCascadeLevels.at(2),         FGame::Instance()->currentLight->shadowCascadeLevels.at(3)
		} //
	};
	D3D11_MAPPED_SUBRESOURCE thirdMappedResource;
	FGame::Instance()->GetRenderSystem()->context->Map(constBuffer[2], 0, D3D11_MAP_WRITE_DISCARD, 0, &thirdMappedResource);
	memcpy(thirdMappedResource.pData, &lightShadowData, sizeof(ShadowData));
	FGame::Instance()->GetRenderSystem()->context->Unmap(constBuffer[2], 0);

	FGame::Instance()->GetRenderSystem()->context->PSSetShaderResources(0, 1, modelComponent->textureView.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->context->PSSetSamplers(0, 1, FGame::Instance()->GetRenderSystem()->samplerState.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->context->PSSetShaderResources(1, 1, FGame::Instance()->currentLight->textureResourceView.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->context->PSSetSamplers(1, 1, FGame::Instance()->GetRenderShadowsSystem()->sSamplerState.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->context->RSSetState(FGame::Instance()->GetRenderSystem()->rastState.Get());
	FGame::Instance()->GetRenderSystem()->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	UINT strides[] { 48 };
	UINT offsets[] { 0 };
	FGame::Instance()->GetRenderSystem()->context->IASetVertexBuffers(0, 1, modelComponent->vertexBuffer.GetAddressOf(), strides, offsets);
	FGame::Instance()->GetRenderSystem()->context->IASetInputLayout(FGame::Instance()->GetRenderSystem()->inputLayout.Get());
	FGame::Instance()->GetRenderSystem()->context->IASetIndexBuffer(modelComponent->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->context->VSSetShader(FGame::Instance()->GetRenderSystem()->vertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->context->PSSetShader(FGame::Instance()->GetRenderSystem()->pixelShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->context->GSSetShader(nullptr, nullptr, 0);
	
	FGame::Instance()->GetRenderSystem()->context->VSSetConstantBuffers(0, 3, constBuffer);
	FGame::Instance()->GetRenderSystem()->context->PSSetConstantBuffers(0, 3, constBuffer);
	
	FGame::Instance()->GetRenderSystem()->context->DrawIndexed(modelComponent->indices.size(), 0, 0);
}