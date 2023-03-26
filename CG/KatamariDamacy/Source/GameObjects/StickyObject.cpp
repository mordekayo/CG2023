#include "StickyObject.h"

#include "Components/CollisionComponents/SphereCollisionComponent.h"

void StickyObject::Construct()
{
    FGameObject::Construct();

    FSphereCollisionComponent* SphereCollision = new FSphereCollisionComponent(1.0f);
    AddComponent(SphereCollision);

    
}
