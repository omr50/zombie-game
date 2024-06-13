#pragma once

class IProjectileEmitting {
public:
	virtual ~IProjectileEmitting() = default;  // Virtual destructor for proper cleanup
	virtual void fireProjectile() = 0;
};
