#include "ZombieManager.h"
#include <cstdlib>
#include <ctime>

ZombieManager::ZombieManager(Soldier* target) {
	this->waveNumber = 1;
	this->target = target;
	AnimationComponent animationComponent;
	animationComponent
		.addAnimationState(ZOMBIE_ATTACK, "resources/zombie/attack/merged.png", 20, 20, 256, 256)
		->addAnimationState(ZOMBIE_RUN, "resources/zombie/walk/merged.png", 32, 32, 256, 256);

	srand(time(NULL));
}

void ZombieManager::createZombie() {
	// give random directions


	// 15% chance of a zombie that goes twice as fast
	float prob = rand() % 101;
	Motion* zombieDimensions;
	// loop while there is a collision, otherwise continue
	while (true) {
		float xPos = rand() % 4000;  // rand() % (max + 1)
		float yPos = rand() % 3181;  // rand() % (max + 1)
		int collision = false;
		zombieDimensions = new Motion({ xPos, yPos }, 256, 256, 1.5, 180, (prob < 15) ? (this->velocity * 2) : this->velocity, true);
		for (int i = 0; i < this->zombies.size(); i++) {
			if (zombieDimensions->checkSmallRectRectCollision(this->zombies[i].motions, 16) || zombieDimensions->checkSmallRectRectCollision(this->target->motions, 0.5)) {
				collision = true;
				break;
			}
		}
		if (!collision)
			break;
		
	}
	ScalingComponent zombieScale = ScalingComponent(1, target->scales.globalScale);
	AnimatableEnemy zombie = AnimatableEnemy(*zombieDimensions, zombieScale, target);
	zombie.maxHealth = this->maxHealth;
    zombie.mode = ZOMBIE_RUN;
	zombies.push_back(zombie);
}

void ZombieManager::createNZombies(int N) {
	if (this->maxHealth == 0) {
		this->maxHealth = 100;
		this->velocity = 6;
	}
	else {
		this->maxHealth += (waveNumber % 2 == 0) ? 50 : 0;
		this->velocity += (waveNumber % 2) ? 2 : 0;
		this->waveNumber++;
		if (waveNumber % 5 == 0) {
			target->numTurrets++;
		}
		// reset ammo on every 3 rounds 
		if (this->waveNumber % 3 == 1) {
			this->target->rifleAmmo = 200;
			this->target->sniperAmmo = 100;
		}
	}
	for (int i = 0; i < N; i++) {
		this->createZombie();
	}
}

// CHANGE THIS LATER
void ZombieManager::update() {
	// *************************** preferrably loop over each zombie and do zombie.update() (BOTH MANAGER AND ZOMBIE IMPLEEMNT IUPDATABLE
	for (int i = 0; i < this->zombies.size(); i++) { 
		// on collision with soldier, start attacking
		if (this->zombies[i].motions.checkSmallRectRectCollision(this->target->motions, 8)) {
			if (this->zombies[i].attackTimeElapsed(1))
				this->target->healthBar -= 5;

			this->zombies[i].mode = ZOMBIE_ATTACK;
		}
		else {
			this->zombies[i].mode = ZOMBIE_RUN;
		}
		this->zombies[i].rotate();
	}
}

// ALSO CHANGE THIS TO PROPERLY DRAW ZOMBIES
void ZombieManager::draw() {
	std::vector<int> zombiesToRemove;

	for (int i = 0; i < this->zombies.size(); i++) {
		if (this->zombies[i].healthBar > 0) {
			bool collision = false;
			this->zombies[i].motions.moveEnemy(target->motions.position);
			for (int j = 0; j < this->zombies.size(); j++) {
				if (i != j) {
					if (this->zombies[i].motions.checkSmallRectRectCollision(this->zombies[j].motions, 16)) {
						collision = true;
						break;
					}
				}
			}
			// if collision then do move the zombie back then draw either way
			if (collision) {
				this->zombies[i].motions.position.x += (rand() % 10 - rand() % 10);
				this->zombies[i].motions.position.y += (rand() % 10 - rand() % 10);
			}
			this->zombies[i].draw();
		}
		else {
			zombiesToRemove.push_back(i);
		}
	}

	// Remove zombies with healthbar 0
	for (int i = zombiesToRemove.size() - 1; i >= 0; i--) {
		int indexToRemove = zombiesToRemove[i];
		this->zombies.erase(this->zombies.begin() + indexToRemove);
	}
	
	//for (int i = 0; i < this->zombies.size(); i++) {
	//	this->zombies[i].draw();
	//}
}

// Helper function to calculate distance between two points
float Vector2Distance(Vector2 v1, Vector2 v2) {
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return sqrt(dx * dx + dy * dy);
}

void ZombieManager::findClosestZombiesToDamage(std::vector<Projectile>& bullets) {
	for (int j = 0; j < bullets.size(); j++) {
		if (!bullets[j].active)
			continue;

		float minDistance = std::numeric_limits<float>::max();
		int closestZombieIndex = -1;

		for (int i = 0; i < this->zombies.size(); i++) {
			if (zombies[i].motions.checkPointRectCollision(bullets[j].motions.position)) {
				float distance = Vector2Distance(zombies[i].motions.position, bullets[j].motions.position);
				if (distance < minDistance) {
					minDistance = distance;
					closestZombieIndex = i;
				}
			}
		}

		if (closestZombieIndex != -1) {
			// A collision was found, and closestZombieIndex is the index of the closest zombie
			bullets[j].active = false;
			zombies[closestZombieIndex].motions.moveEnemyKnockback(this->target->motions.position);
			zombies[closestZombieIndex].color.g = 0;
			zombies[closestZombieIndex].color.b = 0;
			zombies[closestZombieIndex].color.a = 255 - (unsigned char) (80 * (zombies[closestZombieIndex].healthBar / zombies[closestZombieIndex].maxHealth));
			zombies[closestZombieIndex].healthBar -= bullets[j].gunDamage;

		}
	}
}

void ZombieManager::findClosestZombiesToTowerDamage() {
	for (int j = 0; j < target->turrets.size(); j++) {
		float minDistance = std::numeric_limits<float>::max();
		int closestZombieIndex = -1;

		for (int i = 0; i < this->zombies.size(); i++) {
			float distance = Vector2Distance(zombies[i].motions.position, target->turrets[j].motions.position);

			if (distance < minDistance && distance < 300) {
				minDistance = distance;
				closestZombieIndex = i;
			}
		}

		if (closestZombieIndex != -1 && target->turrets[j].attackTimeElapsed(0.5)) {
			zombies[closestZombieIndex].motions.moveEnemyKnockback(this->target->motions.position);
			zombies[closestZombieIndex].color.g = 0;
			zombies[closestZombieIndex].color.b = 0;
			zombies[closestZombieIndex].color.a = 255 - (unsigned char)(80 * (zombies[closestZombieIndex].healthBar / zombies[closestZombieIndex].maxHealth));
			// tower does half its hp
			zombies[closestZombieIndex].healthBar -= 50;
			// tell tower to draw line from itself to this position and update as well
			target->turrets[j].motions.calculateRotationAngle(target->turrets[j].motions.position, zombies[closestZombieIndex].motions.position);
			target->turrets[j].mode = SHOOTING_TURRET;
			target->turrets[j].drawLineTo(zombies[closestZombieIndex].motions.position);

		}
		else
		{
			target->turrets[j].mode = IDLE_TURRET;
		}
	}
}



