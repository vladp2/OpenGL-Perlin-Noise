#include <iostream>
#include <ctime>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Map.h"
#include "FileManager.h"
#include "Camera.h"
#include "Object.h"
#include "Globals.h"
#include "Window.h"
#include "Buttons.h"

using namespace std;
using namespace glm;



extern Window window;

extern Camera cam;

extern Buttons buttons;

extern Model model[100];
extern GLuint modelCount;

extern Texture tex[100];
extern GLuint texCount;

extern Object obj[100];
extern GLuint objCount;



void Map::GenObjects() {
    obj[++objCount].Init("cube", "cube_red", vec3(2.0f));

    obj[objCount].modelMatrix = mat4(1.0f);
    obj[objCount].Proj(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    obj[objCount].Trans(vec3(2.0f));

    cout << obj[objCount].uniformMatrix[3][0] << '\n';
    cout << obj[objCount].uniformMatrix[3][1] << '\n';
    cout << obj[objCount].uniformMatrix[3][2] << '\n';
    cout << obj[objCount].uniformMatrix[3][3] << '\n';
}


//crosshair.Proj(45.0f, 4.0f / 3.0f, 5.0f, 100.0f);



bool Map::LoadShaders() {
    std::string vertexFile = "Game/shaders/VertexShader.txt";
    std::string fragmentFile = "Game/shaders/FragmentShader.txt";

    this->programId = ReadShaders(vertexFile, fragmentFile);

    if (!programId)
        return false;

    glUseProgram(this->programId);

    return true;
}


bool Map::LoadModels(std::string verticesFile) {
    std::ifstream fileStream(verticesFile, std::ios::in);

	if (!fileStream.is_open()) {
        std::cout << "Impossible to open " << verticesFile << '\n';
		return false;
	}

	std::string modelName;
    while (fileStream >> modelName) {
        ++modelCount;
        model[modelCount].name = modelName;

        if (!ReadModel(modelName))
            return false;
    }



    fileStream.close();

    return true;
}


bool Map::LoadTextures(std::string texturesFile) {
    std::ifstream fileStream(texturesFile, std::ios::in);

	if (!fileStream.is_open()) {
        std::cout << "Impossible to open " << texturesFile << '\n';
		return false;
	}

	std::string textureName;
    while (fileStream >> textureName) {
        ++texCount;
        tex[texCount].name = textureName;

        if (!ReadTexture(textureName))
            return false;
    }

    fileStream.close();

    return true;
}


void Map::EnableZBuffer() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void Map::DrawObjects() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (GLuint i = 1; i <= objCount; ++i)
        if (obj[i].active) {
            obj[i].Move(cam.pos, cam.pos + cam.dir, cam.up);
            obj[i].Draw();
        }

    //crosshair.Move(vec3(0.0f, 0.0f, 5.1f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    //crosshair.Draw();
}


void Map::CheckKeys() {
    if (buttons.CheckMouse(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS) ||
        buttons.CheckMouse(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS)) {

        this->targetObj = this->RayCast(); // verific daca se afla un obiect pe directia tintei

        if (this->targetObj) // daca am gasit cel mai apropiat obiect din fata tintei
            this->targetObjDist = obj[this->targetObj].uniformMatrix[3][2]; // salvez distanta fata de obiect
    }

    if (this->targetObj) {
        if (buttons.CheckMouse(GLFW_MOUSE_BUTTON_LEFT, GLFW_REPEAT)) { // mut obiectul
            this->MoveObject();

            if (buttons.CheckMouse(GLFW_MOUSE_BUTTON_RIGHT, GLFW_REPEAT)) // mut si rotesc obiectul
                obj[targetObj].Rotate(1.0f, vec3(1.0f, 0.0f, 0.0f));
        }
        else
            if (buttons.CheckMouse(GLFW_MOUSE_BUTTON_RIGHT, GLFW_REPEAT)) // rotesc obiectul
                obj[targetObj].Rotate(1.0f, vec3(1.0f, 0.0f, 0.0f));
        else
            this->targetObj = 0; // nu mai folosesc obiectul
    }

    if (buttons.CheckKeybd(GLFW_KEY_G, GLFW_PRESS)) {
        this->targetObj = this->RayCast();

        if (this->targetObj)
            this->DeleteObject();
    }

    if (buttons.CheckKeybd(GLFW_KEY_F, GLFW_PRESS))
        this->PlaceObject("cube", "cube_blue", 10.0f);
}


void Map::PlaceObject(string modelName, string modelTexture, float dist) {
    vec3 pos = vec3(cam.pos.x + cam.dir.x * dist,
                       cam.pos.y + cam.dir.y * dist,
                       cam.pos.z + cam.dir.z * dist);

    obj[++objCount].Init(modelName, modelTexture, pos);
}


GLuint Map::RayCast() {
    GLuint target = 0;
    float minDist = 999999.0f;

    for (GLuint i = 1; i <= objCount; ++i) {
        float xDist = abs(obj[i].minVertex.x - obj[i].maxVertex.x);
        float yDist = abs(obj[i].minVertex.y - obj[i].maxVertex.y);

        if (abs(obj[i].uniformMatrix[3][0]) < xDist)
        if (abs(obj[i].uniformMatrix[3][1]) < yDist)
            if (obj[i].uniformMatrix[3][2] < minDist) {
                target = i;
                minDist = obj[i].uniformMatrix[3][2];
            }

    }

    return target;
}


void Map::MoveObject() {
    vec3 newPos = vec3(cam.pos.x + cam.dir.x * this->targetObjDist,
                       cam.pos.y + cam.dir.y * this->targetObjDist,
                       cam.pos.z + cam.dir.z * this->targetObjDist);

    obj[targetObj].modelMatrix[3][0] = newPos[0];
    obj[targetObj].modelMatrix[3][1] = newPos[1];
    obj[targetObj].modelMatrix[3][2] = newPos[2];
}


void Map::DeleteObject() {
    obj[this->targetObj].active = false;

    for (GLuint i = targetObj; i < objCount; ++i)
        obj[i] = obj[i + 1];
    --objCount;
}
