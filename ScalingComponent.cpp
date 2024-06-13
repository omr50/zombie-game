/*
holds the static variable for the global screen
scale and then holds the individual scale for
the sprite it belongs to. The static variable
is initialized in main, and changed on every
frame when the screen size changes.
*/
#include "ScalingComponent.h"
#include <cmath>

ScalingComponent::ScalingComponent(float individualScale, float* globalScale) { this->individualScale = individualScale; this->globalScale = globalScale; };

Vector2 ScalingComponent::getCombinedScale() const {
    return { individualScale * (*globalScale), individualScale * (*globalScale) };
}

Vector2 ScalingComponent::getScaledPosition(Vector2 position) {
    return { position.x * (*globalScale), position.y * (*globalScale) };
}

float ScalingComponent::getTotalScalingFactor() {
    return individualScale * (*globalScale);
}

// builds a destination rectangle which is scaled by the scaling factors
// position is scaled by global screen scale, size is scaled by both
// screen scale * individual scale
Rectangle ScalingComponent::buildDestRect(Vector2 position, Rectangle currFrame) {
    Vector2 scaledPosition = getScaledPosition(position);
    float totalScalingFactor = getTotalScalingFactor();
    return { scaledPosition.x, scaledPosition.y, abs(currFrame.width) * totalScalingFactor, currFrame.height * totalScalingFactor };
}

