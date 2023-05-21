#include "TransformComponent.h"

FTransformComponent::FTransformComponent()
{
	localPosition = DirectX::SimpleMath::Vector3::Zero;
	localRotation = DirectX::SimpleMath::Quaternion::Identity;
	scale = DirectX::SimpleMath::Vector3(1, 1, 1);
	parent = nullptr;
}

FTransformComponent::FTransformComponent(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation)
{
	localPosition = position;
	localRotation = rotation;
	parent = nullptr;
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetPosition() const
{
	if (parent)
	{
		return DirectX::SimpleMath::Vector3::Transform(localPosition, parent->GetModel());
	}
	else
	{
		return localPosition;
	}
}

DirectX::SimpleMath::Quaternion FTransformComponent::GetRotation() const
{
	if (parent)
	{
		return parent->GetRotation() * localRotation;
	}
	else
	{
		return localRotation;
	}
}

void FTransformComponent::SetPosition(const DirectX::SimpleMath::Vector3& position)
{
	if (parent)
	{
		localPosition = DirectX::SimpleMath::Vector3::Transform(position, parent->GetView());
	}
	else
	{
		localPosition = position;
	}
}

void FTransformComponent::SetRotation(const DirectX::SimpleMath::Quaternion& rotation)
{
	if (parent)
	{
		DirectX::SimpleMath::Quaternion quaternion;
		parent->GetRotation().Inverse(quaternion);
		localRotation = rotation * quaternion;
	}
	else
	{
		localRotation = rotation;
	}
}

DirectX::SimpleMath::Matrix FTransformComponent::GetView() const
{
	if (parent)
	{
		return parent->GetView() * GetLocalView();
	}
	else
	{
		return GetLocalView();
	}
}

DirectX::SimpleMath::Matrix FTransformComponent::GetLocalView() const
{
	return DirectX::SimpleMath::Matrix::CreateTranslation( - localPosition) *
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(localRotation).Transpose() *
			DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(1 / scale.x, 1 / scale.y, 1 / scale.z));
}

DirectX::SimpleMath::Matrix FTransformComponent::GetModel() const
{
	if (parent)
	{
		return GetLocalModel() * parent->GetModel();
	}
	else
	{
		return GetLocalModel();
	}
}

DirectX::SimpleMath::Matrix FTransformComponent::GetLocalModel() const
{
	return DirectX::SimpleMath::Matrix::CreateScale(scale) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(localRotation) * DirectX::SimpleMath::Matrix::CreateTranslation(localPosition);
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetLocalLeft() const
{
	return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Right, GetLocalModel());
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetLocalUp() const
{
	return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Up, GetLocalModel());
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetLocalForward() const
{
	return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Forward, GetLocalModel());
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetLeft() const
{
	return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Right, GetModel());
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetUp() const
{
	return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Up, GetModel());
}

DirectX::SimpleMath::Vector3 FTransformComponent::GetForward() const
{
	return DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Forward, GetModel());
}
