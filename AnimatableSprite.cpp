#pragma once
#include "AnimatableSprite.h"
AnimatableSprite::AnimatableSprite(Motion& spriteDimensions, ScalingComponent& spriteScales)
	: BaseSprite(spriteDimensions, spriteScales) {
	// Constructor implementation, if any additional setup is needed
}

void AnimatableSprite::draw() {
	animationComponent.draw(this);
}

void AnimatableSprite::update() {
	// Update implementation
}

void AnimatableSprite::rotate() {

}

void AnimatableSprite::fireProjectile() {

}
