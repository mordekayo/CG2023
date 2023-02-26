#pragma once
#include <set>

class FObjectComponent;

class FGameObject
{
	public:

	FGameObject() = default;
	
	void Init() const;
	void Update() const;
	void Draw() const;

	void AddComponent(FObjectComponent* ComponentToAdd);
	void DeleteComponent(FObjectComponent* ComponentToDelete);
private:
	
	std::set<FObjectComponent*> Components; 
};

