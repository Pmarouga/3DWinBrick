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
GLuint LaLocation, LdLocation, LsLocation, lightPositionLocation, lightPowerLocation;
// material properties
GLuint KdLocation, KsLocation, KaLocation, NsLocation;

//texture properties
GLuint diffuseColorSampler, specularColorSampler;
GLuint diffuseTextureWoodBlock, specularTexture;

struct Light {
    glm::vec4 La;
    glm::vec4 Ld;
    glm::vec4 Ls;
    glm::vec3 lightPosition_worldspace;
    float power;
};

struct Material {
    glm::vec4 Ka;
    glm::vec4 Kd;
    glm::vec4 Ks;
    float Ns;
};

Light light{
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec4{ 1, 1, 1, 1 },
    vec3{ 0, 4, 4 },
    20.0f
};


//moving brick
Cube* cube;
//ball
Sphere* sphere;
//gamespace
Box* box;
//hitbox
Block* block;

// tests
Cube* cube1;
WoodBlock* woodblock;
BlockMesh* blockMesh;

int useText;


void uploadMaterial(const Material& mtl) {
    glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
    glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
    glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
    glUniform1f(NsLocation, mtl.Ns);
}

void uploadLight(const Light& light) {
    glUniform4f(LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
    glUniform4f(LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
    glUniform4f(LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
    glUniform3f(lightPositionLocation, light.lightPosition_worldspace.x,
        light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
    glUniform1f(lightPowerLocation, light.power);
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
    LaLocation = glGetUniformLocation(shaderProgram, "light.La");
    LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
    LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
    lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
    lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
   
    
   diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
   specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");

    diffuseTextureWoodBlock = loadSOIL("models/WoodBlock_Diffuse.jpg");

    useText = glGetUniformLocation(shaderProgram, "useTexture");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    box = new Box(8);
    
    cube1 = new Cube(vec3(4,9,4), vec3(0,0,0), 2, 0.1);

  //  woodblock = new WoodBlock(vec3(4, 3, 2));

    cube = new Cube(vec3(4, 0, 4), vec3(0, 0, 0), 2, 0.1);
    
    sphere = new Sphere(vec3(4, 4, 4), vec3(0, -2, 0), 0.2, 10);
   
	//block = new Block(vec3(0, 8, 7.5));
    blockMesh = new BlockMesh(&shaderProgram);
    blockMesh->createMesh(1);

}

void free() {
    delete cube1;
    delete cube;
    delete sphere;
    delete box;
	delete block;
    delete woodblock;
    delete blockMesh;

    glDeleteTextures(1, &diffuseTextureWoodBlock);

    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop() {
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
               cube->x = cube->x + vec3(0, 0, -0.01);
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


        // camera
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        //light
        uploadLight(light);
      /*
        //light
        glUniform3f(lightLocation, lightPos.x, lightPos.y, lightPos.z); // light
        glUniform4f(lightColour, 145.0, 3.0, 3.0, 1.0);
        glUniform1d(lightpow, 100.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        */

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
        glUniform1i(useText, 0);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sphere->modelMatrix[0][0]);
        sphere->draw();

		cube->update(t, dt);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cube->modelMatrix[0][0]);
		cube->draw();

		block->update(t, dt);
        block->bind();
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &block->modelMatrix[0][0]);
		block->draw();

        cube1->update(t, dt);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cube1->modelMatrix[0][0]);
        cube1->draw();

        blockMesh->update();
        blockMesh->draw();
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
        woodblock->draw();

        t += dt;

		
		handleBoxSphereCollision(*box, *sphere);
		handleCubeSphereCollision(*cube, *sphere);

        if (lives > 0) {
            if (sphere->x.y < cube->x.y) {
                sphere = new Sphere(vec3(4, 4, 4), vec3(0, -2, 0), 0.2, 10);

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
