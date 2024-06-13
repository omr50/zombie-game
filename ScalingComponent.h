#pragma once

// this class CAUSES NO CIRCULAR DEPENDENCIES 

#include "raylib.h"

class ScalingComponent {
public:
    float individualScale;
    float* globalScale;

    ScalingComponent(float individualScale, float* globalScale);

    Vector2 getCombinedScale() const;
    Vector2 getScaledPosition(Vector2 position);
    float getTotalScalingFactor();
    Rectangle buildDestRect(Vector2 position, Rectangle currFrame);
};

