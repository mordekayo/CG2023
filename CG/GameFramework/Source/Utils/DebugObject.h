#pragma once
#include "GameObjects/GameObject.h"

class DebugObject : public FGameObject
{
public:

    DebugObject();
    void CreateGrid(int GridSize, float CellSize, DirectX::SimpleMath::Color color);
};