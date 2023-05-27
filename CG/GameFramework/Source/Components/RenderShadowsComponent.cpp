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
	FGame::Instance()->GetRenderShadowsSystem()->RenderShadowsComponents.push_back(this);

	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;
	constBufferDesc.ByteWidth = sizeof(CascadeData);
	FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&constBufferDesc, nullptr, sConstBuffer.GetAddressOf());	
}

void FRenderShadowsComponent::Draw()
{
	FGame::Instance()->DirectionalLight->LightViewProjectionMatrices = FGame::Instance()->DirectionalLight->GetLightSpaceMatrices();
	const CascadeData cascadeData
	{
		GameObject->TransformComponent->GetModel(),
		{ 
			FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(0), FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(1),
		    FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(2), FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(3)
		} //
	};
	D3D11_MAPPED_SUBRESOURCE firstMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(sConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &firstMappedResource);
	memcpy(firstMappedResource.pData, &cascadeData, sizeof(CascadeData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(sConstBuffer.Get(), 0);


	FGame::Instance()->GetRenderSystem()->Context->RSSetState(FGame::Instance()->GetRenderShadowsSystem()->RastState.Get());	
	FGame::Instance()->GetRenderSystem()->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT strides[] = { 48 };
	UINT offsets[] = { 0 };
	FGame::Instance()->GetRenderSystem()->Context->IASetVertexBuffers(0, 1, MeshComponent->VertexBuffer.GetAddressOf(), strides, offsets); //
	FGame::Instance()->GetRenderSystem()->Context->IASetInputLayout(FGame::Instance()->GetRenderShadowsSystem()->InputLayout.Get());
	FGame::Instance()->GetRenderSystem()->Context->IASetIndexBuffer(MeshComponent->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetShader(FGame::Instance()->GetRenderShadowsSystem()->VertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->PSSetShader(nullptr, nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->GSSetShader(FGame::Instance()->GetRenderShadowsSystem()->GeometryShader.Get(), nullptr, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetConstantBuffers(0, 1, sConstBuffer.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->Context->GSSetConstantBuffers(0, 1, sConstBuffer.GetAddressOf());

	FGame::Instance()->GetRenderSystem()->Context->DrawIndexed(MeshComponent->Indices.size(), 0, 0);
}