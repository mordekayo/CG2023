#include "DirectionalLightComponent.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "DisplayWin32.h"
#include "Components/TransformComponent.h"
#include "Render/RenderSystem.h"

FDirectionalLightComponent::FDirectionalLightComponent(int shadowMapSize, float viewWidth, float viewHeight, float nearZ, float farZ)
{
	this->shadowMapSize = shadowMapSize;
	this->viewWidth     = viewWidth;
	this->viewHeight    = viewHeight;
	this->nearZ         = nearZ;
	this->farZ          = farZ;
}

void FDirectionalLightComponent::Initialize()
{
	D3D11_TEXTURE2D_DESC textureDesc = {}; // ok
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = static_cast<UINT>(shadowMapSize);
	textureDesc.Height = static_cast<UINT>(shadowMapSize);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 4; // shadowCascadeLevels.size()
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	auto result = FGame::Instance()->GetRenderSystem()->Device->CreateTexture2D(&textureDesc, nullptr, shadowMapTexture2D.GetAddressOf());
	assert(SUCCEEDED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; // ok
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.MipSlice = 0;
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
	depthStencilViewDesc.Texture2DArray.ArraySize = 4; // shadowCascadeLevels.size()
	result = FGame::Instance()->GetRenderSystem()->Device->CreateDepthStencilView(shadowMapTexture2D.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
	assert(SUCCEEDED(result));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc; // ok
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	shaderResourceViewDesc.Texture2DArray.ArraySize = 4; // shadowCascadeLevels.size()
	result = FGame::Instance()->GetRenderSystem()->Device->CreateShaderResourceView(shadowMapTexture2D.Get(), &shaderResourceViewDesc, textureResourceView.GetAddressOf());
	assert(SUCCEEDED(result));

	viewport = std::make_shared<D3D11_VIEWPORT>(); // ok
	viewport->Width = static_cast<float>(shadowMapSize);
	viewport->Height = static_cast<float>(shadowMapSize);
	viewport->MinDepth = 0.0f;
	viewport->MaxDepth = 1.0f;
	viewport->TopLeftX = 0.0f;
	viewport->TopLeftY = 0.0f;
}

DirectX::SimpleMath::Matrix FDirectionalLightComponent::GetViewMatrix()
{
	return GameObject->TransformComponent->GetView();
}

DirectX::SimpleMath::Matrix FDirectionalLightComponent::GetProjectionMatrix()
{
	return DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}


std::vector<DirectX::SimpleMath::Vector4> FDirectionalLightComponent::GetFrustumCornerWorldSpace(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	const auto viewProj = view * proj;
	const auto inv = viewProj.Invert();

	std::vector<DirectX::SimpleMath::Vector4> frustumCorners;
	frustumCorners.reserve(8);
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const DirectX::SimpleMath::Vector4 pt =
					DirectX::SimpleMath::Vector4::Transform(DirectX::SimpleMath::Vector4(
						2.0f * static_cast<float>(x) - 1.0f,
						2.0f * static_cast<float>(y) - 1.0f,
						static_cast<float>(z),
						1.0f), inv);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}
	return frustumCorners;
}

DirectX::SimpleMath::Matrix FDirectionalLightComponent::GetLightSpaceMatrix(const float nearZ, const float farZ)
{
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI);
	const DirectX::SimpleMath::Matrix perspective = rotation * DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		FGame::Instance()->CurrentCamera->FovAngle,
		FGame::Instance()->CurrentCamera->Aspect,
		nearZ,
		farZ
	);
	const std::vector<DirectX::SimpleMath::Vector4> corners = GetFrustumCornerWorldSpace(FGame::Instance()->CurrentCamera->GameObject->TransformComponent->GetView(), perspective);

	DirectX::SimpleMath::Vector3 center = DirectX::SimpleMath::Vector3::Zero;
	for (const auto& v : corners)
	{
		center += DirectX::SimpleMath::Vector3(v);
	}
	center /= static_cast<float>(corners.size());
	const auto lightView = DirectX::SimpleMath::Matrix::CreateLookAt(
		center,
		center + direction,
		DirectX::SimpleMath::Vector3::Up
	);

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		const auto trf = DirectX::SimpleMath::Vector4::Transform(v, lightView);
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}
	constexpr float zMult = 10.0f;

	minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
	maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;

	auto lightProjection = DirectX::SimpleMath::Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);
	return lightView * lightProjection;
}

std::vector<DirectX::SimpleMath::Matrix> FDirectionalLightComponent::GetLightSpaceMatrices()
{
	std::vector<DirectX::SimpleMath::Matrix> ret;
	ret.push_back(GetLightSpaceMatrix(FGame::Instance()->CurrentCamera->NearZ, shadowCascadeLevels[0]));
	ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[0], shadowCascadeLevels[1]));
	ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[1], shadowCascadeLevels[2]));
	ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[2], FGame::Instance()->CurrentCamera->FarZ));
	return ret;
}