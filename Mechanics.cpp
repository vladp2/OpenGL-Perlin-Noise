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

#include "Mechanics.h"
#include "Buttons.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Collision.h"
#include "Character.h"


#define ef else if


using namespace std;
using namespace glm;


extern vector<Model> models;
extern Object player;


double Mechanics::deltaTime;
double Mechanics::lastFrame;
float Mechanics::moveSpeed;
bool Mechanics::moveForward;
bool Mechanics::moveBackward;
bool Mechanics::moveLeft;
bool Mechanics::moveRight;
bool Mechanics::moveUp;
bool Mechanics::moveDown;
bool Mechanics::isMoving;
bool Mechanics::isJumping;
bool Mechanics::onGround;
bool Mechanics::onObject;
float Mechanics::force;
vec3 Mechanics::direction;


void Mechanics::CalcDeltaTime() {
    double currentFrame = glfwGetTime();

    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}


void Mechanics::MoveForward(const float speed) {
    vec3 _move = {
        Camera::GetDir().x * speed,
        Camera::GetDir().y * speed,
        Camera::GetDir().z * speed
    };

    player.positionMatrix[3][0] += _move.x;
    player.positionMatrix[3][1] += _move.y;
    player.positionMatrix[3][2] += _move.z;
}


void Mechanics::MoveBackward(const float speed) {
    vec3 _move = {
        -Camera::GetDir().x * speed,
        -Camera::GetDir().y * speed,
        -Camera::GetDir().z * speed
    };

    player.positionMatrix[3][0] += _move.x;
    player.positionMatrix[3][1] += _move.y;
    player.positionMatrix[3][2] += _move.z;
}


void Mechanics::MoveLeft(const float speed) {
    vec3 _move = {
        -Camera::GetRight().x * speed,
        -Camera::GetRight().y * speed,
        -Camera::GetRight().z * speed
    };

    player.positionMatrix[3][0] += _move.x;
    player.positionMatrix[3][1] += _move.y;
    player.positionMatrix[3][2] += _move.z;
}


void Mechanics::MoveRight(const float speed) {
    vec3 _move = {
        Camera::GetRight().x * speed,
        Camera::GetRight().y * speed,
        Camera::GetRight().z * speed
    };

    player.positionMatrix[3][0] += _move.x;
    player.positionMatrix[3][1] += _move.y;
    player.positionMatrix[3][2] += _move.z;
}


void Mechanics::MoveUp(const float speed) {
    vec3 _move = {
        Camera::GetUp().x * speed,
        Camera::GetUp().y * speed,
        Camera::GetUp().z * speed,
    };

    player.positionMatrix[3][0] += _move.x;
    player.positionMatrix[3][1] += _move.y;
    player.positionMatrix[3][2] += _move.z;
}


void Mechanics::MoveDown(const float speed) {
    vec3 _move = {
        -Camera::GetUp().x * speed,
        -Camera::GetUp().y * speed,
        -Camera::GetUp().z * speed,
    };

    player.positionMatrix[3][0] += _move.x;
    player.positionMatrix[3][1] += _move.y;
    player.positionMatrix[3][2] += _move.z;
}


void Mechanics::CheckKeys() {
    moveForward = false;
    moveBackward = false;
    moveLeft = false;
    moveRight = false;
    moveUp = false;
    moveDown = false;

    if (Buttons::CheckKeybd(GLFW_KEY_W, GLFW_REPEAT)) {
        moveForward = true;
    }
    if (Buttons::CheckKeybd(GLFW_KEY_S, GLFW_REPEAT)) {
        moveBackward = true;
    }
    if (Buttons::CheckKeybd(GLFW_KEY_A, GLFW_REPEAT)) {
        moveLeft = true;
    }
    if (Buttons::CheckKeybd(GLFW_KEY_D, GLFW_REPEAT)) {
        moveRight = true;
    }
    if (Buttons::CheckKeybd(GLFW_KEY_SPACE, GLFW_REPEAT)) {
        moveUp = true;
    }
    if (Buttons::CheckKeybd(GLFW_KEY_LEFT_CONTROL, GLFW_REPEAT)) {
        moveDown = true;
    }
    if (Buttons::CheckKeybd(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT)) {
        moveSpeed = 150.0f;
    }
    else
        moveSpeed = 15.0f;
}


void Mechanics::Move() {
    CalcDeltaTime();

    if (moveForward != moveBackward) {
        if (moveForward)
            MoveForward(moveSpeed * deltaTime);
        else
            MoveBackward(moveSpeed * deltaTime);
    }

    if (moveLeft != moveRight) {
        if (moveLeft)
            MoveLeft(moveSpeed * deltaTime);
        else
            MoveRight(moveSpeed * deltaTime);
    }

    if (moveUp != moveDown) {
        if (moveUp)
            MoveUp(3.0f * moveSpeed * deltaTime);
        else
            MoveDown(moveSpeed * deltaTime);
    }

    ApplyGravity();

    Rotate();
}


void Mechanics::Rotate() {
    //player.Face(vec3(0, Camera::GetYaw() * RAD + 180.0f, 0));

    player.rotationMatrix[0].x = Camera::GetRight().x;
    player.rotationMatrix[0].y = Camera::GetRight().y;
    player.rotationMatrix[0].z = Camera::GetRight().z;
    player.rotationMatrix[1].x = Camera::GetUp().x;
    player.rotationMatrix[1].y = Camera::GetUp().y;
    player.rotationMatrix[1].z = Camera::GetUp().z;
    player.rotationMatrix[2].x = -Camera::GetDir().x;
    player.rotationMatrix[2].y = -Camera::GetDir().y;
    player.rotationMatrix[2].z = -Camera::GetDir().z;
}


void Mechanics::ApplyGravity() {
    if (!onGround && !onObject) {
        force = 0.00002f + force * 1.002f;
        player.positionMatrix[3].y -= force;
    }
    else {
        force = 0.0f;
    }
}


double Mechanics::GetDeltaTime() {
    return deltaTime;
}


bool Mechanics::IsOnGround() {
    return onGround;
}


bool Mechanics::IsOnObject() {
    return onObject;
}


void Mechanics::SetOnGround(const bool val) {
    onGround = val;
}


void Mechanics::SetOnObject(const bool val) {
    onObject = val;
}
