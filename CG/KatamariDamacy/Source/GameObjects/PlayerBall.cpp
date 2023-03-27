#include "PlayerBall.h"

#include "Components/RenderComponent.h"
#include "Components/CollisionComponents/SphereCollisionComponent.h"


PlayerBall::PlayerBall()
{

}

void PlayerBall::Construct()
{
    FGameObject::Construct();
}

void PlayerBall::SetScale(float NewScale)
{
    Scale = NewScale;
    //SetTransform(GetWorldTransform() * (1 + Scale));
}

float PlayerBall::GetScale() const
{
    return Scale;
}
