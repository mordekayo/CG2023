#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

class FRenderSystem
{
public:

    void Init();

    [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
    [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const;
    
    void BeginFrame();
    void EndFrame();
    
private:
    
    Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> BackTex = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView = nullptr;
    D3D11_VIEWPORT Viewport;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthView;
};
