#include "StoneBlock.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>

using namespace glm;

StoneBlock::StoneBlock(vec3 position) : Block() {
    pos = position;
    hits = 3;
    cube = new Drawable("models/cube.obj");
}

StoneBlock::~StoneBlock() {
    delete cube;
}