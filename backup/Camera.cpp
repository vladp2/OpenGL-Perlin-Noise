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

#include "Camera.h"
#include "Window.h"
#include "Object.h"

using namespace std;
using namespace glm;


extern Window window;


void Camera::Init() {
    this->mouse = {window.width / 2, window.height / 2};
    this->mouseSpeed = 0.3f;
    this->keySpeed = 10.0f;

    this->horizAngle = 3.14f;
    this->vertAngle = 0.0f;
    this->fov = 45.0f;

    this->pos = vec3(0.0f, 0.0f, 5.0f);
    this->dir = vec3(0.0f, 0.0f, 0.0f);
    this->right = vec3(0.0f, 0.0f, 0.0f);
    this->up = vec3(0.0f, 0.0f, 0.0f);
}


void Camera::GetDeltaTime() {
    float currentFrame = glfwGetTime();

    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
}


bool Camera::CheckMouse() {
    glfwGetCursorPos(window.handle, &this->mouse.x, &this->mouse.y);

    this->horizAngle += this->mouseSpeed * this->deltaTime * float(window.width / 2 - this->mouse.x);
    this->vertAngle += this->mouseSpeed * this->deltaTime * float(window.height / 2 - this->mouse.y);

    this->dir = vec3(cos(this->vertAngle) * sin(this->horizAngle),
                   sin(this->vertAngle),
                   cos(this->vertAngle) * cos(this->horizAngle));

    this->right = vec3(sin(this->horizAngle - 3.14f / 2.0f),
                     0,
                     cos(this->horizAngle - 3.14f / 2.0f));

    this->up = cross(this->right, this->dir);

    glfwSetCursorPos(window.handle, window.width / 2, window.height / 2);

    return true;
}


bool Camera::CheckKeys() {
    if (glfwGetKey(window.handle, GLFW_KEY_W) == GLFW_PRESS) {
        this->pos += this->dir * this->deltaTime * this->keySpeed;
    }

    if (glfwGetKey(window.handle, GLFW_KEY_S) == GLFW_PRESS) {
        this->pos -= this->dir * this->deltaTime * this->keySpeed;
    }

    if (glfwGetKey(window.handle, GLFW_KEY_D) == GLFW_PRESS) {
        this->pos += this->right * this->deltaTime * this->keySpeed;
    }

    if (glfwGetKey(window.handle, GLFW_KEY_A) == GLFW_PRESS) {
        this->pos -= this->right * this->deltaTime * this->keySpeed;
    }

    if (glfwGetKey(window.handle, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->pos += this->up * this->deltaTime * this->keySpeed;
    }

    if (glfwGetKey(window.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        this->pos -= this->up * this->deltaTime * this->keySpeed;
    }

    return true;
}
