#pragma once
#include "ObjectComponent.h"

#undef min
#undef max
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX::SimpleMath;

class FMeshComponent : public FObjectComponent
{
public:

    FMeshComponent(std::string textureFileName);
    FMeshComponent() = delete;

    virtual void Initialize() override;

    Vector4 ambient  { Vector3(0.5f, 0.5f, 0.5f) };
    Vector4 diffuse  { Vector3(0.2f, 0.5f, 0.9f) };
    Vector4 specular { Vector3(1.0f, 1.0f, 1.0f) };

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    std::vector<Vector4> points;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    std::vector<int> indices;

    std::string textureFileName;
    Microsoft::WRL::ComPtr<ID3D11Resource> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;

    void AddPlane(float planeSize);

    void AddMesh(float scaleRate, std::string objectFileName);
    void ProcessNode(aiNode* node, const aiScene* scene, float scaleRate);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, float scaleRate);
};

