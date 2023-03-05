#include "Camera.h"

#include "../DisplayWin32.h"
#include "../Game.h"
#include <iostream>

FCamera::FCamera()
{

}

void FCamera::Update(float DeltaTime)
{
    //std::cout << "Pos x: " << Position.x << " Pos y: " << Position.y << " Pos z: " << Position.z << std::endl;
    //std::cout << "Tar x: " << Target.x << " Pos y: " << Target.y << " Tar z: " << Target.z << std::endl;
    //std::cout << "Up x: " << UpVector.x << " Up y: " << UpVector.y << " Up z: " << UpVector.z << std::endl;
    //
    ViewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(Position, { 0.5f, -0.3f, 1.0f }, UpVector);

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
            static_cast<float>(FGame::Instance()->GetDisplay().GetScreenWidth()),
            static_cast<float>(FGame::FGame::Instance()->GetDisplay().GetScreenHeight()),
            0.1,
            1000);
    }
}

DirectX::SimpleMath::Matrix FCamera::GetViewProjectionMatrix(DirectX::SimpleMath::Matrix WorldView) const
{
    return WorldView * ViewMatrix * ProjectionMatrix;
}
