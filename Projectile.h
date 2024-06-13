#pragma once
#include "AnimatableSprite.h"

class Projectile : public AnimatableSprite {
public:
    Projectile(Motion spriteDimensions, ScalingComponent spriteScales, Camera2D* camera, int gunDamage);

    Vector2 velocity;
    bool active = true;
    Camera2D* camera;
    int gunDamage;

    void draw() override;
    void update() override;
    void rotate() override;
};
