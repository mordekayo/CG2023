#pragma once
#include "ObjectComponent.h"
#include "Utils/SimpleMath.h"

class FCameraComponent : public FObjectComponent
{
public:

    FCameraComponent();

    virtual void Initialize() override;

    DirectX::SimpleMath::Matrix GetProjection();
    float fovAngle = DirectX::XM_PIDIV2;
    float aspect;
    float nearZ;
    float farZ;
};