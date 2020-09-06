#include "BlockMesh.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>

using namespace glm;



BlockMesh::BlockMesh(GLuint* shader) {

    level = 1;
    shaderPr = shader;

    diffuseColorSampler = glGetUniformLocation(*shaderPr, "diffuseColorSampler");
    specularColorSampler = glGetUniformLocation(*shaderPr, "specularColorSampler");

    modelMatrixlocation = glGetUniformLocation(*shaderPr, "M");



    vec3 pos = vec3(0.5, 0.5, 0.5);
    hits = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                 blockMesh[x][y][z]= new Block(vec3(0,0,0));
                 mesh[x][y][z] = 0;
            }
        }

    }
}

BlockMesh::BlockMesh() {

    level = 1;
    pos = vec3(0, 0, 0);
    hits = 0;
    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {
            for (int z = 0; z < 7; z++) {
                blockMesh[x][y][z] = new Block(vec3(0, 0, 0));
            }
        }

    }
  
}


BlockMesh::~BlockMesh() {
    for (int x = 0; x < 7; x++) {
        for (int y = 3; y < 7; y++) {
            for (int z = 0; z < 7; z++) {
                delete blockMesh[x][y][z]->cube;
            }
        }

    }
}
void BlockMesh ::bind(unsigned int drawable) {
   
}

void BlockMesh::draw(unsigned int drawable) {


    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {
            for (int z = 0; z < 7; z++) {
                if (mesh[x][y][z] != 0) {

                    blockMesh[x][y][z]->bind();


                    glUniformMatrix4fv(modelMatrixlocation, 1, GL_FALSE, &blockMesh[x][y][z]->modelMatrix[0][0]);
                    //select texture and flush to shader
                    if (mesh[x][y][z] == 1) {
                        diffuseTextureBlock = blockMesh[x][y][z]->loadTexture(0);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, diffuseTextureBlock);
                        glUniform1i(diffuseColorSampler, 0);
                    }
                    blockMesh[x][y][z]->draw();

                }
            }
        }

    }
}

void BlockMesh::update() {

    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {
            for (int z = 0; z < 7; z++) {
                blockMesh[x][y][z]->update(0.0,0.0);;
            }
        }

    }

}

void BlockMesh::createMesh(int level) {
    
    if (level == 1) {
        for (int x = 0; x < 7; x++) {
            for (int y = 3; y < 7; y++) {
                for (int z = 0; z < 7; z++) {
                    pos += vec3(x, y, z);
                    blockMesh[x][y][z] = new WoodBlock(pos);
                    mesh[x][y][z] = 1;

                }
            }

        }
    }

}
