#pragma once

#include <vector>

#include "Sources.h"
#include "Components/Light/DirectionalLightComponent.h"

class FRenderShadowsComponent;

class FShadowsRenderSystem
{
public:

    FShadowsRenderSystem();

    void PrepareFrame();
    void Draw();
    void EndFrame();

    void InitializeShader(const std::string& ShaderFileName);

    Microsoft::WRL::ComPtr<ID3D11InputLayout>  InputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> RastState;

    std::vector<FRenderShadowsComponent*> RenderShadowsComponents;
};