#include <iostream>
#include <ctime>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Map.h"
#include "Render.h"
#include "FileManager.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Window.h"
#include "Buttons.h"
#include "Algorithms.h"
#include "Character.h"
#include "Collision.h"


#define ef else if


using namespace std;
using namespace glm;


extern vector<Model> gModels;
extern vector<Texture> gTextures;
extern vector<Object> gObjects;
extern Object crosshair;
extern Object player;
extern vector<vector<Object>> terrain;
extern uint chunkCount;
extern uint chunkDist;
extern uint chunkLength;


int Map::targetObj;
float Map::targetObjDist;


extern Object skybox;
extern vector<mat4> matrices;


void SpawnInstanced() {
    for (uint i = 0; i < matrices.size(); ++i) {
        gObjects.push_back(Object());
        gObjects.back().Create("cube", "white", vec3(matrices[i][3].x, matrices[i][3].y, matrices[i][3].z));
    }
}


void Map::SpawnObjects() {
    SpawnInstanced();

    player.Create("player", "abstract", vec3(0, 0, 0));
    player.Scale(vec3(1, 1, 1));

    skybox.Create("skybox", "skybox", vec3(0, 0, 0));
    skybox.modelMatrix = glm::scale(mat4(1), vec3(Camera::GetFarPlane() / sqrt(3.0f) - 10.0f));

    crosshair.Create("crosshair", "yellow", vec3(0, 0, 0));
}


void Map::CheckKeys() {
    if (Buttons::CheckMouse(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS) ||
        Buttons::CheckMouse(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS) ||
        Buttons::CheckMouse(GLFW_MOUSE_BUTTON_4, GLFW_PRESS) ||
        Buttons::CheckMouse(GLFW_MOUSE_BUTTON_5, GLFW_PRESS)) {

        targetObj = Collision::RayCast(Camera::GetPos(), Camera::GetDir(), targetObjDist); // verific daca se afla un obiect pe directia data
    }

    if (targetObj != -1) {
        bool check = false;

        if (Buttons::CheckMouse(GLFW_MOUSE_BUTTON_LEFT, GLFW_REPEAT)) { // mut obiectul
            MoveObject(targetObjDist);

            check = true;
        }
        if (Buttons::CheckMouse(GLFW_MOUSE_BUTTON_RIGHT, GLFW_REPEAT)) { // rotesc obiectul
            gObjects[targetObj].Rotate(vec3(1.0f, 0.0f, 0.0f));
            matrices[targetObj] = gObjects[targetObj].modelMatrix;

            check = true;
        }

        if (Buttons::CheckMouse(GLFW_MOUSE_BUTTON_4, GLFW_REPEAT)) {
            targetObjDist -= targetObjDist <= 15.0f ? 0.0f : 0.05f;
            MoveObject(targetObjDist);

            check = true;
        }
        ef (Buttons::CheckMouse(GLFW_MOUSE_BUTTON_5, GLFW_REPEAT)) {
            targetObjDist += targetObjDist >= 100.0f ? 0.0f : 0.05f;
            MoveObject(targetObjDist);

            check = true;
        }

        if (!check)
            targetObj = -1; // nu mai folosesc obiectul
    }

    if (Buttons::CheckKeybd(GLFW_KEY_G, GLFW_PRESS)) {
        targetObj = Collision::RayCast(Camera::GetPos(), Camera::GetDir(), targetObjDist);

        if (targetObj != -1)
            DeleteObject();
    }

    if (Buttons::CheckKeybd(GLFW_KEY_F, GLFW_PRESS)) {
        SpawnObject("cube", "granite", 40);
    }
}


void Map::SpawnObject(const string modelName, const string modelTexture, const float dist) {
    vec3 pos = {
        Camera::GetPos().x + Camera::GetDir().x * dist,
        Camera::GetPos().y + Camera::GetDir().y * dist,
        Camera::GetPos().z + Camera::GetDir().z * dist
    };

    gObjects.push_back(Object());
    gObjects.back().Create(modelName, modelTexture, pos);

    matrices.push_back(gObjects.back().modelMatrix);
}


void Map::MoveObject(const float dist) {
    gObjects[targetObj].Place({
        Camera::GetPos().x + Camera::GetDir().x * dist,
        Camera::GetPos().y + Camera::GetDir().y * dist,
        Camera::GetPos().z + Camera::GetDir().z * dist
    });

    matrices[targetObj] = gObjects[targetObj].modelMatrix;
}


void Map::DeleteObject() {
    for (uint i = targetObj; i < gObjects.size() - 1; ++i)
        gObjects[i] = gObjects[i + 1];

    gObjects.pop_back();
    matrices.pop_back();
}
