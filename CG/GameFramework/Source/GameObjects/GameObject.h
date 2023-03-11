#pragma once
#include "Utils/SimpleMath.h"
#include <set>

class FObjectComponent;

class FGameObject
{
	public:
	virtual ~FGameObject() = default;

	FGameObject() = default;
	
	virtual void Init();
	virtual void Construct();
	virtual void Update(float DeltaTime);
	void Draw() const;

	virtual DirectX::SimpleMath::Vector3 GetTransform() const;

	DirectX::SimpleMath::Matrix GetWorldView() const;
	
	void AddComponent(FObjectComponent* ComponentToAdd);
	void DeleteComponent(FObjectComponent* ComponentToDelete);

	void AddTransform(DirectX::SimpleMath::Vector3 Transform);
	void SetTransform(DirectX::SimpleMath::Vector3 NewPosition);

	DirectX::SimpleMath::Quaternion GetRotationQuat() const;
	DirectX::SimpleMath::Vector3 GetRotationEuler() const;

	void SetRotationQuat(DirectX::SimpleMath::Quaternion NewRotation);
	void SetRotationEuler(DirectX::SimpleMath::Vector3 NewRotation);
	
protected:

	void UpdateWorldMatrix();
	
	DirectX::SimpleMath::Matrix WorldView;
	
	DirectX::SimpleMath::Vector3 Translation = {0.0f, 0.0f, 0.0f};
	DirectX::SimpleMath::Quaternion Rotation = DirectX::SimpleMath::Quaternion::Identity;
	std::set<FObjectComponent*> Components; 

	
};

