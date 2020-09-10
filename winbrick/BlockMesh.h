#ifndef BLOCKMESH_H
#define BLOCKMESH_H

#include<list>;
#include <glm/glm.hpp>
#include<winbrick/WoodBlock.h>
#include<winbrick/Block.h>
#include<winbrick/StoneBlock.h>
#include<winbrick/BrickBlock.h>
#include<winbrick/MystBlock.h>
struct Material {
    glm::vec4 Ka;
    glm::vec4 Kd;
    glm::vec4 Ks;
    float Ns;
};

class Drawable;
class loadShaders;



/**
 * Represents the bounding box
 */
class BlockMesh {
public:
    Block* blockMesh[8][8][8];
    
    Material wood, empty;
    GLuint shaderPr;
 //   Drawable* cube;
    glm::vec3 pos;
    int level, collisionDetector[8][8][8], count, justGotHit;

    GLuint modelMatrixlocation;

    GLuint diffuseColorSampler, specularColorSampler;
    GLuint diffuseTextureWoodBlock, diffuseTextureStoneBlock, diffuseTextureBrickBlock, diffuseTextureSteelBlock;
   
    // material properties
    GLuint KdLocation, KsLocation, KaLocation, NsLocation;


    BlockMesh(GLuint* shader);
    BlockMesh();
    ~BlockMesh();
    void bind(unsigned int drawable = 0);
    void draw(unsigned int drawable = 0);
    void update();
    void createMesh(int level);
    void BlockMesh::uploadMaterial(const Material& mtl);
};

#endif
