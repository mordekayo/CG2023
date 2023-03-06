#pragma once

#include "../GameObjects/GameObject.h"
#include "../Utils/SimpleMath.h"

class FCamera : public FGameObject
{
public:
    FCamera();

    void Update(float DeltaTime);

    DirectX::SimpleMath::Matrix GetViewProjectionMatrix(DirectX::SimpleMath::Matrix WorldView) const;

    void TogglePerspective();

    void SetParent(FGameObject* NewParent);

private:

    FGameObject* Parent = nullptr;
    bool IsPerspective = true;
    
    float FieldOfView = 90.0f;
    
    DirectX::SimpleMath::Matrix ViewMatrix;
    DirectX::SimpleMath::Matrix ProjectionMatrix;
    
    DirectX::SimpleMath::Vector3 Transform = {0.0f, 0.0f, -1.0f};
    DirectX::SimpleMath::Vector3 UpVector = {0.0f, 1.0f, 0.0f};
    DirectX::SimpleMath::Vector3 Target = {0.0f, 0.0f, 0.0f};
    
};
