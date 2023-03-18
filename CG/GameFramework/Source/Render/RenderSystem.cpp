#include "RenderSystem.h"
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>

#include "DisplayWin32.h"
#include "Game.h"

void FRenderSystem::Init()
{
    constexpr D3D_FEATURE_LEVEL FeatureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
    SwapChainDesc.BufferCount = 2;
    SwapChainDesc.BufferDesc.Width = FGame::Instance()->GetDisplay().GetScreenWidth();
    SwapChainDesc.BufferDesc.Height = FGame::Instance()->GetDisplay().GetScreenHeight();
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = FGame::Instance()->GetDisplay().GetHWnd();
    SwapChainDesc.Windowed = true;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
	
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        FeatureLevel,
        1,
        D3D11_SDK_VERSION,
        &SwapChainDesc,
        &SwapChain,
        &Device,
        nullptr,
        &Context);
	
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackTex);	// __uuidof(ID3D11Texture2D)
    Device->CreateRenderTargetView(BackTex.Get(), nullptr, &RenderTargetView);
    
    D3D11_TEXTURE2D_DESC DepthTexDesc = {};
    DepthTexDesc.ArraySize = 1;
    DepthTexDesc.MipLevels = 1;
    DepthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DepthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DepthTexDesc.CPUAccessFlags = 0;
    DepthTexDesc.MiscFlags = 0;
    DepthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthTexDesc.Width = FGame::Instance()->GetDisplay().GetScreenWidth();
    DepthTexDesc.Height = FGame::Instance()->GetDisplay().GetScreenHeight();
    DepthTexDesc.SampleDesc = { 1, 0 };

    Device->CreateTexture2D(&DepthTexDesc, nullptr, &DepthBuffer);
    Device->CreateDepthStencilView(DepthBuffer.Get(), nullptr, &DepthView);
}

Microsoft::WRL::ComPtr<ID3D11Device> FRenderSystem::GetDevice() const
{
    return Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> FRenderSystem::GetContext() const
{
    return Context;
}

void FRenderSystem::BeginFrame()
{
    Viewport.Width = static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth());
    Viewport.Height = static_cast<float>(FGame::Instance()->GetDisplay().GetScreenHeight());
    Viewport.MinDepth = 0;
    Viewport.MaxDepth = 1.0f;
    Viewport.TopLeftX = 0;
    Viewport.TopLeftY = 0;

    Context->ClearState();
    Context->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), DepthView.Get());
    Context->RSSetViewports(1, &Viewport);
    
    const float Color[] = { 0.53f, 0.8f, 0.92f, 1.0f };
    Context->ClearRenderTargetView(RenderTargetView.Get(), Color);
    Context->ClearDepthStencilView(DepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void FRenderSystem::EndFrame()
{
    Context->OMSetRenderTargets(0, nullptr, nullptr);

    SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}