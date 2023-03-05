#pragma once

#include "../Utils/SimpleMath.h"

class FCamera
{
public:
    FCamera();

    void Update(float DeltaTime);

    DirectX::SimpleMath::Matrix GetViewProjectionMatrix(DirectX::SimpleMath::Matrix WorldView) const;

    void TogglePerspective();

private:

    bool IsPerspective = true;
    
    float FieldOfView = 90.0f;
    
    DirectX::SimpleMath::Matrix ViewMatrix;
    DirectX::SimpleMath::Matrix ProjectionMatrix;
    
    DirectX::SimpleMath::Vector3 Position = {0.0f, 0.2f, -0.5f};
    DirectX::SimpleMath::Vector3 UpVector = {0.0f, 1.0f, 0.0f};
    DirectX::SimpleMath::Vector3 Target = {0.0f, 0.0f, 0.0f};
    
};
