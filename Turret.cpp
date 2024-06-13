#include "Turret.h"

Turret::Turret(Motion motion, ScalingComponent scales) : AnimatableSprite(motion, scales) {
}

bool Turret::attackTimeElapsed(double seconds) {
	auto currentTime = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - this->lastAttack);
	bool isGreater = duration.count() > seconds;
	
	if (isGreater)
		this->lastAttack = currentTime;
	return isGreater;
}

void Turret::drawLineTo(Vector2 position) {
	Vector2 scaledPosition = this->scales.getScaledPosition(position);
	this->animationComponent.drawLine(this->scales.getScaledPosition(this->motions.position), scaledPosition, WHITE);

}

void Turret::draw() {
	this->animationComponent.draw(this);
}
