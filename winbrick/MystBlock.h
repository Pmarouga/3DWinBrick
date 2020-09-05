#ifndef MYST_BLOCK_H
#define MYST_BLOCK_H

#include <glm/glm.hpp>
#include <winbrick/Block.h>

class Drawable;

/**
 * Represents the bounding box
 */
class MystBlock : public Block{
public:

    MystBlock(glm::vec3 position);
    ~MystBlock();

   
};

#endif
