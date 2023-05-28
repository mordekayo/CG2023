#pragma once
#include "Components/ObjectComponent.h"
#include "Components/RenderComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class FPointLightComponent : public FObjectComponent
{
public:

    FPointLightComponent(float InConstant, float InLinear, float InQuadratic);

    virtual void Initialize() override;

    void Draw();

    ID3D11Buffer** ConstBuffer;

    float SphereRadius;
    
    DirectX::SimpleMath::Vector3 Color = { DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f) };

    float Constant  = 1.0f;
    float Linear    = 0.09f;
    float Quadratic = 0.032f;

    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    std::vector<DirectX::SimpleMath::Vector4> Points;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    std::vector<int> Indices;

    void PoiAddMesh(float SphereRadius, const std::string& ObjectFileName);
    void ProcessNode(aiNode* Node, const aiScene* Scene, float SphereRadius);
    void ProcessMesh(aiMesh* Mesh, const aiScene* Scene, float SphereRadius);
};
