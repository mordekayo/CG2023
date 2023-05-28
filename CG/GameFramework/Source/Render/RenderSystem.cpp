#include "RenderSystem.h"

#include "DisplayWin32.h"
#include "Components/RenderComponent.h"
#include "Game.h"
#include "ShadowsRenderSystem.h"
#include "Components/Light/PointLightComponent.h"
#include "GameObjects/GameObject.h"
#include "Render/GBuffer.h"

FRenderSystem::FRenderSystem()
{
	constexpr D3D_FEATURE_LEVEL FeatureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
	
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	SwapChainDesc.BufferDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = FGame::Instance()->GetDisplay()->GetHWnd();
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	
	HRESULT Result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf()
	);
	assert(SUCCEEDED(result));
	
	Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(result));
	
	Result = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &RenderView);
	assert(SUCCEEDED(result));

	D3D11_TEXTURE2D_DESC DepthTextureDesc = {};
	DepthTextureDesc.ArraySize = 1;
	DepthTextureDesc.MipLevels = 1;
	DepthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthTextureDesc.CPUAccessFlags = 0;
	DepthTextureDesc.MiscFlags = 0;
	DepthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthTextureDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	DepthTextureDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	DepthTextureDesc.SampleDesc = { 1, 0 };
	
	Result = Device->CreateTexture2D(&DepthTextureDesc, nullptr, DepthBuffer.GetAddressOf());
	assert(SUCCEEDED(result));
	
	Result = Device->CreateDepthStencilView(DepthBuffer.Get(), nullptr, &DepthView);
	assert(SUCCEEDED(result));

	D3D11_BLEND_DESC OpaqueBlendStateDesc; 
	ZeroMemory(&OpaqueBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	OpaqueBlendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	OpaqueBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	OpaqueBlendStateDesc.IndependentBlendEnable = false;
	Device->CreateBlendState(&OpaqueBlendStateDesc, &OpaqueBlendState);

	D3D11_BLEND_DESC LightBlendStateDesc;
	ZeroMemory(&LightBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	LightBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	LightBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	LightBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	LightBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	LightBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	LightBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	LightBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	LightBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	LightBlendStateDesc.IndependentBlendEnable = false;
	Device->CreateBlendState(&LightBlendStateDesc, &LightBlendState);

	D3D11_DEPTH_STENCIL_DESC OpaqueDepthStencilDesc = {};
	OpaqueDepthStencilDesc.DepthEnable = TRUE;
	OpaqueDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	OpaqueDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Result = Device->CreateDepthStencilState(&OpaqueDepthStencilDesc, &OpaqueDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC LightingLessDepthStencilDesc = {};    ///
	LightingLessDepthStencilDesc.DepthEnable = TRUE;
	LightingLessDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	LightingLessDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Result = Device->CreateDepthStencilState(&LightingLessDepthStencilDesc, &LightingLessDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC LightingGreaterDepthStencilDesc = {}; ///
	LightingGreaterDepthStencilDesc.DepthEnable = TRUE;
	LightingGreaterDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	LightingGreaterDepthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	Result = Device->CreateDepthStencilState(&LightingGreaterDepthStencilDesc, &LightingGreaterDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC DirectLightDepthStencilStateDesc = {};
	DirectLightDepthStencilStateDesc.DepthEnable = FALSE;
	DirectLightDepthStencilStateDesc.StencilEnable = FALSE;
	DirectLightDepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	Result = Device->CreateDepthStencilState(&DirectLightDepthStencilStateDesc, &DirectLightDepthStencilState);
	
	Viewport = std::make_shared<D3D11_VIEWPORT>();
	Viewport->TopLeftX = 0;
	Viewport->TopLeftY = 0;
	Viewport->Width = static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenWidth());
	Viewport->Height = static_cast<float>(FGame::Instance()->GetDisplay()->GetScreenHeight());
	Viewport->MinDepth = 0;
	Viewport->MaxDepth = 1.0f;
	
	InitializeOpaqueShader("../GameFramework/Source/Shaders/DefferedOpaque.hlsl");
	InitializeDirectLightShader("../GameFramework/Source/Shaders/DefferedDirectLight.hlsl");
	InitializePointLightShader("../GameFramework/Source/Shaders/DefferedPointLights.hlsl");
	
	CD3D11_RASTERIZER_DESC CullBackRasterizerStateDesc = {};
	CullBackRasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	CullBackRasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	Result = Device->CreateRasterizerState(&CullBackRasterizerStateDesc, &CullBackRasterizerState);
	assert(SUCCEEDED(result));

	CD3D11_RASTERIZER_DESC CullFrontRasterizerStateDesc = {};
	CullFrontRasterizerStateDesc.CullMode = D3D11_CULL_FRONT;
	CullFrontRasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	Result = Device->CreateRasterizerState(&CullFrontRasterizerStateDesc, &CullFrontRasterizerState);
	assert(SUCCEEDED(result));
	
	D3D11_SAMPLER_DESC SamplerStateDesc = {};
	SamplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	
	Result = Device->CreateSamplerState(&SamplerStateDesc, SamplerState.GetAddressOf());
	assert(SUCCEEDED(result));
}

void FRenderSystem::InitializeOpaqueShader(const std::string& ShaderFileName)
{
	const std::wstring FileName(ShaderFileName.begin(), ShaderFileName.end());
	ID3DBlob* ErrorCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	
	auto Result = D3DCompileFromFile(
		FileName.c_str(),
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		VertexShaderByteCode.GetAddressOf(),
		&ErrorCode
	);
	
	if (FAILED(Result))
	{
		if (ErrorCode)
		{
			const char* CompileErrors = static_cast<char*>(ErrorCode->GetBufferPointer());
			std::cout << CompileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing Shader File: " << ShaderFileName << std::endl;
		}
		return;
	}
	
	Result = Device->CreateVertexShader(
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		nullptr, &OpaqueVertexShader
	);
	assert(SUCCEEDED(result));

	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderByteCode;
	Result = D3DCompileFromFile(
		FileName.c_str(),
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		PixelShaderByteCode.GetAddressOf(),
		&ErrorCode
	);
	assert(SUCCEEDED(Result));
	
	Result = Device->CreatePixelShader(
		PixelShaderByteCode->GetBufferPointer(),
		PixelShaderByteCode->GetBufferSize(),
		nullptr, &OpaquePixelShader
	);
	assert(SUCCEEDED(result));

	const D3D11_INPUT_ELEMENT_DESC InputElements[] = {
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
	Result = Device->CreateInputLayout(
		InputElements,
		3,
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		&OpaqueInputLayout
	);
	assert(SUCCEEDED(result));
}

void FRenderSystem::InitializeDirectLightShader(const std::string& ShaderFileName)
{
	const std::wstring FileName(ShaderFileName.begin(), ShaderFileName.end());
	ID3DBlob* ErrorCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	
	auto Result = D3DCompileFromFile(
		FileName.c_str(),
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		VertexShaderByteCode.GetAddressOf(),
		&ErrorCode
	);
	
	if (FAILED(Result))
	{
		if (ErrorCode)
		{
			const char* CompileErrors = static_cast<char*>(ErrorCode->GetBufferPointer());
			std::cout << CompileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing Shader File: " << ShaderFileName << std::endl;
		}
		return;
	}
	
	Result = Device->CreateVertexShader(
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		nullptr, &DirectLightVertexShader
	);
	assert(SUCCEEDED(result));

	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderByteCode;
	Result = D3DCompileFromFile(
		FileName.c_str(),
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		PixelShaderByteCode.GetAddressOf(),
		&ErrorCode
	);
	assert(SUCCEEDED(Result));
	
	Result = Device->CreatePixelShader(
		PixelShaderByteCode->GetBufferPointer(),
		PixelShaderByteCode->GetBufferSize(),
		nullptr, &DirectLightPixelShader
	);
	assert(SUCCEEDED(Result));

	const D3D11_INPUT_ELEMENT_DESC InputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	
	Result = Device->CreateInputLayout(
		InputElements,
		1,
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		&DirectLightInputLayout
	);
	assert(SUCCEEDED(Result));
}

void FRenderSystem::InitializePointLightShader(const std::string& ShaderFileName)
{
	const std::wstring FileName(ShaderFileName.begin(), ShaderFileName.end());
	ID3DBlob* ErrorCode = nullptr;
	
	Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderByteCode;
	auto Result = D3DCompileFromFile(
		FileName.c_str(),
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		VertexShaderByteCode.GetAddressOf(),
		&ErrorCode
	);
	if (FAILED(Result))
	{
		if (ErrorCode)
		{
			const char* CompileErrors = static_cast<char*>(ErrorCode->GetBufferPointer());
			std::cout << CompileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing Shader File: " << ShaderFileName << std::endl;
		}
		return;
	}

	Result = Device->CreateVertexShader(
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		nullptr, &PointLightsVertexShader
	);
	assert(SUCCEEDED(result));

	Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderByteCode;
	Result = D3DCompileFromFile(
		FileName.c_str(),
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		PixelShaderByteCode.GetAddressOf(),
		&ErrorCode
	);
	assert(SUCCEEDED(Result));
	
	Result = Device->CreatePixelShader(
		PixelShaderByteCode->GetBufferPointer(),
		PixelShaderByteCode->GetBufferSize(),
		nullptr, &PointLightsPixelShader
	);
	assert(SUCCEEDED(Result));

	constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	Result = Device->CreateInputLayout(
		InputElements,
		1,
		VertexShaderByteCode->GetBufferPointer(),
		VertexShaderByteCode->GetBufferSize(),
		&PointLightsInputLayout
	);
	assert(SUCCEEDED(Result));
}

void FRenderSystem::Draw()
{
	Context->ClearState();

	Context->ClearRenderTargetView(GBuffer->DiffuseRTV, DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	Context->ClearRenderTargetView(GBuffer->NormalRTV, DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	Context->ClearRenderTargetView(GBuffer->WorldPositionRTV, DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	
	ID3D11RenderTargetView* RenderTargetViews[] = {
		GBuffer->DiffuseRTV,
		GBuffer->NormalRTV,
		GBuffer->WorldPositionRTV,
		nullptr, nullptr, nullptr, nullptr, nullptr
	};
	Context->OMSetRenderTargets(8, RenderTargetViews, DepthView.Get());

	Context->RSSetViewports(1, Viewport.get());

	Context->ClearDepthStencilView(DepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (FRenderComponent* RenderComponent : RenderComponents)
	{
		RenderComponent->DrawOpaque();
	}

	Context->ClearState();
	Context->OMSetRenderTargets(1, RenderView.GetAddressOf(), DepthView.Get());
	Context->RSSetViewports(1, Viewport.get());
	constexpr float backgroundColor[] = { 0.2f, 0.2f, 0.2f };
	Context->ClearRenderTargetView(RenderView.Get(), backgroundColor);

	FGame::Instance()->DirectionalLight->Draw();

	for (const auto PointLight : FGame::Instance()->PointLights)
	{
		PointLight->Draw();
	}
}

void FRenderSystem::EndFrame()
{
	Context->OMSetRenderTargets(0, nullptr, nullptr);
	SwapChain->Present(1, 0);
}