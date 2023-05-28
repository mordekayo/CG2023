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

    FRenderComponent(FMeshComponent* MeshComponent);
    FRenderComponent() = delete;

    virtual void Initialize() override;

    void DrawOpaque();

    FMeshComponent* MeshComponent;

    ID3D11Buffer** ConstBuffer;
};
