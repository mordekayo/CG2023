#pragma once
#include "ObjectComponent.h"
#include "Utils/SimpleMath.h"
#include <d3d11.h>
#include <string>
#include <windows.h>
#include <wrl.h>
#include <vector>

#undef min
#undef max
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class FGameObject;

class FRenderComponent : public FObjectComponent
{
public:

    FRenderComponent(std::wstring PathToShader);
    FRenderComponent(std::wstring PathToShader, std::wstring PathToTexture);
    
    virtual ~FRenderComponent();
    void Init() override;
    void Update() override;
    virtual void Draw();

    void SetPoints(std::vector<DirectX::SimpleMath::Vector4>&& NewPoints);
    void SetIndices(std::vector<int> &&NewIndices);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY NewTopology);

    void AddGrid(int GridSize, float CellSize, DirectX::SimpleMath::Color Color);
    void SetMeshPlane(float Size);
    void AddCube(float radius);
    void AddSphere(float radius, int sliceCount, int stackCount, DirectX::XMFLOAT4 color);
    void AddMesh(float scaleRate, std::string MeshPath);

    void ProcessNode(aiNode* node, const aiScene* scene, float scaleRate);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, float scaleRate);
private:

    bool bUseTexture = false;
    std::wstring ShaderFileName;
    std::wstring TextureFileName;
    
    std::vector<DirectX::SimpleMath::Vector4> Points;
    std::vector<int> Indices;
    D3D11_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    
    UINT Strides[1];
    UINT Offsets[1];
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;
    ID3D11Buffer* ConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11Resource> Texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
};
