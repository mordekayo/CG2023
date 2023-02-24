#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "GameObject.h"

class Triangle : GameObject
{
public:

    Triangle();

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
    Microsoft::WRL::ComPtr<D3D11_VIEWPORT> Viewport;
};
