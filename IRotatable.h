#pragma once
// no circular dependencies
class IRotatable {
public:
    virtual ~IRotatable() = default;  // Virtual destructor for proper cleanup
    virtual void rotate() = 0;
};
