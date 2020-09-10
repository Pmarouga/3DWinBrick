#ifndef GUN_H
#define GUN_H

#include<glm/glm.hpp>
#include<winbrick/Cube.h>
#include<winbrick/Sphere.h>
#include<vector>


class Cube;
class Sphere;

class Gun {
public:

	Cube* kyvos;
	Sphere* bull;

	Gun(Cube& cube);
	void shoot();
	void updateBulets();

};

#endif