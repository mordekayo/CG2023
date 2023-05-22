#pragma once

#include "Sources.h"
#include <vector>

class DisplayWin32;
class FRenderComponent;
class RenderShadowsComponent;

class FRenderSystem
{
public:

    FRenderSystem();

    void PrepareFrame();
    void Draw();
    void EndFrame();

    void InitializeShader(const std::string& ShaderFileName);

    std::shared_ptr<D3D11_VIEWPORT> Viewport;
    Microsoft::WRL::ComPtr<ID3D11Device> Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthView;

    Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;

    std::vector<FRenderComponent*> RenderComponents;
};