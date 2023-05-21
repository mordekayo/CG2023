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

    void InitializeShader(std::string shaderFileName);

    std::shared_ptr<D3D11_VIEWPORT> viewport;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthView;

    Microsoft::WRL::ComPtr<ID3D11InputLayout>     inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>    vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>     pixelShader;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>    samplerState;

    std::vector<FRenderComponent*> renderComponents;
};