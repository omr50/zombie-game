#pragma once
#include <vector>
#include "AnimatableEnemy.h"
#include "Projectile.h"
#include "SoldierComponent.h"
#include "Turret.h"

class ZombieManager : public IUpdatable {
public:
	std::vector<AnimatableEnemy> zombies;
	Soldier* target;
	float velocity = 0;
	float maxHealth = 0;
	int waveNumber = 0;
	ZombieManager(Soldier* target);


	void createZombie();
	void createNZombies(int N);
	void findClosestZombiesToDamage(std::vector<Projectile>& bullet);
	void findClosestZombiesToTowerDamage();
	void update() override;
	void draw();
};
