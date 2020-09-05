#include "BlockMesh.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>
using namespace glm;

BlockMesh::BlockMesh(vec3 position) {

    pos=position;
    hits = 0;
    cube = new Drawable("models/cube.obj");
}
BlockMesh::BlockMesh() {

    pos = vec3(0, 0, 0);
    hits = 0;
    cube = new Drawable("models/cube.obj");
}

BlockMesh::~BlockMesh() {
    delete cube;
}
void BlockMesh ::bind(unsigned int drawable) {
    cube->bind();
}
void BlockMesh::draw(unsigned int drawable) {

    cube->draw();
   
}

void Blockmesh::update(float t, float dt) {

    mat4 translate = glm::translate(mat4(), pos);
    mat4 scale = glm::scale(mat4(), vec3(0.8, 0.8, 0.8));
    modelMatrix = translate * scale;


}
