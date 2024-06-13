#pragma once

#include "BaseSprite.h"
#include "AnimationComponent.h"

class Motion;
class ScalingComponent;

class AnimatableSprite : public BaseSprite {
public:
    AnimationComponent animationComponent;
    float healthBar = 100;
    float maxHealth = 100;

    // Constructor
    AnimatableSprite(Motion& spriteDimensions, ScalingComponent& spriteScales);

    void draw() override;
    void update() override;
    void rotate() override;
    void fireProjectile() override;
};


