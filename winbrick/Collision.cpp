#include "Collision.h"
#include "Box.h"
#include "Sphere.h"
#include "Cube.h"
#include "BlockMesh.h"

using namespace glm;


bool checkForBoxSphereCollision(glm::vec3& pos, const float& r, const float& size, glm::vec3& n);
bool checkForCubeSphereCollision(glm::vec3& posSphere, const float& aktina, glm::vec3& posCube, float mass, float length);
void handleCubeSphereCollision(Cube& cube, Sphere& sphere);
void handleBoxSphereCollision(Box& box, Sphere& sphere);
//void handleMeshBlockSphereCollision(BlockMesh& blockMesh, Sphere& sphere);
//bool checkForBlockMeshSphereCollision();


void handleBoxSphereCollision(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision(sphere.x, sphere.r, box.size, n)) {
		sphere.v = reflect(sphere.v,n);
		sphere.P = sphere.v*sphere.m;

    }
}
/*void handleMeshBlockSphereCollision(BlockMesh& blockMesh, Sphere& sphere) {
    vec3 n;
    if (1) {
        sphere.v = reflect(sphere.v, n);
        sphere.P = sphere.v * sphere.m;
    }
}

bool checkForBlockMeshSphereCollision(int isOcuppied[8][8][8] ) {

}*/


bool checkForBoxSphereCollision(vec3& pos, const float& r, const float& size, vec3& n) {
    if (pos.x - r <= 0) {
        //correction
        float dis = -(pos.x - r);
        pos = pos + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    } else if (pos.x + r >= size) {
        //correction
        float dis = size - (pos.x + r);
        pos = pos + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    }  else if (pos.y + r >= size) {
        //correction
        float dis = size - (pos.y + r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, 1, 0);
    }
    //axis z turned off
    /*
    else if (pos.z - r <= 0) {
        //correction
        float dis = -(pos.z - r);
        pos = pos + vec3(0, 0, dis);

        n = vec3(0, 0, -1);
    } else if (pos.z + r >= size) {
        //correction
        float dis = size - (pos.z + r);
        pos = pos + vec3(0, 0, dis);

        n = vec3(0, 0, 1);
    }
    */
    else {
        return false;
    }
    return true;
}

void handleCubeSphereCollision(Cube & cube, Sphere & sphere)
{
    vec3 n = vec3(0, 1, 0);
	if (checkForCubeSphereCollision(sphere.x, sphere.r, cube.x, cube.m, cube.l)) {
		// Task 2b: define the velocity of the sphere after the collision
		sphere.v = reflect(sphere.v, n);
		sphere.P = sphere.v*sphere.m;

	}
}
bool checkForCubeSphereCollision(vec3& posSphere, const float& aktina, vec3& posCube, float mass, float length) {
    if (posSphere.y - aktina <= posCube.y + mass / 2) {
        if ((posSphere.x +aktina/2 <= posCube.x + length / 2) && (posSphere.x+aktina/2 >= posCube.x - length / 2)) {
            if ((posSphere.z+aktina/2 <= posCube.z + length / 2) && (posSphere.z+aktina/2  >= posCube.z - length / 2)) {
                return true;
            }
           


        }
    }
    return false;
}
