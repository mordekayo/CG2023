#pragma once

#include <d3d11.h>
#include <windows.h>
#include <wrl.h>
#include "GameObject.h"

class Triangle : public GameObject
{
public:

    Triangle();

    void Init() override;
    void Update() override;
    void Draw() override;

private:

    UINT Strides[1];
    UINT Offsets[1];
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
};
