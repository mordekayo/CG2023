#include "StickyObject.h"

#include "Components/CollisionComponents/SphereCollisionComponent.h"

void StickyObject::Construct()
{
    FGameObject::Construct();
}

void StickyObject::SetScale(float NewScale)
{
    Scale = NewScale;
}

float StickyObject::GetScale()
{
    return Scale;
}
