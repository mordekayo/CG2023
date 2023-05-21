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
	FTransformComponent* TransformComponent;
	FRenderComponent* RenderComponent;
	FMeshComponent* MeshComponent;
	FRenderShadowsComponent* RenderShadowsComponent;
	
	std::vector<FObjectComponent*> components;

	FGameObject(FGameObject* parent = nullptr);
	~FGameObject();

	virtual void Initialize();
	virtual void Update(float deltaTime);

	void AddComponent(FObjectComponent* component);

	void CreatePlane(float planeSize, std::string textureFileName);
	void CreateMesh(float scaleRate, std::string textureFileName, std::string objectFileName);
};