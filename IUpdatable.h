#pragma once

class IUpdatable {
public:
    virtual ~IUpdatable() = default; // Virtual destructor for proper cleanup of derived classes
    virtual void update() = 0;
};


