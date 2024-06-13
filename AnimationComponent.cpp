/*
Each movable sprite has its own animation component
because it may have several states (move, jump, idle, action
etc.) and so this class has a map which holds key value
pairs of MODE enum (state) to the frames that must be loaded.
It makes animation easier by allowing you to just call draw
by passing in the sprite. The sprite and animation Component hold
the state necessary to draw the object on the screen such as textures,
frames, scaling factors, current frame, etc.
*/


// the draw method does not take parameters.
// it depends on the internal state of the 
// object. We can set the state variable
// for the MODE (enum) and using that, we
// can choose which animation we want.
#include "AnimationComponent.h"

std::map<MODE, AnimationState> AnimationComponent::animations;

void AnimationComponent::draw(BaseSprite* sprite) {
	// set currFrame to 0 if we reached last frame
	if (AnimationComponent::animations[sprite->mode].currentFrame >= AnimationComponent::animations[sprite->mode].frames.size())
		AnimationComponent::animations[sprite->mode].currentFrame = 0;

	AnimationState anim = AnimationComponent::animations[sprite->mode];
	Rectangle currFrame = anim.frames[anim.currentFrame];

	// local copy to inverse direction
	// don't inverse since we use mouse to control direction
	if (sprite->motions.direction == 1) {
		//currFrame.width *= -1;
	}
	// scale the current rectangle with individual and global scales. 
	Rectangle destRect = sprite->scales.buildDestRect(sprite->motions.position, currFrame);
	Vector2 origin = { destRect.width / 2.0f, destRect.height / 2.0f };
	// Draw the current frame of anim
	DrawTexturePro(anim.spriteTexture, currFrame, destRect, origin, sprite->motions.rotationAngle, sprite->color);
	// draws halved rect centered on object
	//DrawRectangle(destRect.x - origin.x/2.0, destRect.y - origin.y / 2.0, destRect.width / 2, destRect.height / 2, { 255, 0, 0, 120 });
	//DrawCircleV({ destRect.x - origin.x, destRect.y - origin.y }, destRect.width / 2.0, { 255, 0, 0, 120 });
	AnimationComponent::animations[sprite->mode].currentFrame++;
}

void AnimationComponent::drawCircle(BaseSprite* sprite) {

	Rectangle destRect = sprite->scales.buildDestRect(sprite->motions.position, { sprite->motions.position.x, sprite->motions.position.y, (float)sprite->motions.width, (float)sprite->motions.height });
	DrawCircleV({destRect.x, destRect.y}, destRect.width, GOLD);
}

// add an AnimationState object to the vector 
// return pointer to this, just to make it easier
// to chain multiple animation creations together
AnimationComponent* AnimationComponent::addAnimationState(MODE mode, std::string path, int numFrames, int columns, int sizeofXFrame, int sizeofYFrame) {
	AnimationState currState;
	currState.spriteTexture = LoadTexture(path.c_str());
	if (currState.spriteTexture.id == 0) {
		TraceLog(LOG_ERROR, "Failed to load sprite");
	}
	currState.currentFrame = 0;
	initializeFrames(currState.frames, numFrames, columns, sizeofXFrame, sizeofYFrame);

	// push into the map
	animations.insert({ mode, currState });
	return this;
}



// initialize the frames vector
void AnimationComponent::initializeFrames(std::vector<Rectangle>& frames, int numFrames, int columns, int sizeofXFrame, int sizeofYFrame) {
	for (int i = 0; i < numFrames; i++) {
		frames.push_back(Rectangle());
		frames[i].x = (i % columns) * sizeofXFrame; // Calculate X based on columns
		frames[i].y = (i / columns) * sizeofYFrame; // Calculate Y based on rows (will be 0 if only one row)
		frames[i].width = sizeofXFrame;
		frames[i].height = sizeofYFrame;
	}
}

void AnimationComponent::drawLine(Vector2 position1, Vector2 position2, Color color) {
	DrawLine(position1.x, position1.y, position2.x, position2.y, color);
}
