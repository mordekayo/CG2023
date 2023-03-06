#include "Camera.h"

#include "../DisplayWin32.h"
#include "../Game.h"

FCamera::FCamera()
{
    SetTransform({0.0f, 0.0f, -1.0f});
}

void FCamera::Update(float DeltaTime)
{
    if (Parent)
    {
        ViewMatrix = GetWorldView()* Parent->GetWorldView();
    }
    ViewMatrix = GetWorldView();

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

void FCamera::SetParent(FGameObject* NewParent)
{
    Parent = NewParent;
}
