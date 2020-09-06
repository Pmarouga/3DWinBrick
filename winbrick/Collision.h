#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

class Box;
class Sphere;
class Cube;
bool checkForBoxSphereCollision(glm::vec3& pos, const float& r, const float& size, glm::vec3& n);
bool checkForCubeSphereCollision(glm::vec3& posSphere, const float& aktina, glm::vec3& posCube, float mass, float length);
void handleCubeSphereCollision(Cube& cube, Sphere& sphere);
void handleBoxSphereCollision(Box& box, Sphere& sphere);
//void handleMeshBlockSphereCollision(BlockMesh& blockMesh, Sphere& sphere);
//bool checkForBlockMeshSphereCollision();

#endif
