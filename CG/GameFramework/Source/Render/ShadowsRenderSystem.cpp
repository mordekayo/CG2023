#include "ShadowsRenderSystem.h"

#include "DisplayWin32.h"
#include "Components/RenderComponent.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Components/RenderShadowsComponent.h"

void FShadowsRenderSystem::InitializeShader(std::string shaderFileName)
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
			std::cout << "Missing Shader File: " << std::endl;
		}
		return;
	}
	result = FGame::Instance()->GetRenderSystem()->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, sVertexShader.GetAddressOf()
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
		}
	};
	result = FGame::Instance()->GetRenderSystem()->Device->CreateInputLayout(
		inputElements,
		1,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		sInputLayout.GetAddressOf()
	);
	assert(SUCCEEDED(result));

	Microsoft::WRL::ComPtr<ID3DBlob> geometryShaderByteCode;
	result = D3DCompileFromFile(
		fileName.c_str(),
		nullptr,
		nullptr,
		"GSMain",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		geometryShaderByteCode.GetAddressOf(),
		&errorCode
	);
	assert(SUCCEEDED(result));
	result = FGame::Instance()->GetRenderSystem()->Device->CreateGeometryShader(
		geometryShaderByteCode->GetBufferPointer(),
		geometryShaderByteCode->GetBufferSize(),
		nullptr, sGeometryShader.GetAddressOf()
	);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	result = FGame::Instance()->GetRenderSystem()->Device->CreateRasterizerState(&rastDesc, sRastState.GetAddressOf());
	assert(SUCCEEDED(result));
}


FShadowsRenderSystem::FShadowsRenderSystem()
{
	//SHADER
	InitializeShader("../GameFramework/Source/Shaders/DepthShader.hlsl");

	D3D11_SAMPLER_DESC sComparisonSamplerDesc;
	ZeroMemory(&sComparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	sComparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sComparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sComparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sComparisonSamplerDesc.BorderColor[0] = 1.0f;
	sComparisonSamplerDesc.BorderColor[1] = 1.0f;
	sComparisonSamplerDesc.BorderColor[2] = 1.0f;
	sComparisonSamplerDesc.BorderColor[3] = 1.0f;
	sComparisonSamplerDesc.MinLOD = 0.f;
	sComparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sComparisonSamplerDesc.MipLODBias = 0.f;
	sComparisonSamplerDesc.MaxAnisotropy = 0;
	sComparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;        //
	sComparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; //
	auto result = FGame::Instance()->GetRenderSystem()->Device->CreateSamplerState(&sComparisonSamplerDesc, sSamplerState.GetAddressOf());
	assert(SUCCEEDED(result));
}

void FShadowsRenderSystem::PrepareFrame()
{
	FGame::Instance()->GetRenderSystem()->Context->RSSetState(sRastState.Get());
	FGame::Instance()->GetRenderSystem()->Context->OMSetRenderTargets(0, nullptr, FGame::Instance()->CurrentLight->depthStencilView.Get());
	FGame::Instance()->GetRenderSystem()->Context->RSSetViewports(1, FGame::Instance()->CurrentLight->viewport.get());
	FGame::Instance()->GetRenderSystem()->Context->ClearDepthStencilView(FGame::Instance()->CurrentLight->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void FShadowsRenderSystem::Draw()
{
	for (FRenderShadowsComponent* RenderShadowsComponent : renderShadowsComponents)
	{
		RenderShadowsComponent->Draw();
	}
}

void FShadowsRenderSystem::EndFrame()
{
	FGame::Instance()->GetRenderSystem()->Context->OMSetRenderTargets(0, nullptr, nullptr);
}