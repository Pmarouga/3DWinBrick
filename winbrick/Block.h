#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class Block {
public:
    Drawable* cube;
    glm::vec3 pos;
    int hits;
    glm::mat4 modelMatrix;

    Block(glm::vec3 position);
    Block();
    ~Block();
    void bind(unsigned int drawable = 0);
    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
