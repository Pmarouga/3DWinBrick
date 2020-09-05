#ifndef BLOCKMESH_H
#define BLOCKMESH_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class BlockMesh {
public:
    Drawable* cube;
    glm::vec3 pos;
    int hits;
    glm::mat4 modelMatrix;

    BlockMesh(glm::vec3 position);
    BlockMesh();
    ~BlockMesh();
    void bind(unsigned int drawable = 0);
    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
