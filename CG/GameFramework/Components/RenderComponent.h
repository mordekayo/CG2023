#pragma once
#include "ObjectComponent.h"
#include <d3d11.h>
#include <windows.h>
#include <wrl.h>

class FRenderComponent : public FObjectComponent
{
public:

    virtual ~FRenderComponent();
    void Init() override;
    void Update() override;
    virtual void Draw();

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
