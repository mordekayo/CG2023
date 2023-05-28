#include "DirectionalLightComponent.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "DisplayWin32.h"
#include "Components/TransformComponent.h"
#include "Render/GBuffer.h"
#include "Render/RenderSystem.h"
#include "Render/ShadowsRenderSystem.h"

struct alignas(16) CameraData
{
	DirectX::SimpleMath::Matrix  CameraViewMatrix;
	DirectX::SimpleMath::Vector3 CameraPosition;
};
struct alignas(16) LightData
{
	DirectX::SimpleMath::Vector4 Color;
	DirectX::SimpleMath::Vector4 Direction;
};
struct alignas(16) ShadowData
{
	DirectX::SimpleMath::Matrix ViewProjMats[4];
	float  Distances[4];
};

FDirectionalLightComponent::FDirectionalLightComponent(int ShadowMapSize, float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	this->ShadowMapSize = ShadowMapSize;
	this->ViewWidth     = ViewWidth;
	this->ViewHeight    = ViewHeight;
	this->NearZ         = NearZ;
	this->FarZ          = FarZ;
}

void FDirectionalLightComponent::Initialize()
{
	D3D11_TEXTURE2D_DESC TextureDesc = {};
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));
	TextureDesc.Width = static_cast<UINT>(ShadowMapSize);
	TextureDesc.Height = static_cast<UINT>(ShadowMapSize);
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 4;
	TextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;
	
	auto Result = FGame::Instance()->GetRenderSystem()->Device->CreateTexture2D(&TextureDesc, nullptr, ShadowMapTexture2D.GetAddressOf());
	assert(SUCCEEDED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	DepthStencilViewDesc.Texture2DArray.MipSlice = 0;
	DepthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
	DepthStencilViewDesc.Texture2DArray.ArraySize = 4;
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateDepthStencilView(ShadowMapTexture2D.Get(), &DepthStencilViewDesc, DepthStencilView.GetAddressOf());
	assert(SUCCEEDED(Result));

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	ShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2DArray.MipLevels = 1;
	ShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	ShaderResourceViewDesc.Texture2DArray.ArraySize = 4;
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateShaderResourceView(ShadowMapTexture2D.Get(), &ShaderResourceViewDesc, TextureResourceView.GetAddressOf());
	assert(SUCCEEDED(Result));

	Viewport = std::make_shared<D3D11_VIEWPORT>();
	Viewport->Width = static_cast<float>(ShadowMapSize);
	Viewport->Height = static_cast<float>(ShadowMapSize);
	Viewport->MinDepth = 0.0f;
	Viewport->MaxDepth = 1.0f;
	Viewport->TopLeftX = 0.0f;
	Viewport->TopLeftY = 0.0f;

	
	ConstBuffer = new ID3D11Buffer * [3];

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
	SecondConstBufferDesc.ByteWidth = sizeof(LightData);
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&SecondConstBufferDesc, nullptr, &ConstBuffer[1]);
	assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC thirdConstBufferDesc = {};
	thirdConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	thirdConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	thirdConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	thirdConstBufferDesc.MiscFlags = 0;
	thirdConstBufferDesc.StructureByteStride = 0;
	thirdConstBufferDesc.ByteWidth = sizeof(ShadowData);
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateBuffer(&thirdConstBufferDesc, nullptr, &ConstBuffer[2]);
	assert(SUCCEEDED(result));
}

void FDirectionalLightComponent::Draw()
{
	const CameraData cameraData
	{
		FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetView(),
		FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetPosition()
	};
	D3D11_MAPPED_SUBRESOURCE FirstMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &FirstMappedResource);
	memcpy(FirstMappedResource.pData, &cameraData, sizeof(CameraData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[0], 0);
	
	// DIRECTIONAL LIGHT
	const LightData lightData
	{
		FGame::Instance()->DirectionalLight->Color,
		FGame::Instance()->DirectionalLight->Direction
	};
	D3D11_MAPPED_SUBRESOURCE secondMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &secondMappedResource);
	memcpy(secondMappedResource.pData, &lightData, sizeof(LightData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[1], 0);

	const ShadowData lightShadowData
	{
		{
			FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(0), FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(1),
			FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(2), FGame::Instance()->DirectionalLight->LightViewProjectionMatrices.at(3)
		},
		{
			FGame::Instance()->DirectionalLight->ShadowCascadeLevels.at(0), FGame::Instance()->DirectionalLight->ShadowCascadeLevels.at(1),
			FGame::Instance()->DirectionalLight->ShadowCascadeLevels.at(2), FGame::Instance()->DirectionalLight->ShadowCascadeLevels.at(3)
		}
	};
	D3D11_MAPPED_SUBRESOURCE thirdMappedResource;
	FGame::Instance()->GetRenderSystem()->Context->Map(ConstBuffer[2], 0, D3D11_MAP_WRITE_DISCARD, 0, &thirdMappedResource);
	memcpy(thirdMappedResource.pData, &lightShadowData, sizeof(ShadowData));
	FGame::Instance()->GetRenderSystem()->Context->Unmap(ConstBuffer[2], 0);

	///

	FGame::Instance()->GetRenderSystem()->Context->OMSetBlendState(FGame::Instance()->GetRenderSystem()->LightBlendState.Get(), nullptr, 0xffffffff); ///

	ID3D11ShaderResourceView* resources[] = {
		FGame::Instance()->GetRenderSystem()->GBuffer->DiffuseSRV,
		FGame::Instance()->GetRenderSystem()->GBuffer->NormalSRV,
		FGame::Instance()->GetRenderSystem()->GBuffer->WorldPositionSRV
	};
	FGame::Instance()->GetRenderSystem()->Context->PSSetShaderResources(0, 3, resources);
	FGame::Instance()->GetRenderSystem()->Context->PSSetShaderResources(3, 1, FGame::Instance()->DirectionalLight->TextureResourceView.GetAddressOf());
	FGame::Instance()->GetRenderSystem()->Context->PSSetSamplers(0, 1, FGame::Instance()->GetRenderShadowsSystem()->SamplerState.GetAddressOf());

	//DIRECTIONAL
	FGame::Instance()->GetRenderSystem()->Context->RSSetState(FGame::Instance()->GetRenderSystem()->CullBackRasterizerState.Get());
	FGame::Instance()->GetRenderSystem()->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	FGame::Instance()->GetRenderSystem()->Context->OMSetDepthStencilState(FGame::Instance()->GetRenderSystem()->DirectLightDepthStencilState.Get(), 0);

	FGame::Instance()->GetRenderSystem()->Context->IASetInputLayout(nullptr);
	FGame::Instance()->GetRenderSystem()->Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetShader(FGame::Instance()->GetRenderSystem()->DirectLightVertexShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->PSSetShader(FGame::Instance()->GetRenderSystem()->DirectLightPixelShader.Get(), nullptr, 0);
	FGame::Instance()->GetRenderSystem()->Context->GSSetShader(nullptr, nullptr, 0);

	FGame::Instance()->GetRenderSystem()->Context->VSSetConstantBuffers(0, 3, ConstBuffer);
	FGame::Instance()->GetRenderSystem()->Context->PSSetConstantBuffers(0, 3, ConstBuffer);

	FGame::Instance()->GetRenderSystem()->Context->Draw(4, 0);
}


DirectX::SimpleMath::Matrix FDirectionalLightComponent::GetViewMatrix() const
{
	return GameObject->TransformComponent->GetView();
}

DirectX::SimpleMath::Matrix FDirectionalLightComponent::GetProjectionMatrix() const
{
	return DirectX::XMMatrixOrthographicLH(ViewWidth, ViewHeight, NearZ, FarZ);
}

std::vector<DirectX::SimpleMath::Vector4> FDirectionalLightComponent::GetFrustumCornerWorldSpace(const DirectX::SimpleMath::Matrix& View, const DirectX::SimpleMath::Matrix& Proj) const
{
	const auto ViewProj = View * Proj;
	const auto InvertedViewProjection = ViewProj.Invert();

	std::vector<DirectX::SimpleMath::Vector4> FrustumCorners;
	FrustumCorners.reserve(8);
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const DirectX::SimpleMath::Vector4 Pt =
					DirectX::SimpleMath::Vector4::Transform(DirectX::SimpleMath::Vector4(
						2.0f * static_cast<float>(x) - 1.0f,
						2.0f * static_cast<float>(y) - 1.0f,
						static_cast<float>(z),
						1.0f), InvertedViewProjection);
				FrustumCorners.push_back(Pt / Pt.w);
			}
		}
	}
	return FrustumCorners;
}

DirectX::SimpleMath::Matrix FDirectionalLightComponent::GetLightSpaceMatrix(const float NearZ, const float FarZ) const
{
	DirectX::SimpleMath::Matrix Rotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI);
	const DirectX::SimpleMath::Matrix Perspective = Rotation * DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		FGame::Instance()->CurrentCamera->FovAngle,
		FGame::Instance()->CurrentCamera->Aspect,
		NearZ,
		FarZ
	);
	const std::vector<DirectX::SimpleMath::Vector4> Corners = GetFrustumCornerWorldSpace(FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetView(), Perspective);

	DirectX::SimpleMath::Vector3 Center = DirectX::SimpleMath::Vector3::Zero;
	for (const auto& v : Corners)
	{
		Center += DirectX::SimpleMath::Vector3(v);
	}
	Center /= static_cast<float>(Corners.size());
	const auto LightView = DirectX::SimpleMath::Matrix::CreateLookAt(
		Center,
		Center + Direction,
		DirectX::SimpleMath::Vector3::Up
	);

	float MinX = std::numeric_limits<float>::max();
	float MaxX = std::numeric_limits<float>::lowest();
	float MinY = std::numeric_limits<float>::max();
	float MaxY = std::numeric_limits<float>::lowest();
	float MinZ = std::numeric_limits<float>::max();
	float MaxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : Corners)
	{
		const auto Trf = DirectX::SimpleMath::Vector4::Transform(v, LightView);
		MinX = std::min(MinX, Trf.x);
		MaxX = std::max(MaxX, Trf.x);
		MinY = std::min(MinY, Trf.y);
		MaxY = std::max(MaxY, Trf.y);
		MinZ = std::min(MinZ, Trf.z);
		MaxZ = std::max(MaxZ, Trf.z);
	}
	constexpr float zMult = 10.0f;

	MinZ = (MinZ < 0) ? MinZ * zMult : MinZ / zMult;
	MaxZ = (MaxZ < 0) ? MaxZ / zMult : MaxZ * zMult;

	auto LightProjection = DirectX::SimpleMath::Matrix::CreateOrthographicOffCenter(MinX, MaxX, MinY, MaxY, MinZ, MaxZ);
	return LightView * LightProjection;
}

std::vector<DirectX::SimpleMath::Matrix> FDirectionalLightComponent::GetLightSpaceMatrices() const
{
	std::vector<DirectX::SimpleMath::Matrix> Ret;
	Ret.push_back(GetLightSpaceMatrix(FGame::Instance()->CurrentCamera->NearZ, ShadowCascadeLevels[0]));
	Ret.push_back(GetLightSpaceMatrix(ShadowCascadeLevels[0], ShadowCascadeLevels[1]));
	Ret.push_back(GetLightSpaceMatrix(ShadowCascadeLevels[1], ShadowCascadeLevels[2]));
	Ret.push_back(GetLightSpaceMatrix(ShadowCascadeLevels[2], FGame::Instance()->CurrentCamera->FarZ));
	return Ret;
}
