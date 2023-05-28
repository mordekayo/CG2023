#pragma once

#include "Game.h"
#include "Components/ObjectComponent.h"
#include "Components/CameraComponent.h"
#include "Components/RenderComponent.h"

class FDirectionalLightComponent : public FObjectComponent
{
public:

    FDirectionalLightComponent(int ShadowMapSize, float ViewWidth, float ViewHeight, float NearZ, float FarZ);
    FDirectionalLightComponent() = delete;

    void Draw();

    ID3D11Buffer** ConstBuffer;
    
    virtual void Initialize() override;

    DirectX::SimpleMath::Vector4 Color = { DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f) };
    DirectX::SimpleMath::Vector4 Direction { DirectX::SimpleMath::Vector3(0.615457f, -0.615457f, 0.492366f) };
    int ShadowMapSize;
    float ViewWidth;
    float ViewHeight;
    float NearZ;
    float FarZ;

    std::vector<DirectX::SimpleMath::Matrix> LightViewProjectionMatrices;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> ShadowMapTexture2D;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureResourceView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
    std::shared_ptr<D3D11_VIEWPORT> Viewport;

    DirectX::SimpleMath::Matrix GetViewMatrix() const;
    DirectX::SimpleMath::Matrix GetProjectionMatrix() const;

    std::vector<float> ShadowCascadeLevels
    {
        FGame::Instance()->CurrentCamera->FarZ / 20,
        FGame::Instance()->CurrentCamera->FarZ / 10,
        FGame::Instance()->CurrentCamera->FarZ / 5,
        FGame::Instance()->CurrentCamera->FarZ
    };
    std::vector<DirectX::SimpleMath::Vector4> GetFrustumCornerWorldSpace(const DirectX::SimpleMath::Matrix& projection, const DirectX::SimpleMath::Matrix& view) const;
    DirectX::SimpleMath::Matrix GetLightSpaceMatrix(const float NearZ, const float FarZ) const;
    std::vector<DirectX::SimpleMath::Matrix> GetLightSpaceMatrices() const;
};
