#pragma once
#include "Sources.h"

class FGBuffer
{
public:

    ID3D11Texture2D* DiffuseTextureCoordinates = nullptr;
    ID3D11Texture2D* NormalTextureCoordinates = nullptr;
    ID3D11Texture2D* WorldPositionTextureCoordinates = nullptr;

    ID3D11ShaderResourceView* DiffuseSRV = nullptr;
    ID3D11ShaderResourceView* NormalSRV = nullptr;
    ID3D11ShaderResourceView* WorldPositionSRV = nullptr;

    ID3D11RenderTargetView* DiffuseRTV = nullptr;
    ID3D11RenderTargetView* NormalRTV = nullptr;
    ID3D11RenderTargetView* WorldPositionRTV = nullptr;

    FGBuffer();
};
