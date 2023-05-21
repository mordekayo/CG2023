#include "RenderSystem.h"

#include "DisplayWin32.h"
#include "Components/RenderComponent.h"
#include "Game.h"
#include "ShadowsRenderSystem.h"

FRenderSystem::FRenderSystem()
{
	//FRAME
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 }; //ok
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	swapChainDesc.BufferDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = FGame::Instance()->GetDisplay()->GetHWnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	auto result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		context.GetAddressOf()
	);
	assert(SUCCEEDED(result));
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(SUCCEEDED(result));
	result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderView);
	assert(SUCCEEDED(result));

	D3D11_TEXTURE2D_DESC depthTexDesc = {}; //ok
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	depthTexDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	depthTexDesc.SampleDesc = { 1, 0 };
	result = device->CreateTexture2D(&depthTexDesc, nullptr, depthBuffer.GetAddressOf());
	assert(SUCCEEDED(result));
	result = device->CreateDepthStencilView(depthBuffer.Get(), nullptr, &depthView);
	assert(SUCCEEDED(result));

	viewport = std::make_shared<D3D11_VIEWPORT>();
	viewport->TopLeftX = 0;
	viewport->TopLeftY = 0;
	viewport->Width = static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenWidth());
	viewport->Height = static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenHeight());
	viewport->MinDepth = 0;
	viewport->MaxDepth = 1.0f;

	//SHADERS
	InitializeShader("../GameFramework/Source/Shaders/LightAndShadowsShader.hlsl");

	//TEXTURE
	D3D11_SAMPLER_DESC samplerStateDesc = {};
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = device->CreateSamplerState(&samplerStateDesc, samplerState.GetAddressOf());
	assert(SUCCEEDED(result));
}

void FRenderSystem::InitializeShader(std::string shaderFileName)
{
	std::wstring fileName(shaderFileName.begin(), shaderFileName.end());
	ID3DBlob* errorCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderByteCode;
	auto result = D3DCompileFromFile(
		fileName.c_str(),
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		vertexShaderByteCode.GetAddressOf(),
		&errorCode
	);
	if (FAILED(result))
	{
		if (errorCode)
		{
			const char* compileErrors = (char*)(errorCode->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing Shader File: " << shaderFileName << std::endl;
		}
		return;
	}
	result = device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, vertexShader.GetAddressOf()
	);
	assert(SUCCEEDED(result));

	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderByteCode;
	result = D3DCompileFromFile(
		fileName.c_str(),
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		pixelShaderByteCode.GetAddressOf(),
		&errorCode
	);
	assert(SUCCEEDED(result));
	result = device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, pixelShader.GetAddressOf()
	);
	assert(SUCCEEDED(result));

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	result = device->CreateInputLayout(
		inputElements,
		3,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		inputLayout.GetAddressOf()
	);
	assert(SUCCEEDED(result));

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	result = device->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());
	assert(SUCCEEDED(result));
}

void FRenderSystem::PrepareFrame()
{
	context->ClearState();
	context->OMSetRenderTargets(1, renderView.GetAddressOf(), depthView.Get());
	context->RSSetViewports(1, viewport.get());
	float backgroundColor[] = { 0.2f, 0.2f, 0.2f };
	context->ClearRenderTargetView(renderView.Get(), backgroundColor);
	context->ClearDepthStencilView(depthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void FRenderSystem::Draw()
{
	for (auto& renderComponent : renderComponents)
	{
		renderComponent->Draw();
	}
}

void FRenderSystem::EndFrame()
{
	context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, 0);
}