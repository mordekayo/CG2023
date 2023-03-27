#pragma once
#include "GameObjects/GameObject.h"

class StickyObject : public FGameObject
{
public:

    virtual void Construct() override;

    void SetScale(float NewScale);
    float GetScale();
    
private:

    float Scale = 0.0f;
};
