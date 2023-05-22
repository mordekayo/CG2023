#pragma once
#include <vector>

#include "Sources.h"

class FTransformComponent;
class FRenderComponent;
class FMeshComponent;
class FRenderShadowsComponent;
class FObjectComponent;

class FGameObject
{
public:

	//COMPONENTS
	FTransformComponent* TransformComponent = nullptr;
	FRenderComponent* RenderComponent = nullptr;
	FMeshComponent* MeshComponent = nullptr;
	FRenderShadowsComponent* RenderShadowsComponent = nullptr;
	
	std::vector<FObjectComponent*> Components;

	FGameObject(FGameObject* parent = nullptr);
	virtual ~FGameObject();

	virtual void Initialize();
	virtual void Update(float DeltaTime);

	void AddComponent(FObjectComponent* Component);

	void CreatePlane(float PlaneSize, const std::string& TextureFileName);
	void CreateMesh(float ScaleRate, const std::string& TextureFileName, const std::string& ObjectFileName);
};