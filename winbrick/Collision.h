#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

class Box;
class Sphere;
class Cube;
void handleBoxSphereCollision(Box& box, Sphere& sphere);
void handleCubeSphereCollision(Cube& cube, Sphere& sphere2);

#endif
