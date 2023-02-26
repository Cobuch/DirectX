#pragma once
#include "SquareComponent.h"

class Starter;

class BallComponent :
    public SquareComponent
{
private:
    Starter* pGame;
    void GenRndDirection();
public:
    float Speed;
    DirectX::SimpleMath::Vector2 Direction;
    BallComponent(Game* g);
    void Update() override;
};