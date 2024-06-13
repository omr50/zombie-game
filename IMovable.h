#pragma once
#include "MODE.h"
// This class Cuases no CIRCULAR DEPENDENCIES
// 
// This interface allows a sprite to implement it,
// meaning that class will have position and movement
// speed, and should be able to implement common functions
// like move left, right, jump, etc. The keyboard manager
// will take in a pointer to IMovable and then move the object.
class IMovable {
public:
    virtual ~IMovable() = default; // Virtual destructor for proper cleanup of derived classes

    virtual void moveLeft(MODE& mode) = 0;
    virtual void moveRight(MODE& mode) = 0;
    virtual void moveUp(MODE& mode) = 0;
    virtual void moveDown(MODE& mode) = 0;
};

