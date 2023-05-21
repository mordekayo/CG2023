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

    void InitializeShader(std::string shaderFileName);

    Microsoft::WRL::ComPtr<ID3D11InputLayout>  sInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> sVertexShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> sGeometryShader;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sSamplerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> sRastState;

    std::vector<FRenderShadowsComponent*> renderShadowsComponents;
};