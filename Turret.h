#pragma once
#include "AnimatableSprite.h"
#include <chrono>

class Turret: public AnimatableSprite {
public:
    std::chrono::steady_clock::time_point lastAttack = std::chrono::steady_clock::now();

    Turret(Motion spriteDimensions, ScalingComponent spriteScales);

    bool attackTimeElapsed(double seconds);

    void draw() override;

    void drawLineTo(Vector2 position);

};

