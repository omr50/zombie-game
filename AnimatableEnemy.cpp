#include "AnimatableEnemy.h"
#include <stdio.h>
/*
This class follows the same design as an AnimatableSprite
except it holds a pointer to the main character which it is supposed
to follow.
*/

// NOT SURE IF NEED TO CREATE IT AS SPRITE
AnimatableEnemy::AnimatableEnemy(Motion spriteDimensions, ScalingComponent spriteScales, AnimatableSprite* target) : AnimatableSprite(spriteDimensions, spriteScales) {
	this->target = target;
}

void AnimatableEnemy::draw() {
	animationComponent.draw(this);
}

// override to calculate based on the soldiers location.
void AnimatableEnemy::rotate() {
	this->motions.calculateRotationAngle(this->motions.position, this->target->motions.position);
}

bool AnimatableEnemy::attackTimeElapsed(double seconds) {
	auto currentTime = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - this->lastAttack);
	bool isGreater = duration.count() > seconds;
	if (isGreater)
		this->lastAttack = currentTime;
	return isGreater;
}
