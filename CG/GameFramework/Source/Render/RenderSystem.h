#pragma once

#include "Sources.h"
#include <vector>

class FGBuffer;
class FDisplayWin32;
class FRenderComponent;
class RenderShadowsComponent;

class FRenderSystem
{
public:

    FRenderSystem();
    
    void Draw();
    void EndFrame();

    void InitializeOpaqueShader(const std::string& ShaderFileName);
    void InitializeDirectLightShader(const std::string& ShaderFileName);
    void InitializePointLightShader(const std::string& ShaderFileName);
    
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

    Microsoft::WRL::ComPtr<ID3D11BlendState> OpaqueBlendState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> LightBlendState;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> OpaqueDepthStencilState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> LightingLessDepthStencilState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> LightingGreaterDepthStencilState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DirectLightDepthStencilState;
    
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> CullBackRasterizerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> CullFrontRasterizerState;

    FGBuffer* GBuffer;

    Microsoft::WRL::ComPtr<ID3D11InputLayout> OpaqueInputLayout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> DirectLightInputLayout;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> PointLightsInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> OpaqueVertexShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> DirectLightVertexShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> PointLightsVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> OpaquePixelShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> DirectLightPixelShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PointLightsPixelShader;
    
    std::vector<FRenderComponent*> RenderComponents;
};