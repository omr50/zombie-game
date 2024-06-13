#include "SoldierComponent.h"


Soldier::Soldier(Motion spriteDimensions, ScalingComponent spriteScales, int Gun) : AnimatableSprite(spriteDimensions, spriteScales) {
	this->gun = Gun;
	this->timeStamp = std::chrono::steady_clock::now();
	camera.target = this->motions.position;
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	this->animationComponent.addAnimationState(IDLE_TURRET, "resources/turret/shoot/merged.png", 1, 1, 201, 196)
		->addAnimationState(SHOOTING_TURRET, "resources/turret/shoot/merged.png", 17, 17, 201, 196);

}

void Soldier::draw() {
	MODE currMode = this->mode;
	// change mode to feet to feet to draw them
	(this->mode == IDLE) ? this->mode = IDLE_FEET : this->mode = MOVING_FEET;
	animationComponent.draw(this);
	// revert back mode to current mode
	this->mode = currMode;

	this->mode = (MODE)((int)this->mode + this->gun);
	animationComponent.draw(this);
	this->mode = (MODE)((int)this->mode - this->gun);

	// draw each projectile in the list
	for (auto projectile : this->bullets) {
		projectile.draw();
	}

	for (auto turret : turrets) {
		turret.draw();
	}

}

void Soldier::update() {
	this->updateProjectiles();
}

void Soldier::updateProjectiles() {
	for (int i = 0; i < this->bullets.size(); i++) {
		this->bullets[i].update();
	}

	this->bullets.erase(
		std::remove_if(this->bullets.begin(), this->bullets.end(), [](const Projectile& e) {
			return !e.active; // This lambda returns true for elements to be removed
			}),
		this->bullets.end());

	
}
// override to compute rotate based upon the mouse position
void Soldier::rotate() {
	Vector2 updatedmousePos = GetScreenToWorld2D(GetMousePosition(), camera);
	this->motions.calculateRotationAngle(this->scales.getScaledPosition(this->motions.position), updatedmousePos);
}

void Soldier::fireProjectile() {

	// check which gun to determine amt of bullets possible

	//pistol
	if (this->gun == 1) {
		auto currTime = std::chrono::steady_clock::now();
		if (!this->bulletTimeElapsed(0.30)) {
			return;
		}
	}
	// rifle
	else if (this->gun == 2 && this->rifleAmmo > 0) {
		if (!this->bulletTimeElapsed(0.08)) {
			return;
		}
		printf("Rifle Ammo %d\n", this->rifleAmmo);
		this->rifleAmmo--;
	}
	// sniper
	else if (this->gun == 3 && this->sniperAmmo > 0) {
		if (!this->bulletTimeElapsed(0.30)) {
			return;
		}
		this->sniperAmmo--;
	}
	else return;
	
	// create new bullet
	ScalingComponent bulletScaling = ScalingComponent(1.0, this->scales.globalScale);
	Vector2 bulletStartPosition;
	// this->motions.calculateRotationAngle(this->scales.getScaledPosition(this->motions.position), mousePosition);
	float radiansAngle = this->motions.rotationAngle * PI / 180.0;
	bulletStartPosition.x = this->motions.position.x + gunOffset.x * cos(radiansAngle) - gunOffset.y * sin(radiansAngle);
	bulletStartPosition.y = this->motions.position.y + gunOffset.x * sin(radiansAngle) + gunOffset.y * cos(radiansAngle);
	Motion bulletMotion = Motion(bulletStartPosition, 4, 4, 1, 0, 50, false);
	// gun does 50 dmg if pistol or rifle, 100 if sniper
	Projectile newBullet = Projectile(bulletMotion, bulletScaling, &camera, (this->gun == 1 || this->gun == 2) ? 50 : 100);
		
	// add bullet to the vector
	this->bullets.push_back(newBullet);
	
}

bool Soldier::bulletTimeElapsed(double seconds) {
	auto currentTime = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - this->timeStamp);
	bool isGreater = duration.count() > seconds;
	if (isGreater)
		this->timeStamp = currentTime;
	return isGreater;
}

void Soldier::addTurret() {
	if (this->numTurrets <= 0)
		return;
	Motion turretMotion = Motion(this->motions.position, 201, 196, 1, 180, 1, true);
	ScalingComponent turretScale = ScalingComponent(1.0, this->scales.globalScale);
	Turret turret = Turret(turretMotion, turretScale);
	turret.mode = IDLE_TURRET;

	this->turrets.push_back(turret);
	printf("Added turret: %d turrets remaining\n", this->numTurrets);
	this->numTurrets--;

}