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
#include "Texture.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Collision.h"
#include "Mechanics.h"
#include "Character.h"
#include "Buttons.h"


using namespace std;
using namespace glm;


extern Object player;


double Camera::mouseX = 0.0f;
double Camera::mouseY = 0.0f;
double Camera::centerX = 1337.0f;
double Camera::centerY = 1337.0f;
float Camera::speed = 1.0f;
float Camera::fov = 70.0f;
float Camera::_ratio = 4.0f / 3.0f;
float Camera::nearPlane = 0.1f; // Recomandat >= 1.0 ?
float Camera::farPlane = 2000.0f; // Recomandat < 1000 ?
vec3 Camera::pos = vec3(0.0f, 0.0f, 0.0f);
vec3 Camera::dir = vec3(0.0f, 0.0f, 0.0f);
vec3 Camera::right = vec3(0.0f, 0.0f, 0.0f);
vec3 Camera::up = vec3(0.0f, 0.0f, 0.0f);
float Camera::playerDist = 20.0f;
mat4 Camera::projMatrix = mat4(0.0f);
mat4 Camera::viewMatrix = mat4(0.0f);
float Camera::frustum[6][4];
float Camera::pitch = 1.337f;
float Camera::yaw = 1.337f;
float Camera::roll = 0.0f;


void Camera::Init() {
    dir = {
        cos(pitch) * sin(yaw),
        sin(pitch),
        cos(pitch) * cos(yaw)
    };

    right = {
        sin(yaw - PIdiv2),
        0,
        cos(yaw - PIdiv2)
    };

    up = cross(right, dir);

    centerX = double(Window::GetWidth()) / 2.0;
    centerY = double(Window::GetHeight()) / 2.0;
}


bool Camera::CheckMouse() {
    glfwGetCursorPos(Window::GetHandle(), &mouseX, &mouseY);

    return
        mouseX != centerX ||
        mouseY != centerY;
}


void Camera::CheckKeys() {
    return;

    if (Buttons::CheckKeybd(GLFW_KEY_Q, GLFW_REPEAT)) {
        Camera::Roll(0.1f);
    }

    if (Buttons::CheckKeybd(GLFW_KEY_E, GLFW_REPEAT)) {
        Camera::Roll(-0.1f);
    }
}


void Camera::MoveFPS() {
    glfwSetCursorPos(Window::GetHandle(), centerX, centerY);

    yaw += speed * (centerX - mouseX) * Mechanics::GetDeltaTime();
    pitch += speed * (centerY - mouseY) * Mechanics::GetDeltaTime();

    NormalizeAngle(yaw);
    NormalizeAngle(pitch);

    dir = {
        cos(pitch) * sin(yaw),
        sin(pitch),
        cos(pitch) * cos(yaw)
    };

    right = {
        sin(yaw - PIdiv2),
        0,
        cos(yaw - PIdiv2)
    };

    up = cross(right, dir);
}



void Camera::Move360() {
    glfwSetCursorPos(Window::GetHandle(), centerX, centerY);

    pitch += (centerY - mouseY) * Mechanics::GetDeltaTime();
    yaw += (centerX - mouseX) * Mechanics::GetDeltaTime();

    pitch /= speed;
    yaw /= speed;

    Pitch();
    Yaw();
}


void Camera::Pitch() {
    dir = normalize(dir * cos(pitch * PI) + up * sin(pitch * PI));

    up = cross(dir, right);
    up *= -1.0f;
}


void Camera::Yaw() {
    dir = normalize(dir * cos(yaw * PI) - right * sin(yaw * PI));

    right = cross(dir, up);
}


void Camera::Roll(const float speed) {
    roll = radians(roll + speed);

    right = normalize(right * cos(roll * PI) + up * sin(roll * PI));

    up = cross(dir, right);
    up *= -1.0f;
}


void Camera::CalcProjMatrix() {
    projMatrix = perspective(fov, _ratio, nearPlane, farPlane);
}


void Camera::CalcViewMatrix() {
    viewMatrix = lookAt(pos, pos + dir, up);
}


void Camera::NormalizeAngle(double& angle) {
    if (angle > PImult2)
        angle -= PImult2;
    ef (angle < 0)
        angle += PImult2;
}


void Camera::NormalizeAngle(float& angle) {
    if (angle > PImult2)
        angle -= PImult2;
    ef (angle < 0)
        angle += PImult2;
}


void Camera::SetThirdPerson() {
    pos.x = player.positionMatrix[3].x - playerDist * dir.x;
    pos.y = player.positionMatrix[3].y - playerDist * dir.y + 6.0;
    pos.z = player.positionMatrix[3].z - playerDist * dir.z;
}


double Camera::GetMouseX() {
    return mouseX;
}


double Camera::GetMouseY() {
    return mouseY;
}


float Camera::GetSpeed() {
    return speed;
}


float Camera::GetPitch() {
    return pitch;
}


float Camera::GetYaw() {
    return yaw;
}


float Camera::GetRoll() {
    return roll;
}


float Camera::GetFOV() {
    return fov;
}


float Camera::GetRatio() {
    return _ratio;
}


float Camera::GetNearPlane() {
    return nearPlane;
}


float Camera::GetFarPlane() {
    return farPlane;
}


float Camera::GetPlayerDist() {
    return playerDist;
}


vec3 Camera::GetPos() {
    return pos;
}


vec3 Camera::GetDir() {
    return dir;
}


vec3 Camera::GetRight() {
    return right;
}


vec3 Camera::GetUp() {
    return up;
}


void Camera::ExtractFrustum() {
   mat4 clip;
   float t;

   /* Combine the two matrices (multiply projection by modelview) */
   clip = projMatrix * viewMatrix;

   /* Extract the numbers for the RIGHT plane */
   frustum[0][0] = clip[0][3] - clip[0][0];
   frustum[0][1] = clip[1][3] - clip[1][0];
   frustum[0][2] = clip[2][3] - clip[2][0];
   frustum[0][3] = clip[3][3] - clip[3][0];

   /* Normalize the result */
   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
   frustum[0][0] /= t;
   frustum[0][1] /= t;
   frustum[0][2] /= t;
   frustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   frustum[1][0] = clip[0][3] + clip[0][0];
   frustum[1][1] = clip[1][3] + clip[1][0];
   frustum[1][2] = clip[2][3] + clip[2][0];
   frustum[1][3] = clip[3][3] + clip[3][0];

   /* Normalize the result */
   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
   frustum[1][0] /= t;
   frustum[1][1] /= t;
   frustum[1][2] /= t;
   frustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   frustum[2][0] = clip[0][3] + clip[0][1];
   frustum[2][1] = clip[1][3] + clip[1][1];
   frustum[2][2] = clip[2][3] + clip[2][1];
   frustum[2][3] = clip[3][3] + clip[3][1];

   /* Normalize the result */
   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
   frustum[2][0] /= t;
   frustum[2][1] /= t;
   frustum[2][2] /= t;
   frustum[2][3] /= t;

   /* Extract the TOP plane */
   frustum[3][0] = clip[0][3] - clip[0][1];
   frustum[3][1] = clip[1][3] - clip[1][1];
   frustum[3][2] = clip[2][3] - clip[2][1];
   frustum[3][3] = clip[3][3] - clip[3][1];

   /* Normalize the result */
   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
   frustum[3][0] /= t;
   frustum[3][1] /= t;
   frustum[3][2] /= t;
   frustum[3][3] /= t;

   /* Extract the FAR plane */
   frustum[4][0] = clip[0][3] - clip[0][2];
   frustum[4][1] = clip[1][3] - clip[1][2];
   frustum[4][2] = clip[2][3] - clip[2][2];
   frustum[4][3] = clip[3][3] - clip[3][2];

   /* Normalize the result */
   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
   frustum[4][0] /= t;
   frustum[4][1] /= t;
   frustum[4][2] /= t;
   frustum[4][3] /= t;

   /* Extract the NEAR plane */
   frustum[5][0] = clip[0][3] + clip[0][2];
   frustum[5][1] = clip[1][3] + clip[1][2];
   frustum[5][2] = clip[2][3] + clip[2][2];
   frustum[5][3] = clip[3][3] + clip[3][2];

   /* Normalize the result */
   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
   frustum[5][0] /= t;
   frustum[5][1] /= t;
   frustum[5][2] /= t;
   frustum[5][3] /= t;
}


bool Camera::SphereInFrustum(const vec3 pos, const float radius) {
   for(uint i = 0; i < 6; ++i)
      if (frustum[i][0] * pos.x + frustum[i][1] * pos.y + frustum[i][2] * pos.z + frustum[i][3] <= -radius)
         return false;

   return true;
}
