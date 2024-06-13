#pragma once
#include "Motion.h"
#include "IMovable.h"
#include "raylib.h"

// No circular dependencies

class Motion : public IMovable {
public:
    Vector2 position;
    int width;
    int height;
    int direction;
    float speed;
    bool isJumping = false;
    bool rotatable = false;
    float rotationAngle = 0.0f;
    float angleOffset = 0;

    Motion(Vector2 position, int width, int height, int direction, float angleOffset = 0, float speed = 1, bool rotatable = false);

    void calculateRotationAngle(Vector2 scaledCoordinates, Vector2 goalCoordinates);

    void moveLeft(MODE& mode) override;
    void moveRight(MODE& mode) override;
    void moveUp(MODE& mode) override;
    void moveDown(MODE& mode) override;

    Vector2 subtract(Vector2 v1, Vector2 v2);
    float magnitude(Vector2 v);
    Vector2 normalize(Vector2 v);
    Vector2 scale(Vector2 v, float scaleFactor);
    void moveEnemy(Vector2 position);
    void moveEnemyKnockback(Vector2 position);
    void reverseEnemyMove(Vector2 position);
    bool checkPointRectCollision(Vector2 point);
    bool checkRectRectCollision(const Motion& motion);
    bool checkSmallRectRectCollision(const Motion& other, float factor);
    Rectangle getOriginalRect();
};


