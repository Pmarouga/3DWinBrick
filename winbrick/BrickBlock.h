#ifndef BRICK_BLOCK_H
#define BRICK_BLOCK_H

#include <glm/glm.hpp>
#include <winbrick/Block.h>

class Drawable;

/**
 * Represents the bounding box
 */
class BrickBlock : public Block{
public:

    BrickBlock(glm::vec3 position);
    ~BrickBlock();

   
};

#endif
