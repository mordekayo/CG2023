#include "DebugObject.h"
#include "Components/RenderComponent.h"

DebugObject::DebugObject() {}

void DebugObject::CreateGrid(int GridSize, float CellSize, DirectX::SimpleMath::Color color)
{
    FRenderComponent* RenderComponent = new FRenderComponent(L"../GameFramework/Source/Shaders/MyTexturedShader.hlsl",
       L"../GameFramework/Source/Textures/Wood.png");
    RenderComponent->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    AddComponent(RenderComponent);
    RenderComponent->AddGrid(GridSize, CellSize, color);
}