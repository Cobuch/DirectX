#pragma once
#include "SquareComponent.h"
#include "SimpleMath.h"

class PlayerComponent final :
    public SquareComponent
{
public:
    DirectX::SimpleMath::Rectangle CollisionBox;
    float Speed;
    explicit PlayerComponent(Game* g);
    void Update() override;
};