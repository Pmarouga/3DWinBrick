#include "BrickBlock.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>
using namespace glm;

BrickBlock::BrickBlock(vec3 position): Block() {
    pos=position;
    hits=2;
    cube = new Drawable("models/cube.obj");
}

BrickBlock::~BrickBlock() {
    delete cube;
}
