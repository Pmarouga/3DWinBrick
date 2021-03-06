#ifndef CUBE_H
#define CUBE_H


#include "RigidBody.h"


class Drawable;

class Cube : public RigidBody {
public:

    Drawable* cube;
    float l;
    glm::mat4 modelMatrix;

	Cube(glm::vec3 pos, glm::vec3 vel, float length, float mass);
	~Cube();

    void update1(float t, float dt);
    void update2(float t, float dt);
    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif