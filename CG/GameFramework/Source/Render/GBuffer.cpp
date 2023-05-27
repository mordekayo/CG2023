#include "GBuffer.h"
#include <cassert>

#include "DisplayWin32.h"
#include "Game.h"
#include "RenderSystem.h"

FGBuffer::FGBuffer()
{
	// DIFFUSE
	D3D11_TEXTURE2D_DESC DiffuseTCDesc = {};
	ZeroMemory(&DiffuseTCDesc, sizeof(D3D11_TEXTURE2D_DESC));
	DiffuseTCDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DiffuseTCDesc.MipLevels = 1;
	DiffuseTCDesc.ArraySize = 1;
	DiffuseTCDesc.SampleDesc.Count = 1;
	DiffuseTCDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	DiffuseTCDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	DiffuseTCDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	
	HRESULT Result = FGame::Instance()->GetRenderSystem()->Device->CreateTexture2D(&DiffuseTCDesc, nullptr, &DiffuseTextureCoordinates);
	assert(SUCCEEDED(Result));
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateRenderTargetView(DiffuseTextureCoordinates, nullptr, &DiffuseRTV);
	assert(SUCCEEDED(Result));

	D3D11_SHADER_RESOURCE_VIEW_DESC DiffuseSRVDesc;
	ZeroMemory(&DiffuseSRVDesc, sizeof(ID3D11ShaderResourceView));
	DiffuseSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DiffuseSRVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DiffuseSRVDesc.Texture2D.MostDetailedMip = 0;
	DiffuseSRVDesc.Texture2D.MipLevels = 1;
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateShaderResourceView(DiffuseTextureCoordinates, &DiffuseSRVDesc, &DiffuseSRV);
	assert(SUCCEEDED(Result));

	// NORMAL
	D3D11_TEXTURE2D_DESC NormalTextureCoordinatesDesc = {};
	ZeroMemory(&DiffuseTCDesc, sizeof(D3D11_TEXTURE2D_DESC));
	NormalTextureCoordinatesDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	NormalTextureCoordinatesDesc.MipLevels = 1;
	NormalTextureCoordinatesDesc.ArraySize = 1;
	NormalTextureCoordinatesDesc.SampleDesc.Count = 1;
	NormalTextureCoordinatesDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	NormalTextureCoordinatesDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	NormalTextureCoordinatesDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateTexture2D(&NormalTextureCoordinatesDesc, nullptr, &NormalTextureCoordinates);
	assert(SUCCEEDED(result));
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateRenderTargetView(NormalTextureCoordinates, nullptr, &NormalRTV);
	assert(SUCCEEDED(result));

	D3D11_SHADER_RESOURCE_VIEW_DESC NormalSRVDesc;
	ZeroMemory(&NormalSRVDesc, sizeof(ID3D11ShaderResourceView));
	NormalSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	NormalSRVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	NormalSRVDesc.Texture2D.MostDetailedMip = 0;
	NormalSRVDesc.Texture2D.MipLevels = 1;
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateShaderResourceView(NormalTextureCoordinates, &NormalSRVDesc, &NormalSRV);
	assert(SUCCEEDED(result));

	// WORLD POSITION
	D3D11_TEXTURE2D_DESC worldPositionTexDesc = {};
	ZeroMemory(&DiffuseTCDesc, sizeof(D3D11_TEXTURE2D_DESC));
	worldPositionTexDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	worldPositionTexDesc.MipLevels = 1;
	worldPositionTexDesc.ArraySize = 1;
	worldPositionTexDesc.SampleDesc.Count = 1;
	worldPositionTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	worldPositionTexDesc.Width = FGame::Instance()->GetDisplay()->GetScreenWidth();
	worldPositionTexDesc.Height = FGame::Instance()->GetDisplay()->GetScreenHeight();
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateTexture2D(&worldPositionTexDesc, nullptr, &WorldPositionTextureCoordinates);
	assert(SUCCEEDED(result));
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateRenderTargetView(WorldPositionTextureCoordinates, nullptr, &WorldPositionRTV);
	assert(SUCCEEDED(result));

	D3D11_SHADER_RESOURCE_VIEW_DESC WorldPositionSRVDesc;
	ZeroMemory(&WorldPositionSRVDesc, sizeof(ID3D11ShaderResourceView));
	WorldPositionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	WorldPositionSRVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	WorldPositionSRVDesc.Texture2D.MostDetailedMip = 0;
	WorldPositionSRVDesc.Texture2D.MipLevels = 1;
	
	Result = FGame::Instance()->GetRenderSystem()->Device->CreateShaderResourceView(WorldPositionTextureCoordinates, &WorldPositionSRVDesc, &WorldPositionSRV);
	assert(SUCCEEDED(result));
}

