#pragma once
#include "AnimatableSprite.h"
#include <chrono>
class AnimatableEnemy : public AnimatableSprite {
public:
    AnimatableSprite* target;
    std::chrono::steady_clock::time_point lastAttack = std::chrono::steady_clock::now();

    AnimatableEnemy(Motion spriteDimensions, ScalingComponent spriteScales, AnimatableSprite* target);

    bool attackTimeElapsed(double seconds);

    void draw() override;

    void rotate() override;
};

