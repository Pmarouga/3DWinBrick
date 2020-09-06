#ifndef BLOCKMESH_H
#define BLOCKMESH_H

#include<list>;
#include<common/shader.h>
#include <glm/glm.hpp>
#include<winbrick/WoodBlock.h>
#include<GL/glew.h>


class Drawable;
class loadShaders;

/**
 * Represents the bounding box
 */
class BlockMesh {
public:

 
    Block* blockMesh[8][8][8];
    
    GLuint* shaderPr;

 //   Drawable* cube;
    glm::vec3 pos;
    int hits, level, mesh[8][8][8];
    GLuint modelMatrixlocation;

    GLuint diffuseColorSampler, specularColorSampler;
    GLuint diffuseTextureBlock, specularTextureWoodBlock;

    BlockMesh(GLuint* shader);
    BlockMesh();
    ~BlockMesh();
    void bind(unsigned int drawable = 0);
    void draw(unsigned int drawable = 0);
    void update();
    void createMesh(int level);
};

#endif
