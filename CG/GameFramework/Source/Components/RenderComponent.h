#pragma once
#include "ObjectComponent.h"
#include "Utils/SimpleMath.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <string>

#undef max
#undef min
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class FGameObject;

class FRenderComponent : public FObjectComponent
{
public:

    FRenderComponent(std::string ShaderFileName);
    FRenderComponent(std::string ShaderFileName, std::string TextureFileName);
    virtual ~FRenderComponent();
    void Init() override;
    void Update() override;
    virtual void Draw();

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY NewTopology);

    void AddMesh(float Scale, std::string PathToFile);
    void AddCube(float Radius);

private:

    void ProcessNode(aiNode* Node, const aiScene* Scene, float Scale);
    void ProcessMesh(aiMesh* Mesh, const aiScene* Scene, float Scale);
    
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
    ID3D11Buffer* ConstantBuffer;
       
    D3D11_VIEWPORT Viewport;


    Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;

    bool bUseTexture = false;
    std::string ShaderFileName;
    std::string TextureFileName;
    Microsoft::WRL::ComPtr<ID3D11Resource> Texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
};
