#pragma once
#include <set>
#include "../Utils/SimpleMath.h"

class FObjectComponent;

class FGameObject
{
	public:

	FGameObject() = default;
	
	void Init() const;
	void Update() const;
	void Draw() const;

	DirectX::SimpleMath::Vector4 GetTransform() const;
	
	void AddComponent(FObjectComponent* ComponentToAdd);
	void DeleteComponent(FObjectComponent* ComponentToDelete);

	void AddTransform(DirectX::SimpleMath::Vector4 Transform);
	void SetPosition(DirectX::SimpleMath::Vector4 NewPosition);

protected:

	DirectX::SimpleMath::Vector4 Position = {0.0f, 0.0f, 0.0f, 1.0f};
	std::set<FObjectComponent*> Components; 
	
};

