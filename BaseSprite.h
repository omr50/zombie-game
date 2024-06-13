#pragma once
// no circular dependencies
#include "IDrawable.h"
#include "IUpdatable.h"
#include "ScalingComponent.h"
#include "Motion.h"
#include "IRotatable.h"
#include "IProjectileEmitting.h"

class BaseSprite : public IDrawable, public IUpdatable, IRotatable, IProjectileEmitting {
public:
    Motion motions;
    ScalingComponent scales;
    MODE mode;
    Color color = { 255, 255, 255, 255 };

    BaseSprite(Motion& spriteMotions, ScalingComponent spriteScales);
};


