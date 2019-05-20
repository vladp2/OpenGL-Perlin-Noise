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
#include "LoadVertex.h"
#include "Map.h"


using namespace std;
using namespace glm;


extern Map _map;

extern Texture tex[100];
extern GLuint texCount;

extern Model model[100];
extern GLuint modelCount;


static const GLfloat UV[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};



bool Object::Init(std::string modelName, std::string textureName, vec3 pos) {
    Model _model;
    for (GLuint i = 1; i <= modelCount; ++i)
        if (model[i].name == modelName) {
            _model = model[i];
            break;
        }

    Texture texture;
    for (GLuint i = 1; i <= texCount; ++i)
        if (tex[i].name == textureName) {
            texture = tex[i];
            break;
        }


    this->vertexCount = _model.size;

    this->GetBoundBox(_model);


    glGenVertexArrays(1, &this->vertexAO);
    glBindVertexArray(this->vertexAO);


    GLuint modelBuffer;

    glGenBuffers(1, &modelBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(float) * 3, _model.data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, this->textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    GLuint UVBuffer;

    glGenBuffers(1, &UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UV), &UV[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


    this->modelMatrix = mat4(1.0f);
    this->Proj(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    this->Trans(pos);

    this->active = true;

    return true;
}


void Object::GetBoundBox(Model _model) {
    this->minVertex = vec3(999999.0f);
    this->maxVertex = vec3(-999999.0f);

    for (GLuint i = 0; i < this->vertexCount; i += 3) {
        if (_model.data[i] < this->minVertex.x)
            this->minVertex.x = _model.data[i];
        if (_model.data[i + 1] < this->minVertex.y)
            this->minVertex.y = _model.data[i + 1];
        if (_model.data[i + 2] < this->minVertex.z)
            this->minVertex.z = _model.data[i + 2];

        if (_model.data[i] > this->maxVertex.x)
            this->maxVertex.x = _model.data[i];
        if (_model.data[i + 1] > this->maxVertex.y)
            this->maxVertex.y = _model.data[i + 1];
        if (_model.data[i + 2] > this->maxVertex.z)
            this->maxVertex.z = _model.data[i + 2];
    }
}



void Object::Trans(vec3 transVector) {
    this->modelMatrix = translate(modelMatrix, transVector);
}


void Object::Rotate(GLfloat rotAngle, vec3 rotVector) {
    if (rotVector == vec3(0.0f))
        return;

    this->modelMatrix = rotate(modelMatrix, radians(rotAngle), rotVector);
}


void Object::Scale(vec3 scaleVector) {
    this->modelMatrix = scale(modelMatrix, scaleVector);
}


void Object::Proj(float fov, float aspectRatio, float minDist, float maxDist){
    this->projMatrix = perspective(radians(fov), aspectRatio, minDist, maxDist);
}


void Object::Move(vec3 eye, vec3 center, vec3 up) {
    this->viewMatrix = lookAt(eye, center, up);

    this->uniformMatrix = this->projMatrix * this->viewMatrix * this->modelMatrix;

    this->uniformHandle = glGetUniformLocation(_map.programId, "uniformMatrix");
    glUniformMatrix4fv(this->uniformHandle, 1, GL_FALSE, &this->uniformMatrix[0][0]);
}


void Object::Draw() {
    glBindVertexArray(this->vertexAO);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Object::Delete() {
    this->active = false;
}
