#include "BlockMesh.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>

using namespace glm;


BlockMesh::BlockMesh(GLuint* shader) {


    wood=Material{
     vec4(1.0, 1.0, 1.0, 1),
      vec4(0.64, 0.64, 0.64, 1),
       vec4(0.5, 0.5, 0.5, 1),
       96.078431f
    };
    
    empty=Material{
        vec4(1.0, 1.0, 1.0, 1),
         vec4(1.0, 1.0, 1.0, 1),
          vec4(1.0, 1.0, 1.0, 1),
          1.0f
    };

    level = 1;
    shaderPr = *shader;
    count = 0;
    justGotHit = 0;

    diffuseTextureWoodBlock = loadSOIL("models/WoodBlock_Diffuse1.jpg");
    diffuseTextureStoneBlock = loadSOIL("models/StoneBlockRealistic_Diffuse.jpg");
    diffuseTextureBrickBlock = loadSOIL("models/BrickBlock_Diffuse1.jpg");
    diffuseTextureSteelBlock = loadSOIL("models/MystBlock_Diffuse.jpg");

    KaLocation = glGetUniformLocation(shaderPr, "mtl.Ka");
    KdLocation = glGetUniformLocation(shaderPr, "mtl.Kd");
    KsLocation = glGetUniformLocation(shaderPr, "mtl.Ks");
    NsLocation = glGetUniformLocation(shaderPr, "mtl.Ns");

    diffuseColorSampler = glGetUniformLocation(shaderPr, "diffuseColorSampler");
    specularColorSampler = glGetUniformLocation(shaderPr, "specularColorSampler");

    modelMatrixlocation = glGetUniformLocation(shaderPr, "M");



    vec3 pos = vec3(0.5, 0.5, 0.5);
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                 blockMesh[x][y][z]= new Block(vec3(0,0,0));
                 blockMesh[x][y][z]->type = -1;
                 collisionDetector[x][y][z] = 1;
            }
        }

    }
}

BlockMesh::BlockMesh() {

    level = 1;
    pos = vec3(0, 0, 0);

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                blockMesh[x][y][z] = new Block(vec3(0, 0, 0));
                
            }
        }

    }
  
}


BlockMesh::~BlockMesh() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                delete blockMesh[x][y][z]->cube;
            }
        }

    }
    vec3 pos = vec3(0.5, 0.5, 0.5);

}
void BlockMesh ::bind(unsigned int drawable) {
   
}

void BlockMesh::uploadMaterial(const Material& mtl) {
    glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
    glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
    glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
    glUniform1f(NsLocation, mtl.Ns);
}

void BlockMesh::draw(unsigned int drawable) {

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                if (blockMesh[x][y][z]->type!=-1) {
                    if (blockMesh[x][y][z]->type == 0) {
                        blockMesh[x][y][z]->bind();
                        glUniformMatrix4fv(modelMatrixlocation, 1, GL_FALSE, &blockMesh[x][y][z]->modelMatrix[0][0]);
                        //select texture and flush to shader
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, diffuseTextureWoodBlock);
                        glUniform1i(diffuseColorSampler, 0);
                     //   uploadMaterial(wood);
                        blockMesh[x][y][z]->draw();
                    }
                    else if (blockMesh[x][y][z]->type == 1) {
                        blockMesh[x][y][z]->bind();
                        glUniformMatrix4fv(modelMatrixlocation, 1, GL_FALSE, &blockMesh[x][y][z]->modelMatrix[0][0]);
                        //select texture and flush to shader
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, diffuseTextureStoneBlock);
                        glUniform1i(diffuseColorSampler, 0);
                       // uploadMaterial(empty);
                        blockMesh[x][y][z]->draw();
                    }
                    else if (blockMesh[x][y][z]->type == 2) {
                        blockMesh[x][y][z]->bind();
                        glUniformMatrix4fv(modelMatrixlocation, 1, GL_FALSE, &blockMesh[x][y][z]->modelMatrix[0][0]);
                        //select texture and flush to shader
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, diffuseTextureBrickBlock);
                        glUniform1i(diffuseColorSampler, 0);
                        // uploadMaterial(empty);
                        blockMesh[x][y][z]->draw();
                    }
                    else if (blockMesh[x][y][z]->type == 3) {
                        blockMesh[x][y][z]->bind();
                        glUniformMatrix4fv(modelMatrixlocation, 1, GL_FALSE, &blockMesh[x][y][z]->modelMatrix[0][0]);
                        //select texture and flush to shader
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, diffuseTextureSteelBlock);
                        glUniform1i(diffuseColorSampler, 0);
                        // uploadMaterial(empty);
                        blockMesh[x][y][z]->draw();
                    }
                }
            }
        }

    }
}

void BlockMesh::update() {

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                blockMesh[x][y][z]->update(0.0,0.0);
            }
        }

    }

}

void BlockMesh::createMesh(int level) {
    int collisionFlag = 0;
    int selector = 0;
    int voidBlock = 1;

    if (level == 1) {
        for (int z = 0; z < 8; z++) {
            if (z == 0 || z == 7) {
                collisionFlag = 1;
            }
            pos.z = z + 0.5;
            for (int y = 5; y < 8; y++) {
                if (y == 6 || y == 5 || y==7 || y==8) {
                    collisionFlag = 1;
                }
                pos.y = y + 0.5;
                for (int x = 0; x < 8; x++) {
                    if (x == 0 || x == 7) {
                        collisionFlag = 1;
                    }
                    pos.x = x + 0.5;
                    if (collisionFlag == 1) {
                        collisionDetector[x][y][z] = 1;
                    }
                   
                    
                    if (selector == 0 && voidBlock != 0 && voidBlock<=8) {
                        blockMesh[x][y][z] = new WoodBlock(pos);
                        blockMesh[x][y][z]->type = 0;
                        count++;
                        selector = 1;
                        voidBlock++;
                    }

                    else if (selector == 1 && voidBlock != 0 && voidBlock<=8) {
                        blockMesh[x][y][z] = new StoneBlock(pos);
                        blockMesh[x][y][z]->type = 1;
                        count++;
                        selector = 2;
                        voidBlock++;
                    }

                    else if (selector == 2 && voidBlock != 0 && voidBlock<=8) {
                        blockMesh[x][y][z] = new BrickBlock(pos);
                        blockMesh[x][y][z]->type = 2;
                        count++;
                        selector = 3;
                        voidBlock++;
                    }
                     else if (selector == 3 && voidBlock!=0 && voidBlock<=8) {
                        blockMesh[x][y][z] = new MystBlock(pos);
                        blockMesh[x][y][z]->type = 3;
                        count++;
                        selector = 0;
                        voidBlock++;
                    }
                     else {
                        blockMesh[x][y][z] = new Block(vec3(0, 0, 0));
                        blockMesh[x][y][z]->type = -1;
                        voidBlock = 0;
                    }
                    if (voidBlock == 0) {
                        voidBlock = 1;
                    }
                    else if (voidBlock == 8) {
                        voidBlock == 0;
                    }

                    collisionFlag = 0;
                }
                collisionFlag = 0;
            }
            collisionFlag = 0;
        }
    }
    vec3 pos = vec3(0.5, 0.5, 0.5);
}
