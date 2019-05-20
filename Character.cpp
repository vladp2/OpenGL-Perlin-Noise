#include <iostream>
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

#include "Character.h"
#include "Algorithms.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Camera.h"
#include "Render.h"


using namespace std;
using namespace glm;


extern vector<Model> models;
extern vector<Texture> textures;


Character::Character() {
    body.resize(6);

    pos = vec3(0.0f, 0.0f, 0.0f);

    box.center = pos;

    box.rotationMatrix[0] = vec3(body[0].modelMatrix[0][0], body[0].modelMatrix[0][1], body[0].modelMatrix[0][2]);
    box.rotationMatrix[1] = vec3(body[0].modelMatrix[1][0], body[0].modelMatrix[1][1], body[0].modelMatrix[1][2]);
    box.rotationMatrix[2] = vec3(body[0].modelMatrix[2][0], body[0].modelMatrix[2][1], body[0].modelMatrix[2][2]);

    box.halfLength.x = 0.6f;
    box.halfLength.y = 3.0f;
    box.halfLength.z = 0.5f;
}


void Character::Init() {
    body[0].Create("skelet_head", "blue", {0.0f, 0.0f, 0.0f});
    body[1].Create("skelet_torso", "red", {0.0f, 0.0f, 0.0f});
    body[2].Create("skelet_larm", "blue", {0.0f, 0.0f, 0.0f});
    body[3].Create("skelet_rarm", "blue", {0.0f, 0.0f, 0.0f});
    body[4].Create("skelet_lleg", "blue", {0.0f, 0.0f, 0.0f});
    body[5].Create("skelet_rleg", "blue", {0.0f, 0.0f, 0.0f});
}


void NormalizeAngle(float& angle) {
    float limit = 90.0f;

    if (angle > limit)
        angle = angle - (int(angle) / int(limit)) * limit;
}


void Character::Update() {
    pos = vec3(body[0].modelMatrix[3][0], body[0].modelMatrix[3][1] + box.halfLength.y, body[0].modelMatrix[3][2]);

    modelMatrix = mat4(
        body[0].modelMatrix[0][0], body[0].modelMatrix[0][1], body[0].modelMatrix[0][2], body[0].modelMatrix[0][3],
        body[0].modelMatrix[1][0], body[0].modelMatrix[1][1], body[0].modelMatrix[1][2], body[0].modelMatrix[1][3],
        body[0].modelMatrix[2][0], body[0].modelMatrix[2][1], body[0].modelMatrix[2][2], body[0].modelMatrix[2][3],
        body[0].modelMatrix[3][0], body[0].modelMatrix[3][1] + box.halfLength.y, body[0].modelMatrix[3][2], body[0].modelMatrix[3][3]
    );

    UpdateOBB();
}


void Character::UpdateOBB() {
    box.center = pos;

    box.rotationMatrix[0] = vec3(body[0].modelMatrix[0][0], body[0].modelMatrix[0][1], body[0].modelMatrix[0][2]);
    box.rotationMatrix[1] = vec3(body[0].modelMatrix[1][0], body[0].modelMatrix[1][1], body[0].modelMatrix[1][2]);
    box.rotationMatrix[2] = vec3(body[0].modelMatrix[2][0], body[0].modelMatrix[2][1], body[0].modelMatrix[2][2]);
}
