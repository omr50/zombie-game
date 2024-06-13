#include "BaseSprite.h"

BaseSprite::BaseSprite(Motion& spriteMotions, ScalingComponent spriteScales)
    : motions(spriteMotions), scales(spriteScales), mode(IDLE) {
    this->motions = motions;
    this->scales = scales;
    this->mode = IDLE;
}
