#include "RenderShadowsComponent.h"
#include "Game.h"
#include "Render/RenderSystem.h"
#include "Render/ShadowsRenderSystem.h"
#include "Components/Light/DirectionalLightComponent.h"
#include "GameObjects/GameObject.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Render/ShadowsRenderSystem.h"

struct alignas(16) CascadeData
{
	Matrix model;
	Matrix viewProjMats[4]; //
};

FRenderShadowsComponent::FRenderShadowsComponent(FMeshComponent* modelComponent)
{
	this->MeshComponent = modelComponent;
}

void FRenderShadowsComponent::Initialize()
{
	FGame::Instance()->GetRenderShadowsSystem()->renderShadowsComponents.push_back(this);

	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;
	constBufferDesc.ByteWidth = sizeof(CascadeData);
	FGame::Instance()->GetRenderSystem()->device->CreateBuffer(&constBufferDesc, nullptr, sConstBuffer.GetAddressOf());	
}

void FRenderShadowsComponent::Draw()
{
	FGame::Instance()->currentLight->lightViewProjectionMatrices = FGame::Instance()->currentLight->GetLightSpaceMatrices();
	const CascadeData cascadeData
	{
		gameObject->TransformComponent->GetModel(),
		{ 
			FGame::Instance()->currentLight->lightViewProjectionMatrices.at(0), FGame::Instance()->currentLight->lightViewProjectionMatrices.at(1),
		    FGame::Instance()->currentLight->lightViewProjectionMatrices.at(2), FGame::Instance()->currentLight->lightViewProjectionMatrices.at(3)
		} //
	};
	D3D11_MAPPED_SUBRESOURCE firstMappedResource;
	FGame::Instance()->GetRenderSystem()->context->Map(sConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &firstMappedResource);
	memcpy(firstMappedResource.pData, &cascadeData, sizeof(CascadeData));
	FGame::Instance()->GetRenderSystem()->context->Unmap(sConstBuffer.Get(), 0);


	FGame::Instance()->GetRenderSystem()->context->RSSetState(FGame::Instance()->GetRenderShadowsSystem()->sRastState.Get());	
	FGame::Instance()->GetRenderSystem()->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT strides[] = { 48 };
	UINT offsets[] = { 0 };
	FGame::Instance()->GetRenderSystem()->context->IASetVertexBuffers(0, 1, MeshComponent->vertexBuffer.GetAddressOf(), strides, offsets); //
	FGame::Instance()->GetRenderSystem()->context->IASetInputLayout(FGame::Instance()->GetRenderShadowsSystem()->sInputLayout.Get());
	FGame::Instance()->GetRenderSystem()->context->IASetIndexBuffer(MeshComponent->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->context->VSSetShader(FGame::Instance()->GetRenderShadowsSystem()->sVertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->context->PSSetShader(nullptr, nullptr, 0);
	FGame::Instance()->GetRenderSystem()->context->GSSetShader(FGame::Instance()->GetRenderShadowsSystem()->sGeometryShader.Get(), nullptr, 0);

	FGame::Instance()->GetRenderSystem()->context->VSSetConstantBuffers(0, 1, sConstBuffer.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->context->GSSetConstantBuffers(0, 1, sConstBuffer.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->context->DrawIndexed(MeshComponent->indices.size(), 0, 0);
}