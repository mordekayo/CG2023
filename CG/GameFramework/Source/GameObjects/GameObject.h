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

	virtual DirectX::SimpleMath::Vector3 GetLocalTranslation() const;
	virtual DirectX::SimpleMath::Vector3 GetWorldTranslation() const;
	
	void AddComponent(FObjectComponent* ComponentToAdd);
	void DeleteComponent(FObjectComponent* ComponentToDelete);

	void AddTranslation(DirectX::SimpleMath::Vector3 Translation);
	void SetTranslation(DirectX::SimpleMath::Vector3 NewTranslation);

	DirectX::SimpleMath::Quaternion GetLocalRotationQuat() const;
	DirectX::SimpleMath::Vector3 GetLocalRotationEuler() const;
	
	DirectX::SimpleMath::Quaternion GetWorldRotationQuat() const;
	DirectX::SimpleMath::Vector3 GetWorldRotationEuler() const;

	void AddRotationQuat(DirectX::SimpleMath::Quaternion AdditionalRotation);
	
	void SetLocalRotationQuat(DirectX::SimpleMath::Quaternion NewRotation);
	void SetLocalRotationEuler(DirectX::SimpleMath::Vector3 NewRotation);

	DirectX::SimpleMath::Vector3 GetForwardVector() const;
	DirectX::SimpleMath::Vector3 GetRightVector() const;
	DirectX::SimpleMath::Vector3 GetLeftVector() const;
	DirectX::SimpleMath::Vector3 GetBackwardVector() const;

	virtual void SetParent(FGameObject* NewParent);

	DirectX::SimpleMath::Matrix GetWorldTransform() const;
	virtual DirectX::SimpleMath::Matrix GetLocalTransform() const;
protected:

	FGameObject* ParentObject = nullptr;
	
	DirectX::SimpleMath::Vector3 Translation = {0.0f, 0.0f, 0.0f};
	DirectX::SimpleMath::Quaternion Rotation = DirectX::SimpleMath::Quaternion::Identity;
	
	std::set<FObjectComponent*> Components = {};
	
};

