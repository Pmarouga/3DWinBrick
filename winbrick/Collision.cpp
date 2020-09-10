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

vec3 reflectionVec = vec3(0, 1, 0);
//void handleMeshBlockSphereCollision(BlockMesh& blockMesh, Sphere& sphere);
//bool checkForBlockMeshSphereCollision();


void handleBoxSphereCollision(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision(sphere.x, sphere.r, box.size, n)) {
		sphere.v = reflect(sphere.v,n);
		sphere.P = sphere.v*sphere.m;

    }
}

void handleMeshBlockSphereCollision(BlockMesh& blockMesh, Sphere& sphere, int check) {
    vec3 n;
    bool collisionFlag = false;
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            for (int z = 0; z < 8; z++) {
                if (blockMesh.collisionDetector[x][y][z] == 1) {
                    collisionFlag = handleBlockSphereCollision(*blockMesh.blockMesh[x][y][z], sphere,check);
                    if (collisionFlag) {
                        blockMesh.blockMesh[x][y][z]->hits--;
                        blockMesh.justGotHit = 1;
                        if (blockMesh.blockMesh[x][y][z]->hits==0) {
                            if (blockMesh.blockMesh[x][y][z]->type == 3) { 
                               sphere.v.x = 2 * sphere.v.x;
                               sphere.v.y = 2 * sphere.v.y;
                               sphere.v.z = 2 * sphere.v.z;
                            }
                            blockMesh.blockMesh[x][y][z] = new Block(vec3(0, 0, 0));
                            blockMesh.blockMesh[x][y][z]->type = -1;
                            blockMesh.count--;
                            
                        }
                        sphere.v = reflect(sphere.v, reflectionVec);
                        sphere.P = sphere.v * sphere.m;
                        break;
                        
                    }
                }
                if (blockMesh.justGotHit == 1) {
                    break;
                }
            }
            if (blockMesh.justGotHit == 1) {
                break;
            }
        }
        if (blockMesh.justGotHit == 1) {
            break;
        }
    }
   
}

bool handleBlockSphereCollision(Block& block, Sphere& sphere, int check) {
   
    if (check == 0) {
        
              //bottom
        if ((sphere.x.x + sphere.r >= block.pos.x - 0.5) && (sphere.x.x + sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r  >= block.pos.y - 0.5) && (sphere.x.y + sphere.r  <= block.pos.y + 0.5)) {
                if ((sphere.x.z + sphere.r  >= block.pos.z - 0.5) && (sphere.x.z + sphere.r  <= block.pos.z + 0.5)) {
                    //correction
                    float dis = -((block.pos.y - 0.5) - (sphere.x.y + sphere.r));
                    sphere.x += vec3(0, dis, 0);
                    reflectionVec = vec3(0, -1, 0);
                    return true;
                }
            }
        }
        if ((sphere.x.x - sphere.r/2 >= block.pos.x - 0.5) && (sphere.x.x - sphere.r/2 <= block.pos.x + 0.5)) {
            if ((sphere.x.y - sphere.r/2 >= block.pos.y - 0.5) && (sphere.x.y - sphere.r/2 <= block.pos.y + 0.5)) {
                if ((sphere.x.z - sphere.r/2 >= block.pos.z - 0.5) && (sphere.x.z - sphere.r/2 <= block.pos.z + 0.5)) {
                    //correction
                    float dis = -((block.pos.y - 0.5) - (sphere.x.y + sphere.r));
                    sphere.x += vec3(0, dis, 0);
                    reflectionVec = vec3(0, -1, 0);
                    return true;
                }
            }
        }
        //top
        if ((sphere.x.x + sphere.r  >= block.pos.x - 0.5) && (sphere.x.x + sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y - sphere.r  >= block.pos.y - 0.5) && (sphere.x.y - sphere.r  <= block.pos.y + 0.5)) {
                if ((sphere.x.z + sphere.r  >= block.pos.z - 0.5) && (sphere.x.z + sphere.r  <= block.pos.z + 0.5)) {
                    float dis = ((block.pos.y - 0.5) - (sphere.x.y + sphere.r ));
                    sphere.x += vec3(0, dis, 0);
                    reflectionVec = vec3(0, 1, 0);
                    return true;
                }
            }
        }
        if ((sphere.x.x - sphere.r >= block.pos.x - 0.5) && (sphere.x.x - sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r  >= block.pos.y - 0.5) && (sphere.x.y + sphere.r  <= block.pos.y + 0.5)) {
                if ((sphere.x.z - sphere.r  >= block.pos.z - 0.5) && (sphere.x.z - sphere.r  <= block.pos.z + 0.5)) {
                    float dis = ((block.pos.y - 0.5) - (sphere.x.y + sphere.r ));
                    sphere.x += vec3(0, dis, 0);
                    reflectionVec = vec3(0, 1, 0);
                    return true;
                }
            }
        }

        //left
        if ((sphere.x.x + sphere.r  >= block.pos.x - 0.5) && (sphere.x.x + sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r  <= block.pos.y + 0.5) && (sphere.x.y - sphere.r  >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r  <= block.pos.z + 0.5) && (sphere.x.z - sphere.r  >= block.pos.z - 0.5)) {
                    reflectionVec = vec3(-1, 0, 0);
                    return true;

                }
            }
        }
        if ((sphere.x.x - sphere.r  >= block.pos.x - 0.5) && (sphere.x.x - sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y - sphere.r  <= block.pos.y + 0.5) && (sphere.x.y + sphere.r  >= block.pos.y - 0.5)) {
                if ((sphere.x.z - sphere.r  <= block.pos.z + 0.5) && (sphere.x.z + sphere.r  >= block.pos.z - 0.5)) {
                    reflectionVec = vec3(-1, 0, 0);
                    return true;

                }
            }
        }
        //right

        if ((sphere.x.x - sphere.r  >= block.pos.x + 0.5) && (sphere.x.x - sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r  <= block.pos.y + 0.5) && (sphere.x.y - sphere.r  >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r <= block.pos.z + 0.5) && (sphere.x.z - sphere.r  >= block.pos.z - 0.5)) {
                    reflectionVec = vec3(-1, 0, 0);
                    return true;

                }
            }
        }
        if ((sphere.x.x + sphere.r >= block.pos.x + 0.5) && (sphere.x.x + sphere.r <= block.pos.x + 0.5)) {
            if ((sphere.x.y - sphere.r <= block.pos.y + 0.5) && (sphere.x.y + sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z - sphere.r <= block.pos.z + 0.5) && (sphere.x.z + sphere.r >= block.pos.z - 0.5)) {
                    reflectionVec = vec3(-1, 0, 0);
                    return true;

                }
            }
        }

        //front
        if ((sphere.x.x + sphere.r <= block.pos.x + 0.5) && (sphere.x.x - sphere.r >= block.pos.x - 0.5)) {
            if ((sphere.x.y + sphere.r <= block.pos.y + 0.5) && (sphere.x.y - sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z - sphere.r >= block.pos.z - 0.5) && (sphere.x.z - sphere.r <= block.pos.z + 0.5)) {
                    reflectionVec = vec3(0, 0, 1);
                    return true;

                }
            }
        }
        if ((sphere.x.x - sphere.r <= block.pos.x + 0.5) && (sphere.x.x + sphere.r >= block.pos.x - 0.5)) {
            if ((sphere.x.y - sphere.r <= block.pos.y + 0.5) && (sphere.x.y + sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r >= block.pos.z - 0.5) && (sphere.x.z + sphere.r <= block.pos.z + 0.5)) {
                    reflectionVec = vec3(0, 0, 1);
                    return true;

                }
            }
        }

        //back

        if ((sphere.x.x + sphere.r <= block.pos.x + 0.5) && (sphere.x.x - sphere.r >= block.pos.x - 0.5)) {
            if ((sphere.x.y + sphere.r <= block.pos.y + 0.5) && (sphere.x.y - sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r >= block.pos.z - 0.5) && (sphere.x.z - sphere.r <= block.pos.z + 0.5)) {
                    reflectionVec = vec3(0, 0, -1);
                    return true;

                }
            }
        }
        if ((sphere.x.x - sphere.r <= block.pos.x + 0.5) && (sphere.x.x + sphere.r >= block.pos.x - 0.5)) {
            if ((sphere.x.y - sphere.r  <= block.pos.y + 0.5) && (sphere.x.y + sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z - sphere.r >= block.pos.z - 0.5) && (sphere.x.z + sphere.r <= block.pos.z + 0.5)) {
                    reflectionVec = vec3(0, 0, -1);
                    return true;

                }
            }
        }
        return false;

    }
    else if (check == 1) {


        if ((sphere.x.x + sphere.r >= block.pos.x - 0.5) && (sphere.x.x + sphere.r <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r >= block.pos.y - 0.5) && (sphere.x.y + sphere.r <= block.pos.y + 0.5)) {
                if ((sphere.x.z + sphere.r >= block.pos.z - 0.5) && (sphere.x.z + sphere.r <= block.pos.z + 0.5)) {
                    //correction
                   
                    return true;
                }
            }
        }
        //top
        if ((sphere.x.x + sphere.r  >= block.pos.x - 0.5) && (sphere.x.x + sphere.r <= block.pos.x + 0.5)) {
            if ((sphere.x.y - sphere.r >= block.pos.y - 0.5) && (sphere.x.y - sphere.r <= block.pos.y + 0.5)) {
                if ((sphere.x.z + sphere.r  >= block.pos.z - 0.5) && (sphere.x.z + sphere.r<= block.pos.z + 0.5)) {
                    float dis = ((block.pos.y - 0.5) - (sphere.x.y + sphere.r ));
                    sphere.x = vec3(100, 100, 100);
                    sphere.v = vec3(0, 0, 0);
                    return true;
                }
            }
        }

        //left
        if ((sphere.x.x + sphere.r>= block.pos.x - 0.5) && (sphere.x.x + sphere.r <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r<= block.pos.y + 0.5) && (sphere.x.y - sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r <= block.pos.z + 0.5) && (sphere.x.z - sphere.r >= block.pos.z - 0.5)) {
                    sphere.x = vec3(100, 100, 100);
                    sphere.v = vec3(0, 0, 0);
                    return true;

                }
            }
        }
        //right

        if ((sphere.x.x - sphere.r >= block.pos.x + 0.5) && (sphere.x.x - sphere.r  <= block.pos.x + 0.5)) {
            if ((sphere.x.y + sphere.r <= block.pos.y + 0.5) && (sphere.x.y - sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r <= block.pos.z + 0.5) && (sphere.x.z - sphere.r >= block.pos.z - 0.5)) {
                    sphere.x = vec3(100, 100, 100);
                    sphere.v = vec3(0, 0, 0);
                    return true;

                }
            }
        }

        //front
        if ((sphere.x.x + sphere.r <= block.pos.x + 0.5) && (sphere.x.x - sphere.r >= block.pos.x - 0.5)) {
            if ((sphere.x.y + sphere.r <= block.pos.y + 0.5) && (sphere.x.y - sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z - sphere.r >= block.pos.z - 0.5) && (sphere.x.z - sphere.r <= block.pos.z + 0.5)) {
                    sphere.x = vec3(100, 100, 100);
                    sphere.v = vec3(0, 0, 0);
                    return true;

                }
            }
        }

        //back

        if ((sphere.x.x + sphere.r <= block.pos.x + 0.5) && (sphere.x.x - sphere.r >= block.pos.x - 0.5)) {
            if ((sphere.x.y + sphere.r  <= block.pos.y + 0.5) && (sphere.x.y - sphere.r >= block.pos.y - 0.5)) {
                if ((sphere.x.z + sphere.r >= block.pos.z - 0.5) && (sphere.x.z - sphere.r <= block.pos.z + 0.5)) {
                    sphere.x = vec3(100, 100, 100);
                    sphere.v = vec3(0, 0, 0);
                    return true;

                }
            }
        }
        return false;

    }
}
//*/


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
    
    else {
        return false;
    }
    return true;
}

void handleCubeSphereCollision(Cube & cube, Sphere & sphere)
{
    vec3 n = vec3(0, 0, 1);
    vec3 N = vec3(0, 0, 0);
    
	if (checkForCubeSphereCollision(sphere.x, sphere.r, cube.x, cube.m, cube.l)) {
		// Task 2b: define the velocity of the sphere after the collision
       
        vec3 tect = vec3(sphere.x.x - cube.x.x, sphere.x.y - cube.x.y, sphere.x.z - cube.x.z);
        N = normalize(tect);
        N.x = 12 * N.x;
        N.y = 12* N.y;
        N.z = 12* N.z;
        sphere.v = N;
		sphere.P = sphere.v*sphere.m;

	}
}
bool checkForCubeSphereCollision(vec3& posSphere, const float& aktina, vec3& posCube, float mass, float length) {
    if (posSphere.y - aktina <= posCube.y + mass / 2) {
        if ((posSphere.x +aktina/2 <= posCube.x + length / 2) && (posSphere.x+aktina/2 >= posCube.x - length / 2)) {
            if ((posSphere.z+aktina/2 <= posCube.z + length / 2) && (posSphere.z+aktina/2  >= posCube.z - length / 2)) {

                //correction 
              
                posSphere = vec3(posSphere.x,posCube.y+mass/2+aktina,posSphere.z);
                return true;
            }
           


        }
    }
    return false;
}
