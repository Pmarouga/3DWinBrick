// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include<common/texture.h>

//
#include "Cube.h"
#include "Sphere.h"
#include "Box.h"
#include "Collision.h"
#include "Block.h"
#include "WoodBlock.h"
#include "StoneBlock.h"
#include "BrickBlock.h"
#include "MystBlock.h"
#include "BlockMesh.h"
#include"Gun.h"


using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "WinBrick"

// Global variables
GLFWwindow* window;
Camera* camera;
GLuint shaderProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;

// light properties
GLuint LaLocation1, LdLocation1, LsLocation1, lightPositionLocation1, lightPowerLocation1;
GLuint LaLocation2, LdLocation2, LsLocation2, lightPositionLocation2, lightPowerLocation2;
GLuint LaLocation3, LdLocation3, LsLocation3, lightPositionLocation3, lightPowerLocation3;
GLuint LaLocation4, LdLocation4, LsLocation4, lightPositionLocation4, lightPowerLocation4;
GLuint LaLocation5, LdLocation5, LsLocation5, lightPositionLocation5, lightPowerLocation5;
// material properties
GLuint KdLocation, KsLocation, KaLocation, NsLocation;

//texture properties
GLuint diffuseColorSampler, specularColorSampler;
GLuint diffuseTextureCube, diffuseTextureFloor, diffuseTextureEarth, diffuseTextureJupiter;

int bulls;
float cubeL;
Sphere* bullets[20];

struct Light {
    glm::vec4 La;
    glm::vec4 Ld;
    glm::vec4 Ls;
    glm::vec3 lightPosition_worldspace;
    float power;
};

Light light1{
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec3{ 4, 6, 12 },
    9.0f
};

Light light2{
    vec4{ 1, 0, 0, 1 },
    vec4{ 1, 0, 0, 1 },
    vec4{ 1, 0, 0, 1 },
    vec3{ 4, 6, -4 },
    9.0f
};

Light light3{
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec3{ -4, 6, 4 },
    9.0f
};
Light light4{
    vec4{ 0, 0, 1, 1 },
    vec4{ 0, 0, 1, 1 },
    vec4{ 0, 0, 1, 1 },
    vec3{ 12, 6, 4 },
    9.0f
};

Light light5{
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec3{ 4, 4, 4 },
    4.0f
};
Light light6{
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec3{ 0, 0, 0},
    9.0f
};


Material wood{
    vec4(1.0, 1.0, 1.0, 1),
     vec4(0.64, 0.64, 0.64, 1),
      vec4(0.5, 0.5, 0.5, 1),
      96.078431f
};
Material empty{
    vec4(1.0, 1.0, 1.0, 1),
     vec4(1.0, 1.0, 1.0, 1),
      vec4(1.0, 1.0, 1.0, 1),
      1.0f
};


//moving brick
Cube* cube, *cubeFloor, *cubeLeft, *cubeRight, *cubeBack;
//ball
Sphere* sphere, *sphere1;
//gamespace
Box* box;
//hitbox
Block* block;

Gun* gun;

// tests
Cube* cube1;
BlockMesh* blockMesh;

int useText;
void swapLights() {
    light6.La = light1.La;
    light6.Ld = light1.Ld;
    light6.Ls = light1.Ls;

    light1.La = light2.La;
    light1.Ld = light2.Ld;
    light1.Ls = light2.Ls;
    
    light2.La = light3.La;
    light2.Ld = light3.Ld;
    light2.Ls = light3.Ls;
   
    light3.La = light4.La;
    light3.Ld = light4.Ld;
    light3.Ls = light4.Ls;

    light4.La = light5.La;
    light4.Ld = light5.Ld;
    light4.Ls = light5.Ls; 
    
    light5.La = light6.La;
    light5.Ld = light6.Ld;
    light5.Ls = light6.Ls;
}
void uploadMaterial(const Material& mtl) {
    glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
    glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
    glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
    glUniform1f(NsLocation, mtl.Ns);
}

void uploadLight(const Light& light1, const Light& light2, const Light& light3, const Light& light4, const Light& light5) {
    //upload light 1
    glUniform4f(LaLocation1, light1.La.r, light1.La.g, light1.La.b, light1.La.a);
    glUniform4f(LdLocation1, light1.Ld.r, light1.Ld.g, light1.Ld.b, light1.Ld.a);
    glUniform4f(LsLocation1, light1.Ls.r, light1.Ls.g, light1.Ls.b, light1.Ls.a);
    glUniform3f(lightPositionLocation1, light1.lightPosition_worldspace.x,
        light1.lightPosition_worldspace.y, light1.lightPosition_worldspace.z);
    glUniform1f(lightPowerLocation1, light1.power);

    //upload light 5
    glUniform4f(LaLocation5, light5.La.r, light5.La.g, light5.La.b, light5.La.a);
    glUniform4f(LdLocation5, light5.Ld.r, light5.Ld.g, light5.Ld.b, light5.Ld.a);
    glUniform4f(LsLocation5, light5.Ls.r, light5.Ls.g, light5.Ls.b, light5.Ls.a);
    glUniform3f(lightPositionLocation5, light5.lightPosition_worldspace.x,
        light5.lightPosition_worldspace.y, light5.lightPosition_worldspace.z);
    glUniform1f(lightPowerLocation5, light5.power);

}


void createContext() {
	
    shaderProgram = loadShaders(
        "StandardShading.vertexshader",
        "StandardShading.fragmentshader");

    //pointers to uniform variables
   
     // get pointers to uniforms
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");

    KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
    KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
    KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
    NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");

    LaLocation1 = glGetUniformLocation(shaderProgram, "light1.La");
    LdLocation1 = glGetUniformLocation(shaderProgram, "light1.Ld");
    LsLocation1 = glGetUniformLocation(shaderProgram, "light1.Ls");
    lightPositionLocation1 = glGetUniformLocation(shaderProgram, "light1.lightPosition_worldspace");
    lightPowerLocation1 = glGetUniformLocation(shaderProgram, "light1.power");

    LaLocation5 = glGetUniformLocation(shaderProgram, "light5.La");
    LdLocation5 = glGetUniformLocation(shaderProgram, "light5.Ld");
    LsLocation5 = glGetUniformLocation(shaderProgram, "light5.Ls");
    lightPositionLocation5 = glGetUniformLocation(shaderProgram, "light5.lightPosition_worldspace");
    lightPowerLocation5 = glGetUniformLocation(shaderProgram, "light5.power");
   
    
   diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
   specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");

   diffuseTextureCube = loadSOIL("models/Cube_Diffuse.jpg");
   diffuseTextureFloor = loadSOIL("models/StoneBlockRealistic_Diffuse.jpg");
   diffuseTextureEarth = loadSOIL("models/Earth_Diffuse");
   diffuseTextureJupiter = loadSOIL("models/Jupiter_Diffuse");

    useText = glGetUniformLocation(shaderProgram, "useTexture");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    cubeL = 4;
    box = new Box(8);

    cube = new Cube(vec3(4, 0, 4), vec3(0, 0, 0), cubeL, 0.1);

   // gun = new Gun(*cube);

    cubeFloor = new Cube(vec3(4, -0.05, 4),vec3(0, 0, 0), 8, 0.1);
    
    sphere = new Sphere(vec3(4, 4, 4), vec3(0, -8, 0), 0.2, 10);
    sphere1 = new Sphere(vec3(-100, -100, -100), vec3(0, 0, 0), 0.2, 10);
    
    cubeBack = new Cube(vec3(4,4,-0.05), vec3(0, 0, 0), 8, 0.1);
    cubeLeft= new Cube(vec3(-0.05, 4, 4), vec3(0, 0, 0), 8, 0.1);
    cubeRight = new Cube(vec3(8.05, 4, 4), vec3(0, 0, 0), 8, 0.1);
   
	//block = new Block(vec3(0, 8, 7.5));
    blockMesh = new BlockMesh(&shaderProgram);
    blockMesh->createMesh(1);

    bulls = 0;
    for (int i = 0; i < 20; i++) {
        bullets[i] = new Sphere(vec3(0, 0, 0), vec3(0, 0, 0), 0, 0);
    }
}

void free() {
    delete cube1;
    delete cube;
    delete sphere;
    delete box;
	delete block;
    delete cubeBack;
    delete cubeFloor;
    delete cubeLeft;
    delete cubeRight;
  //  delete woodblock;
    delete blockMesh;
   // delete bullets;

    glDeleteTextures(1, &diffuseTextureCube);

    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop() {
    float lastTimeShot = 0;
    float lastTimeSwap = 0;
    bulls = 0;
    float t = glfwGetTime();
    vec3 lightPos = vec3(10, 10, 10);
    camera->position = glm::vec3(box->size / 2, box->size / 2, 20);
	int lives = 3;

    do {
        // calculate dt
        float currentTime = glfwGetTime();
        //float dt = currentTime - t;
		float dt = 0.001f;
      
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //cube movement
        

    // Move forward
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            if (cube->x.z > 1.5) {
               cube->x = cube->x + vec3(0,0,-0.01);
            }
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            if (cube->x.z < 7) {
                cube->x = cube->x + vec3(0, 0, +0.01);
            }
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            if (cube->x.x > 1) {
                cube->x = cube->x + vec3(-0.01, 0, 0);
            }
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            if (cube->x.x < 7) {
                cube->x = cube->x + vec3(0.01, 0, 0);
            }
        }
        //shoot
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            if (bulls < 20 && currentTime-lastTimeShot>0.5) {
              bullets[bulls]= new Sphere(cube->x+vec3(0,0.1,0), vec3(0, 10, 0), 0.1, 20);
              bulls++;
              lastTimeShot = glfwGetTime();
            }
        }
        //change cube
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            if (currentTime - lastTimeSwap > 0.1) {
                if (cube->l == 4) {
                    cubeL = 2;
                }
                else if (cube->l == 2) {
                    cubeL = 0.8;
                }
                else{
                    cubeL = 4;
                }
                lastTimeSwap = glfwGetTime();
                    cube->l = cubeL;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            if (sphere1->x == vec3(-100,-100,-100)) {
                sphere1 = new Sphere(vec3(4, 4, 4), vec3(0, -8, 0), 0.2, 10);
            }
        }


        for (int i = 0; i < 20; i++) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    for (int z = 0; z < 8; z++) {
                        if (handleBlockSphereCollision(*blockMesh->blockMesh[x][y][z], *bullets[i], 1) == true && bullets[i]->v != vec3(0,0,0)) {
                            bulls--;
                            bullets[i] = new Sphere(vec3(-100, -100, -100), vec3(0, 0, 0), 0, 0);

                            blockMesh->blockMesh[x][y][z]->hits--;
                            if (blockMesh->blockMesh[x][y][z]->hits == 0) {
                                blockMesh->blockMesh[x][y][z] = new Block(vec3(0, 0, 0));
                                blockMesh->blockMesh[x][y][z]->type = -1;
                                blockMesh->count--;
                                blockMesh->justGotHit = 1;
                            }
                      }
                    }
                }
          }
           
        }


        // camera
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        //light
        uploadLight(light1, light2, light3, light4, light5);

        //material
    //  uploadMaterial(wood);
      
       
       /*
        //light
        glUniform3f(lightLocation, lightPos.x, lightPos.y, lightPos.z); // light
        glUniform4f(lightColour, 145.0, 3.0, 3.0, 1.0);
        glUniform1d(lightpow, 100.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        */
        if (blockMesh->justGotHit == 1) {
            swapLights();
        }
        
        for (int i = 0; i < bulls ; i++) {
            if (bullets[i]->x.y > 8) {
                bulls--;
                bullets[i] = new Sphere(vec3(-100, -100, -100), vec3(0, 0, 0), 0, 0);
            }
            bullets[i]->update(t, dt);
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &bullets[i]->modelMatrix[0][0]);
            bullets[i]->draw();
        }
        // box
        box->update(t, dt);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box->modelMatrix[0][0]);
        box->draw();
        /*/
		sphere->forcing = [&](float t, const vector<float>& y)->vector<float> {
            vector<float> f(6, 0.0f);
            f[1] = -sphere->m*g;
            return f;
        };
		/*/
        sphere->update(t, dt);
       // std::cout << glm::length(sphere->v)<<std::endl;
      //  glUniform1i(useText, 0);
       
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sphere->modelMatrix[0][0]);
        sphere->draw();

        sphere1->update(t, dt);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sphere1->modelMatrix[0][0]);
        sphere1->draw();

		cube->update(t, dt);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureCube);
      //  glUniform1i(useText, 1);
        glUniform1i(diffuseColorSampler, 0);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cube->modelMatrix[0][0]);
		cube->draw();

        cubeFloor->update(t, dt);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureFloor);
        glUniform1i(diffuseColorSampler, 0);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cubeFloor->modelMatrix[0][0]);
        cubeFloor->draw();

        cubeBack->update2(t, dt);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureFloor);
        //  glUniform1i(useText, 1);
        glUniform1i(diffuseColorSampler, 0);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cubeBack->modelMatrix[0][0]);
        cubeBack->draw();

        cubeLeft->update1(t, dt);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureFloor);
        //  glUniform1i(useText, 1);
        glUniform1i(diffuseColorSampler, 0);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cubeLeft->modelMatrix[0][0]);
        cubeLeft->draw();

        cubeRight->update1(t, dt);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureFloor);
        //  glUniform1i(useText, 1);
        glUniform1i(diffuseColorSampler, 0);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cubeRight->modelMatrix[0][0]);
        cubeRight->draw();


    //  gun->updateBulets();
        /*
		block->update(t, dt);
        block->bind();
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &block->modelMatrix[0][0]);
		block->draw();

        */
    //    glUniform1i(useText, 1);

        blockMesh->update();
        blockMesh->draw();
        blockMesh->justGotHit =0;
         
        
        /*
        woodblock->update(t, dt);
        woodblock->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTextureWoodBlock);
        glUniform1i(useText, 1);
        glUniform1i(diffuseColorSampler, 0);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &woodblock->modelMatrix[0][0]);
        */
        /*
        glUniform1d(Ns, 27.8974);
        glUniform4f(Ks, 0.992157, 0.941176, 0.807843, 1);
        glUniform4f(Kd, 0.780392, 0.568627, 0.113725, 1);
        glUniform4f(Ka, 0.329412, 0.223529, 0.027451, 1);
        */
       // woodblock->draw();

        t += dt;

		
		handleBoxSphereCollision(*box, *sphere);
		handleCubeSphereCollision(*cube, *sphere);
        handleMeshBlockSphereCollision(*blockMesh, *sphere,0);
        
        handleBoxSphereCollision(*box, *sphere1);
        handleCubeSphereCollision(*cube, *sphere1);
        handleMeshBlockSphereCollision(*blockMesh, *sphere1, 0);

      
        if (sphere1->x.y < cube->x.y) {
            sphere1 = new Sphere(vec3(-100, -100, -100), vec3(0, 0, 0), 0.2, 10);
        }
        if (lives > 0) {
            if (sphere->x.y < cube->x.y) {
                sphere = new Sphere(vec3(4, 4, 4), vec3(0, -8, 0), 0.2, 10);

                lives = lives - 1;

            }
        }
        else {
            break;
        }
      
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}

void initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    // glFrontFace(GL_CCW);

    // enable point size when drawing points
    glEnable(GL_PROGRAM_POINT_SIZE);

  

    // enable textures
    glEnable(GL_TEXTURE_2D);

    // Log
    logGLParameters();

    // Create camera
    camera = new Camera(window);
}

int main(void) {
    try {
        initialize();
        createContext();
        mainLoop();
        free();
    } catch (exception& ex) {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}
