#include "Camera.h"

#include "../DisplayWin32.h"
#include "../Game.h"
#include <iostream>

FCamera::FCamera()
{

}

void FCamera::Update(float DeltaTime)
{
    std::cout << "Pos x: " << Transform.x << " Pos y: " << Transform.y << " Pos z: " << Transform.z << std::endl;
    std::cout << "Tar x: " << Target.x << " Pos y: " << Target.y << " Tar z: " << Target.z << std::endl;
    std::cout << "Up x: " << UpVector.x << " Up y: " << UpVector.y << " Up z: " << UpVector.z << std::endl;
    
    ViewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(Transform, Target, UpVector);

    if (IsPerspective)
    {
        ProjectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
            FieldOfView,
            static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth()) /
            static_cast<float>(FGame::FGame::Instance()->GetDisplay().GetScreenHeight()),
            0.1f,
            1000.0f);
    }
    else
    {
        ProjectionMatrix = DirectX::SimpleMath::Matrix::CreateOrthographic(
            static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth()) / 475,
            static_cast<float>(FGame::FGame::Instance()->GetDisplay().GetScreenHeight()) / 475,
            0.1,
            1000);
    }
}

DirectX::SimpleMath::Matrix FCamera::GetViewProjectionMatrix(DirectX::SimpleMath::Matrix WorldView) const
{
    return WorldView * ViewMatrix * ProjectionMatrix;
}

void FCamera::TogglePerspective()
{
    IsPerspective = !IsPerspective;
}

void FCamera::AddTransform(DirectX::SimpleMath::Vector3 NewTransform)
{
    Transform += NewTransform;
}

void FCamera::SetTransform(DirectX::SimpleMath::Vector3 NewTransform)
{
    Transform = NewTransform;
}

DirectX::SimpleMath::Vector3 FCamera::GetTransform()
{
    return Transform;
}

void FCamera::SetTarget(DirectX::SimpleMath::Vector3 NewTarget)
{
    Target = NewTarget;
}
