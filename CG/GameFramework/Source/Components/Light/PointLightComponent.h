#pragma once
#include "Components/ObjectComponent.h"

class FPointLightComponent : public FObjectComponent
{
public:

    FPointLightComponent(float InConstant, float InLinear, float InQuadratic);

    DirectX::SimpleMath::Vector3 Color = { DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f) };

    float Constant  = 1.0f;
    float Linear    = 0.09f;
    float Quadratic = 0.032f;
    
};
