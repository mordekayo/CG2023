#pragma once

#include "Game.h"
#include "Components/ObjectComponent.h"
#include "Components/CameraComponent.h"

class FDirectionalLightComponent : public FObjectComponent
{
public:

    FDirectionalLightComponent(int shadowMapSize, float viewWidth, float viewHeight, float nearZ, float farZ);
    FDirectionalLightComponent() = delete;
	
    virtual void Initialize() override;

    DirectX::SimpleMath::Vector4 direction { DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.1f) };
    int shadowMapSize;
    float viewWidth;
    float viewHeight;
    float nearZ;
    float farZ;

    std::vector<DirectX::SimpleMath::Matrix> lightViewProjectionMatrices;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> shadowMapTexture2D;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    std::shared_ptr<D3D11_VIEWPORT> viewport;

    DirectX::SimpleMath::Matrix GetViewMatrix();
    DirectX::SimpleMath::Matrix GetProjectionMatrix();

    std::vector<float> shadowCascadeLevels //
    {
        FGame::Instance()->CurrentCamera->FarZ / 20,
        FGame::Instance()->CurrentCamera->FarZ / 10,
        FGame::Instance()->CurrentCamera->FarZ / 5,
        FGame::Instance()->CurrentCamera->FarZ
    };
    std::vector<DirectX::SimpleMath::Vector4> GetFrustumCornerWorldSpace(const DirectX::SimpleMath::Matrix& projection, const DirectX::SimpleMath::Matrix& view); //
    DirectX::SimpleMath::Matrix GetLightSpaceMatrix(const float nearZ, const float farZ); //
    std::vector<DirectX::SimpleMath::Matrix> GetLightSpaceMatrices(); //
};