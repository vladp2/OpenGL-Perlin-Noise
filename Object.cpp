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

#include "Object.h"
#include "Model.h"
#include "Texture.h"
#include "Render.h"
#include "Map.h"
#include "Camera.h"


using namespace std;
using namespace glm;


extern vector<uint> VAOs;
extern vector<uint> VBOs;
extern vector<uint> IBOs;
extern vector<uint> textureIDs;
extern vector<Texture> gTextures;
extern vector<Model> gModels;
extern vector<Object> gObjects;


void Object::Create(const string& modelName, const string& textureName, const vec3 pos) {
    modelID = Model::nameMap[modelName];
    textureID = Texture::nameMap[textureName];

    positionMatrix = mat4(1.0f);
    rotationMatrix = mat4(1.0f);
    scalingMatrix = mat4(1.0f);
    modelMatrix = mat4(1.0f);

    SetPos(pos);
    SetOBB();
}


void Object::Create(const uint modelID, const uint textureID, const vec3 pos) {
    this->modelID = modelID;
    this->textureID = textureID;

    modelMatrix = mat4(1.0f);
    positionMatrix = mat4(1.0f);
    rotationMatrix = mat4(1.0f);
    scalingMatrix = mat4(1.0f);
    modelMatrix = mat4(1.0f);

    SetPos(pos);
    SetOBB();
}


void Object::SetPos(vec3 pos) {
    if (gModels[modelID].name == "crosshair") {
        Place(vec3(0.0f, 0.0f, 0.0f));
        Scale(vec3(0.001f));
        return;
    }

    Place(pos);
}


void Object::SetOBB() {
    vec3 minVertex = gModels[modelID].minVertex;
    vec3 maxVertex = gModels[modelID].maxVertex;

    box.center = vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

    box.rotationMatrix[0] = vec3(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
    box.rotationMatrix[1] = vec3(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
    box.rotationMatrix[2] = vec3(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);

    box.halfLength.x = abs(maxVertex.x - minVertex.x) / 2.0f;
    box.halfLength.y = abs(maxVertex.y - minVertex.y) / 2.0f;
    box.halfLength.z = abs(maxVertex.z - minVertex.z) / 2.0f;
}


void Object::ResetOBB() {
    box.center = vec3(0.0f, 0.0f, 0.0f);

    box.rotationMatrix[0] = vec3(1.0f, 0.0f, 0.0f);
    box.rotationMatrix[1] = vec3(0.0f, 1.0f, 0.0f);
    box.rotationMatrix[2] = vec3(0.0f, 0.0f, 1.0f);

    box.halfLength = vec3(1.0f, 1.0f, 1.0f);
}


void Object::UpdateOBB() {
    box.center.x = positionMatrix[3].x;
    box.center.y = positionMatrix[3].y;
    box.center.z = positionMatrix[3].z;

    box.rotationMatrix[0] = vec3(rotationMatrix[0].x, rotationMatrix[0].y, rotationMatrix[0].z);
    box.rotationMatrix[1] = vec3(rotationMatrix[1].x, rotationMatrix[1].y, rotationMatrix[1].z);
    box.rotationMatrix[2] = vec3(rotationMatrix[2].x, rotationMatrix[2].y, rotationMatrix[2].z);

    box.halfLength.x = scalingMatrix[0].x;
    box.halfLength.y = scalingMatrix[1].y;
    box.halfLength.z = scalingMatrix[2].z;
}


void Object::Place(const vec3 position) {
    positionMatrix[3].x = position.x;
    positionMatrix[3].y = position.y;
    positionMatrix[3].z = position.z;

    box.center.x = positionMatrix[3].x;
    box.center.y = positionMatrix[3].y;
    box.center.z = positionMatrix[3].z;

    modelMatrix = positionMatrix * rotationMatrix * scalingMatrix;
}


void Object::Rotate(const vec3 rotation) {
    if (rotation.x)
        rotationMatrix = glm::rotate(rotationMatrix, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

    if (rotation.y)
        rotationMatrix = glm::rotate(rotationMatrix, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));

    if (rotation.z)
        rotationMatrix = glm::rotate(rotationMatrix, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));

    box.rotationMatrix[0] = vec3(rotationMatrix[0].x, rotationMatrix[0].y, rotationMatrix[0].z);
    box.rotationMatrix[1] = vec3(rotationMatrix[1].x, rotationMatrix[1].y, rotationMatrix[1].z);
    box.rotationMatrix[2] = vec3(rotationMatrix[2].x, rotationMatrix[2].y, rotationMatrix[2].z);

    modelMatrix = positionMatrix * rotationMatrix * scalingMatrix;
}


void Object::Scale(const vec3 scaling) {
    scalingMatrix[0].x = scaling.x;
    scalingMatrix[1].y = scaling.y;
    scalingMatrix[2].z = scaling.z;

    box.halfLength.x = scalingMatrix[0].x;
    box.halfLength.y = scalingMatrix[1].y;
    box.halfLength.z = scalingMatrix[2].z;

    modelMatrix = positionMatrix * rotationMatrix * scalingMatrix;
}


void Object::Face(const vec3 angles) {
    rotationMatrix = mat4(1.0f);

    Rotate(angles);
}


vec3 Object::GetEulerAngles() const {
    return vec3(
        -atan2(modelMatrix[2][1], modelMatrix[2][2]),
        -atan2(-modelMatrix[2][0], sqrt(modelMatrix[2][1] * modelMatrix[2][1] + modelMatrix[2][2] * modelMatrix[2][2])),
        -atan2(modelMatrix[1][0], modelMatrix[0][0])
    );
}


uint Object::GetModelID() const {
    return modelID;
}


void Object::SetModelID(const uint ID) {
    modelID = ID;
}


void Object::SetTextureID(const uint ID) {
    textureID = ID;
}


uint Object::GetTextureID() {
    return textureID;
}
