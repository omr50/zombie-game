#pragma once
// no circular dependencies
class IDrawable {
public:
    virtual ~IDrawable() = default;  // Virtual destructor for proper cleanup
    virtual void draw() = 0;
};
