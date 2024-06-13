

/*
Holds dimension info of the sprite
*/
#include "Motion.h"
#include <cmath>
#include <stdio.h>

Motion::Motion(Vector2 position, int width, int height, int direction, float angleOffset, float speed, bool rotatable) {
	this->position = position;
	this->width = width;
	this->height = height;
	this->direction = direction;
	this->speed = speed;
	this->rotatable = rotatable;
	this->angleOffset = angleOffset;
}

// commands to move character
// - change mode
// - change direction
// update position (call the move position functions)


// Calculates rotation angle by comparing this entity's 
// coordinates with the coordinates of the goal entity
// which can be mouse or another entity. It then calculates
// angle using basic trig function.
void Motion::calculateRotationAngle(Vector2 scaledCoordinates, Vector2 goalCoordinates) {

	if (!this->rotatable)
		return;
	// get the distance, (a, b) in right triangle 
	float x = scaledCoordinates.x - goalCoordinates.x;
	float y = scaledCoordinates.y - goalCoordinates.y;
	// printf("Coords (%f, %f) : (%f, %f)\n", mouseCoordinates.x, mouseCoordinates.y, destRect.x, destRect.y);
	this->rotationAngle = atan2(y, x) * 180.0 / PI;
	this->rotationAngle += this->angleOffset;
}

void Motion::moveLeft(MODE& mode) {
	position.x -= (5 * speed);
	mode = MOVING;
	direction = 1;
}

void Motion::moveRight(MODE& mode) {
	position.x += (5 * speed);
	mode = MOVING;
	direction = 2;
}

void Motion::moveUp(MODE& mode) {
	position.y -= (5 * speed);
	mode = MOVING;
	direction = 3;
}

void Motion::moveDown(MODE& mode) {
	position.y += (5 * speed);
	mode = MOVING;
	direction = 4;
}

Rectangle Motion::getOriginalRect() {
	return { this->position.x , this->position.y, (float)this->width, (float)this->height };
}

// REMOVED THESE CLASSES FOR NOW TO AVOID ISSUES

// subtract to get direction vector
Vector2 Motion::subtract(Vector2 v1, Vector2 v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}

// this gets the length of the vector
float Motion::magnitude(Vector2 v) {
	return sqrt(v.x * v.x + v.y * v.y);
}


Vector2 Motion::normalize(Vector2 v) {
	float mag = this->magnitude(v);
	// divide by zero error
	if (mag == 0) return { 0, 0 };
	return { v.x / mag, v.y / mag };
}


void Motion::moveEnemy(Vector2 position) {
	Vector2 directionVector = subtract(position, this->position);

	Vector2 normalizedDirection = normalize(directionVector);
	this->position.x += normalizedDirection.x * speed;
	this->position.y += normalizedDirection.y * speed;
}

void Motion::moveEnemyKnockback(Vector2 position) {
	Vector2 directionVector = subtract(position, this->position);

	Vector2 normalizedDirection = normalize(directionVector);
	this->position.x -= normalizedDirection.x * 10 * speed;
	this->position.y -= normalizedDirection.y * 10 * speed;
}
void Motion::reverseEnemyMove(Vector2 position) {
	Vector2 directionVector = subtract(position, this->position);

	Vector2 normalizedDirection = normalize(directionVector);
	this->position.x -= normalizedDirection.x  * speed;
	this->position.y -= normalizedDirection.y  * speed;
}

Vector2 Motion::scale(Vector2 v, float scaleFactor) {
	return { v.x * scaleFactor, v.y * scaleFactor };
}


bool Motion::checkPointRectCollision(Vector2 point) {
	Vector2 origin = { this->width / 4.0f, this->height / 4.0f };
	//destRect.x - origin.x / 2.0, destRect.y - origin.y / 2.0, destRect.width / 2, destRect.height / 2,
	Vector2 finalPos = { position.x - origin.x, position.y - origin.y };
	
	return point.x >= finalPos.x &&
		point.x <= finalPos.x + width / 2.0 &&
		point.y >= finalPos.y &&
		point.y <= finalPos.y + height / 2.0;
}

bool Motion::checkRectRectCollision(const Motion& other) {
	// Calculate the half-sizes for both rectangles
	Vector2 halfSizeThis = { (float)this->width / 4.0f, (float)this->height / 4.0f };
	Vector2 halfSizeOther = { other.width / 4.0f, other.height / 4.0f };

	// Calculate the "reduced-size" rectangles
	Rectangle reducedThis = { position.x - halfSizeThis.x, position.y - halfSizeThis.y, width / 2.0f, height / 2.0f };
	Rectangle reducedOther = { other.position.x - halfSizeOther.x, other.position.y - halfSizeOther.y, other.width / 2.0f, other.height / 2.0f };

	// Check for collision between the reduced-size rectangles
	return reducedThis.x < reducedOther.x + reducedOther.width &&
		reducedThis.x + reducedThis.width > reducedOther.x &&
		reducedThis.y < reducedOther.y + reducedOther.height &&
		reducedThis.y + reducedThis.height > reducedOther.y;
}

bool Motion::checkSmallRectRectCollision(const Motion& other, float factor) {
	// Calculate the half-sizes for both rectangles
	float halvedFactor = factor / 2;
	Vector2 halfSizeThis = { (float)this->width / factor, (float)this->height / factor };
	Vector2 halfSizeOther = { other.width / factor, other.height / factor };

	// Calculate the "reduced-size" rectangles
	Rectangle reducedThis = { position.x - halfSizeThis.x, position.y - halfSizeThis.y, width / halvedFactor, height / halvedFactor };
	Rectangle reducedOther = { other.position.x - halfSizeOther.x, other.position.y - halfSizeOther.y, other.width / halvedFactor, other.height / halvedFactor };

	// Check for collision between the reduced-size rectangles
	return reducedThis.x < reducedOther.x + reducedOther.width &&
		reducedThis.x + reducedThis.width > reducedOther.x &&
		reducedThis.y < reducedOther.y + reducedOther.height &&
		reducedThis.y + reducedThis.height > reducedOther.y;
}
