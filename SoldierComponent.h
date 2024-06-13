#pragma once
#include "Motion.h"
#include "ScalingComponent.h"
#include "AnimatableSprite.h"
#include "Projectile.h"
#include <vector>
#include <chrono>
#include "Turret.h"


class Soldier : public AnimatableSprite {
public:
    int gun;
    std::chrono::steady_clock::time_point timeStamp;
    std::vector<Projectile> bullets;
    std::vector<Turret> turrets;
    Vector2 gunOffset = { 0.0, 25.0 };
    Camera2D camera = { 0 };
    int rifleAmmo = 200;
    int pistolAmmo = 100;
    int sniperAmmo = 100;
    int numTurrets = 1;

    Soldier(Motion spriteDimensions, ScalingComponent spriteScales, int Gun = 1);

    void draw() override;
    void update() override;
    void rotate() override;
    void fireProjectile() override;
    void updateProjectiles();
    bool bulletTimeElapsed(double seconds);
    void addTurret();
};

