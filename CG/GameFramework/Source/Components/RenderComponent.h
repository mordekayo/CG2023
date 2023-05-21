#pragma once
#include "ObjectComponent.h"

#include "MeshComponent.h"

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class FMeshComponent;

class FRenderComponent : public FObjectComponent
{
public:

    FRenderComponent(FMeshComponent* modelComponent);
    FRenderComponent() = delete;

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;

    void Draw();

    FMeshComponent* modelComponent;

    ID3D11Buffer** constBuffer;
};
