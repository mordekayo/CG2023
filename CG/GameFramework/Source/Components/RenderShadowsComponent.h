#pragma once
#include "ObjectComponent.h"
#include "MeshComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX::SimpleMath;

class FRenderShadowsComponent : public FObjectComponent
{
public:

    FRenderShadowsComponent(FMeshComponent* MeshComponent);
    FRenderShadowsComponent() = delete;

    FMeshComponent* MeshComponent;

    Microsoft::WRL::ComPtr<ID3D11Buffer> sConstBuffer;

    virtual void Initialize() override;

    void Draw();
};

