#ifndef WOOD_BLOCK_H
#define WOOD_BLOCK_H

#include <glm/glm.hpp>
#include <winbrick/Block.h>

class Drawable;

/**
 * Represents the bounding box
 */
class WoodBlock : public Block{
public:

    WoodBlock(glm::vec3 position);
    ~WoodBlock();

   
};

#endif
