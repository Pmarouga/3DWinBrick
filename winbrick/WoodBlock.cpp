#include "WoodBlock.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>
using namespace glm;

WoodBlock::WoodBlock(vec3 position): Block() {
    pos=position;
    hits=1;
    cube = new Drawable("models/cube.obj");
}

WoodBlock::~WoodBlock() {
    delete cube;
}

/*
void WoodBlock::draw(unsigned int drawable) {
	mat4 translate = glm::translate(mat4(), pos);
	mat4 scale = glm::scale(mat4(), vec3(0.70, 0.50, 1));
	modelMatrix = translate * scale;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    cube->bind();
    cube->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void WoodBlock::update(float t, float dt) {
   
}
*/
