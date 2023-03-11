#pragma once
#include "ObjectComponent.h"
#include "Utils/SimpleMath.h"
#include <d3d11.h>
#include <windows.h>
#include <wrl.h>
#include <vector>

class FGameObject;

class FRenderComponent : public FObjectComponent
{
public:

    virtual ~FRenderComponent();
    void Init() override;
    void Update() override;
    virtual void Draw();

    void SetPoints(std::vector<DirectX::SimpleMath::Vector4>&& NewPoints);
    void SetIndices(std::vector<int> &&NewIndices);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY NewTopology);

private:
    
    std::vector<DirectX::SimpleMath::Vector4> LocalPoints;
    std::vector<int> Indices;
    D3D11_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    
    UINT Strides[1];
    UINT Offsets[1];
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    ID3D11Buffer* ConstantBuffer;
};
