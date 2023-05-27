#include "PointLightComponent.h"

FPointLightComponent::FPointLightComponent(float InConstant, float InLinear, float InQuadratic)
{
    Constant  = InConstant;
    Linear = InLinear;
    Quadratic = InQuadratic;
}
