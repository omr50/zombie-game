#pragma once
#include <map>
#include <vector>
#include <string>
#include "MODE.h"
#include "BaseSprite.h"

struct AnimationState {
    std::vector<Rectangle> frames;
    Texture2D spriteTexture;
    int currentFrame;
};

class AnimationComponent {

public:
    static std::map<MODE, AnimationState> animations;
    void draw(BaseSprite* sprite);
    void drawCircle(BaseSprite* sprite);
    AnimationComponent* addAnimationState(MODE mode, std::string path, int numFrames, int columns, int sizeofXFrame, int sizeofYFrame);
    void initializeFrames(std::vector<Rectangle>& frames, int numFrames, int columns, int sizeofXFrame, int sizeofYFrame);
    void drawLine(Vector2 position1, Vector2 position2, Color color);   
};


