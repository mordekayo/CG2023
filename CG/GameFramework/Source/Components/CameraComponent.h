#pragma once
#include "ObjectComponent.h"
#include "Utils/SimpleMath.h"

class FCameraComponent : public FObjectComponent
{
public:

    FCameraComponent();

    virtual void Initialize() override;

    DirectX::SimpleMath::Matrix GetProjection();
    float FovAngle = DirectX::XM_PIDIV2;
    float Aspect;
    float NearZ;
    float FarZ;
};