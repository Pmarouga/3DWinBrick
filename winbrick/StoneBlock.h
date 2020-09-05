#ifndef STONE_BLOCK_H
#define STONE_BLOCK_H

#include <glm/glm.hpp>
#include <winbrick/Block.h>

class Drawable;

/**
 * Represents the bounding box
 */
class StoneBlock : public Block {
public:

    StoneBlock(glm::vec3 position);
    ~StoneBlock();


};

#endif
