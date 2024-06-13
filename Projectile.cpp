#include "Projectile.h"
#include "raylib.h"
// calling the constructor should give back a valid bullet 
// with proper velocity (determined from soldier direction)

Projectile::Projectile(Motion motion, ScalingComponent scaling, Camera2D* camera, int gunDamage) : AnimatableSprite(motion, scaling) {
	Rectangle currRect = { this->motions.position.x, this->motions.position.y, this->motions.width, this->motions.height };
	this->camera = camera;
	Vector2 scaledPosition = this->scales.getScaledPosition(this->motions.position);
	Vector2 updatedmousePos = GetScreenToWorld2D(GetMousePosition(), *this->camera);
	this->velocity = this->motions.subtract(updatedmousePos, scaledPosition);
	this->velocity = this->motions.scale(this->motions.normalize(this->velocity), this->motions.speed);
	this->active = true;
	this->gunDamage = gunDamage;
}
void Projectile::draw() {
	if (this->active)
		animationComponent.drawCircle(this);
}

// override to compute rotate based upon the mouse position
void Projectile::rotate() {
	this->motions.calculateRotationAngle(this->scales.getScaledPosition(this->motions.position), GetMousePosition());
}

void Projectile::update() {
	if (this->active) {
		this->motions.position.x += this->velocity.x;
		this->motions.position.y += this->velocity.y;

		// Check if bullet goes off screen
		if (this->motions.position.x < 0 || this->motions.position.x > 4000 ||
			this->motions.position.y < 0 || this->motions.position.y > 3181) {
			this->active = false;
		}
	}

}