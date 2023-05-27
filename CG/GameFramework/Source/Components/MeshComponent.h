#pragma once
#include "ObjectComponent.h"

#undef min
#undef max
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct FMaterial
{
    DirectX::SimpleMath::Vector4 DiffuseReflectionCoefficient { DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f) };
    DirectX::SimpleMath::Vector4 AbsorptionCoef{ DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f) };
    DirectX::SimpleMath::Vector4 AmbientConstant { DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f) };
};

class FMeshComponent : public FObjectComponent
{
public:

    FMeshComponent(const std::string& TextureFileName);
    FMeshComponent() = delete;

    virtual void Initialize() override;
    
    FMaterial Material;

    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    std::vector<DirectX::SimpleMath::Vector4> Points;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    std::vector<int> Indices;

    std::string TextureFileName;
    Microsoft::WRL::ComPtr<ID3D11Resource> Texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureView;

    void AddPlane(float PlaneSize);

    void AddMesh(float ScaleRate, const std::string& ObjectFileName);
    void ProcessNode(aiNode* Node, const aiScene* Scene, float ScaleRate);
    void ProcessMesh(aiMesh* Mesh, const aiScene* Scene, float ScaleRate);
};

