#include "MystBlock.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/ModelLoader.h>
using namespace glm;

MystBlock::MystBlock(vec3 position): Block() {
    pos=position;
    hits=1;
    type = 3;
    cube = new Drawable("models/cube.obj");
}

MystBlock::~MystBlock() {
    delete cube;
}
