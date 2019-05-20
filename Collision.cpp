#include <iostream>
#include <iomanip>
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

#include "Collision.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include "Mechanics.h"
#include "FileManager.h"
#include "Algorithms.h"
#include "Terrain.h"
#include "Character.h"


#define ef else if

#define PI 3.1415926535897932f
#define RAD 57.29577951308232f
#define EPSILON 0.0001f


using namespace std;
using namespace glm;


extern vector<Model> gModels;
extern vector<Object> gObjects;
extern Object player;


extern bool moveFront;
extern bool moveSide;
extern bool moveVert;
extern vec3 dirFront;
extern vec3 dirSide;
extern vec3 dirVert;
extern float speedFront;
extern float speedSide;
extern float speedVert;
extern PerlinNoise noise;


float Collision::depth;
vec3 Collision::dir;
vec3 Collision::sign;
string Collision::face;
float Collision::terrainHeight;


bool Collision::IsUnderGround(const Object& player) {
    return player.positionMatrix[3].y - player.box.halfLength.y < terrainHeight;
}


bool Collision::IsOnGround(const Object& player) {
    return player.positionMatrix[3].y - player.box.halfLength.y == terrainHeight;
}


bool Collision::IsOnObject() {
    return dir.y > 0.0f && sign.y == -1;
}


void Collision::SetOnGround(Object& player) {
    player.positionMatrix[3].y = terrainHeight + player.box.halfLength.y;
}


void Collision::CheckTerrain() {
    terrainHeight = noise.GetHeight(abs(player.positionMatrix[3].x), abs(player.positionMatrix[3].z));

    Mechanics::SetOnGround(false);

    if (IsUnderGround(player)) {
        SetOnGround(player);
    }

    if (IsOnGround(player)) {
        Mechanics::SetOnGround(true);
    }
}


void Collision::CheckObjects() {
    Mechanics::SetOnObject(false);

    for (uint i = 0; i < gObjects.size(); ++i) {
        if (NaiveCheckOBB(player.box, gObjects[i].box, 60.0f))
        if (CheckOBB(player.box, gObjects[i].box)) {
            cout << "CRASH\n";

            Mechanics::SetOnObject(true);

            PushBack(player);
        }
    }
}


bool Collision::NaiveCheckOBB(const OBB& firstBox, const OBB& secondBox, const float dist) {
    if (abs(firstBox.center.x - secondBox.center.x) > dist)
        return false;

    if (abs(firstBox.center.y - secondBox.center.y) > dist)
        return false;

    if (abs(firstBox.center.z - secondBox.center.z) > dist)
        return false;

    return true;
}


void Collision::PushBack(Object& character) {
    character.positionMatrix[3].x -= depth * dir.x * sign.x;
    character.positionMatrix[3].y -= depth * dir.y * sign.y;
    character.positionMatrix[3].z -= depth * dir.z * sign.z;
}


bool Collision::CheckOBB(const OBB& firstBox, const OBB& secondBox) {
    depth = 999999.0f;

    float firstRadius;
    float secondRadius;
    float dist;
    vec3 translation;
    mat3 rotationMatrix;
    mat3 absRotationMatrix;
    vec3 axis;

    translation = secondBox.center - firstBox.center;

    sign.x = (translation.x < 0 ? -1 : 1);
    sign.y = (translation.y < 0 ? -1 : 1);
    sign.z = (translation.z < 0 ? -1 : 1);

    for (short i = 0; i < 3; ++i)
    for (short j = 0; j < 3; ++j)
        rotationMatrix[i][j] = dot(firstBox.rotationMatrix[i], secondBox.rotationMatrix[j]);

    translation = vec3(
        dot(translation, firstBox.rotationMatrix[0]),
        dot(translation, firstBox.rotationMatrix[1]),
        dot(translation, firstBox.rotationMatrix[2])
    );

    for (short i = 0; i < 3; i++)
    for (short j = 0; j < 3; j++)
        absRotationMatrix[i][j] = abs(rotationMatrix[i][j]) + EPSILON;

    for (short i = 0; i < 3; i++) {
        firstRadius = firstBox.halfLength[i];
        secondRadius = secondBox.halfLength[0] * absRotationMatrix[i][0] + secondBox.halfLength[1] * absRotationMatrix[i][1] + secondBox.halfLength[2] * absRotationMatrix[i][2];
        dist = abs(translation[i]);

        if (dist > firstRadius + secondRadius)
            return false;

        axis = firstBox.rotationMatrix[i];
        dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
        if (dist < depth) {
            depth = dist;
            dir = abs(axis);
        }
    }

    for (short i = 0; i < 3; i++) {
        firstRadius = firstBox.halfLength[0] * absRotationMatrix[0][i] + firstBox.halfLength[1] * absRotationMatrix[1][i] + firstBox.halfLength[2] * absRotationMatrix[2][i];
        secondRadius = secondBox.halfLength[i];
        dist = abs(translation[0] * rotationMatrix[0][i] + translation[1] * rotationMatrix[1][i] + translation[2] * rotationMatrix[2][i]);

        if (dist > firstRadius + secondRadius)
            return false;

        axis = secondBox.rotationMatrix[i];
        dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
        if (dist < depth) {
            depth = dist;
            dir = abs(axis);
        }
    }

    firstRadius = firstBox.halfLength[1] * absRotationMatrix[2][0] + firstBox.halfLength[2] * absRotationMatrix[1][0];
    secondRadius = secondBox.halfLength[1] * absRotationMatrix[0][2] + secondBox.halfLength[2] * absRotationMatrix[0][1];
    dist = abs(translation[2] * rotationMatrix[1][0] - translation[1] * rotationMatrix[2][0]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[0], secondBox.rotationMatrix[0]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[1] * absRotationMatrix[2][1] + firstBox.halfLength[2] * absRotationMatrix[1][1];
    secondRadius = secondBox.halfLength[0] * absRotationMatrix[0][2] + secondBox.halfLength[2] * absRotationMatrix[0][0];
    dist = abs(translation[2] * rotationMatrix[1][1] - translation[1] * rotationMatrix[2][1]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[0], secondBox.rotationMatrix[1]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[1] * absRotationMatrix[2][2] + firstBox.halfLength[2] * absRotationMatrix[1][2];
    secondRadius = secondBox.halfLength[0] * absRotationMatrix[0][1] + secondBox.halfLength[1] * absRotationMatrix[0][0];
    dist = abs(translation[2] * rotationMatrix[1][2] - translation[1] * rotationMatrix[2][2]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[0], secondBox.rotationMatrix[2]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[0] * absRotationMatrix[2][0] + firstBox.halfLength[2] * absRotationMatrix[0][0];
    secondRadius = secondBox.halfLength[1] * absRotationMatrix[1][2] + secondBox.halfLength[2] * absRotationMatrix[1][1];
    dist = abs(translation[0] * rotationMatrix[2][0] - translation[2] * rotationMatrix[0][0]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[1], secondBox.rotationMatrix[0]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[0] * absRotationMatrix[2][1] + firstBox.halfLength[2] * absRotationMatrix[0][1];
    secondRadius = secondBox.halfLength[0] * absRotationMatrix[1][2] + secondBox.halfLength[2] * absRotationMatrix[1][0];
    dist = abs(translation[0] * rotationMatrix[2][1] - translation[2] * rotationMatrix[0][1]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[1], secondBox.rotationMatrix[1]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[0] * absRotationMatrix[2][2] + firstBox.halfLength[2] * absRotationMatrix[0][2];
    secondRadius = secondBox.halfLength[0] * absRotationMatrix[1][1] + secondBox.halfLength[1] * absRotationMatrix[1][0];
    dist = abs(translation[0] * rotationMatrix[2][2] - translation[2] * rotationMatrix[0][2]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[1], secondBox.rotationMatrix[2]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[0] * absRotationMatrix[1][0] + firstBox.halfLength[1] * absRotationMatrix[0][0];
    secondRadius = secondBox.halfLength[1] * absRotationMatrix[2][2] + secondBox.halfLength[2] * absRotationMatrix[2][1];
    dist = abs(translation[1] * rotationMatrix[0][0] - translation[0] * rotationMatrix[1][0]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[2], secondBox.rotationMatrix[0]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[0] * absRotationMatrix[1][1] + firstBox.halfLength[1] * absRotationMatrix[0][1];
    secondRadius = secondBox.halfLength[0] * absRotationMatrix[2][2] + secondBox.halfLength[2] * absRotationMatrix[2][0];
    dist = abs(translation[1] * rotationMatrix[0][1] - translation[0] * rotationMatrix[1][1]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[2], secondBox.rotationMatrix[1]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    firstRadius = firstBox.halfLength[0] * absRotationMatrix[1][2] + firstBox.halfLength[1] * absRotationMatrix[0][2];
    secondRadius = secondBox.halfLength[0] * absRotationMatrix[2][1] + secondBox.halfLength[1] * absRotationMatrix[2][0];
    dist = abs(translation[1] * rotationMatrix[0][2] - translation[0] * rotationMatrix[1][2]);

    if (dist > firstRadius + secondRadius)
        return false;

    axis = cross(firstBox.rotationMatrix[2], secondBox.rotationMatrix[2]);
    dist = (firstRadius + secondRadius - dist) / dot(axis, axis);
    if (dist < depth) {
        depth = dist;
        dir = abs(axis);
    }

    return true;
}


vec3 Collision::ClosestPointOBB(const vec3& p, const OBB& obb) {
    vec3 dist;
    vec3 dir = p - obb.center;

    dist.x = dot(dir, obb.rotationMatrix[0]);
    if (dist.x > obb.halfLength.x)
        dist.x = obb.halfLength.x;
    ef (dist.x < -obb.halfLength.x)
        dist.x = -obb.halfLength.x;

    dist.y = dot(dir, obb.rotationMatrix[1]);
    if (dist.y > obb.halfLength.y)
        dist.y = obb.halfLength.y;
    ef (dist.y < -obb.halfLength.y)
        dist.y = -obb.halfLength.y;

    dist.z = dot(dir, obb.rotationMatrix[2]);
    if (dist.z > obb.halfLength.z)
        dist.z = obb.halfLength.z;
    ef (dist.z < -obb.halfLength.z)
        dist.z = -obb.halfLength.z;

    return
        obb.center +
        dist.x * obb.rotationMatrix[0] +
        dist.y * obb.rotationMatrix[1] +
        dist.z * obb.rotationMatrix[2];
}


bool Collision::PointPlaneNormal(const vec3& p, const vec3& p1, const vec3& p2, const vec3& p3) {
    vec3 u = p2 - p1;
    vec3 v = p3 - p1;
    vec3 w = p - p1;

    vec3 normal = cross(u, v);

    float gamma = dot(cross(u, w), normal) / dot(normal, normal);
    float beta = dot(cross(w, v), normal) / dot(normal, normal);
    float alpha = 1 - gamma - beta;

    return
        alpha >= 0 && alpha <= 1 &&
        beta >= 0 && beta <= 1 &&
        gamma >= 0 && gamma <= 1;
}


vec3 Collision::PlaneNormal(const vec3& p1, const vec3& p2, const vec3& p3) {
    return normalize(cross(p2 - p1, p3 - p1));
}


float Collision::PointPlaneDistance(const vec3& p, const vec3& pPlane, const vec3& normal) {
    return
        normal.x * p.x +
        normal.y * p.y +
        normal.z * p.z -
        dot(normal, pPlane);
}


float Collision::PointPointDistance(const vec3& p1, const vec3& p2) {
    return sqrt(
        (p1.x - p2.x) * (p1.x - p2.x) +
        (p1.y - p2.y) * (p1.y - p2.y) +
        (p1.z - p2.z) * (p1.z - p2.z)
    );
}


void Collision::NormalizeRay(vec3& direction) {
    // The ray Start and End positions, in Normalized Device Coordinates
	glm::vec4 start_NDC(
		(Camera::GetMouseX() / Window::GetWidth()  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		(Camera::GetMouseY() / Window::GetHeight() - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);

	glm::vec4 end_NDC(
		(Camera::GetMouseX() / Window::GetWidth()  - 0.5f) * 2.0f,
		(Camera::GetMouseY() / Window::GetHeight() - 0.5f) * 2.0f,
		0.0,
		1.0f
	);

	mat4 projMatrix = perspective(Camera::GetFOV(), Camera::GetRatio(), Camera::GetNearPlane(), Camera::GetFarPlane());
    mat4 viewMatrix = lookAt(Camera::GetPos(), Camera::GetPos() + Camera::GetDir(), Camera::GetUp());

	glm::mat4 matrix = glm::inverse(projMatrix * viewMatrix);

	glm::vec4 start_world = matrix * start_NDC;
	start_world /= start_world.w;

	glm::vec4 end_world = matrix * end_NDC;
	end_world /= end_world.w;

	glm::vec3 dir_world = end_world - start_world;
	dir_world = glm::normalize(dir_world);

	direction = glm::normalize(dir_world);
}


void Collision::CheckFaces(const vec3 origin, const vec3 direction, const OBB box) {
    float minDist = 999999.0f;
    float dist;
    OBB faceBox = box;

    faceBox.center.x = box.center.x - 1.0f;
    faceBox.halfLength.x = 0.0f;

    dist = CheckRay(origin, direction, faceBox);
    if (dist != -1)
    if (dist < minDist) {
        minDist = dist;
        face = "-x";
    }

    faceBox.center.x = box.center.x + 1.0f;

    dist = CheckRay(origin, direction, faceBox);
    if (dist != -1)
    if (dist < minDist) {
        minDist = dist;
        face = "+x";
    }

    faceBox.center.x = box.center.x;
    faceBox.halfLength.x = box.halfLength.x;
    faceBox.center.z = box.center.z - 1.0f;
    faceBox.halfLength.z = 0.0f;

    dist = CheckRay(origin, direction, faceBox);
    if (dist != -1)
    if (dist < minDist) {
        minDist = dist;
        face = "-z";
    }

    faceBox.center.z = box.center.z + 1.0f;

    dist = CheckRay(origin, direction, faceBox);
    if (dist != -1)
    if (dist < minDist) {
        minDist = dist;
        face = "+z";
    }

    faceBox.center.z = box.center.z;
    faceBox.halfLength.z = box.halfLength.z;
    faceBox.center.y = box.center.y - 1.0f;
    faceBox.halfLength.y = 0.0f;

    dist = CheckRay(origin, direction, faceBox);
    if (dist != -1)
    if (dist < minDist) {
        minDist = dist;
        face = "-y";
    }

    faceBox.center.y = box.center.y + 1.0f;

    dist = CheckRay(origin, direction, faceBox);
    if (dist != -1)
    if (dist < minDist) {
        minDist = dist;
        face = "+y";
    }
}


uint Collision::RayCast(vec3 origin, vec3 direction, float& targetObjDist) {
    int target = -1;
    float targetDist = 999999.0f;
    float dist;

    NormalizeRay(direction);

    for (uint i = 0; i < gObjects.size(); ++i) {
        dist = CheckRay(origin, direction, gObjects[i].box);

        if (dist == -1)
            continue;

        if (dist < targetDist) {
            targetDist = dist;
            target = i;
        }
    }

    if (target != -1) {
        CheckFaces(origin, direction, gObjects[target].box);
    }

    targetObjDist = targetDist + 2.0f;

    return target;
}


float Collision::CheckRay(const vec3 origin, const vec3 direction, const OBB& box) {
    float minDist = 0.0f;
    float maxDist = 999999.0f;
    float dist1;
    float dist2;
    vec3 translation;
    vec3 axis;
    float e;
    float f;

    translation = box.center - origin;

    // X Axis
    axis = vec3(box.rotationMatrix[0].x, box.rotationMatrix[0].y, box.rotationMatrix[0].z);

    e = dot(axis, translation);
    f = dot(direction, axis);

    dist1 = (e - box.halfLength.x) / (f ? f : 1.0f);
    dist2 = (e + box.halfLength.x) / (f ? f : 1.0f);

    if (dist1 > dist2)
        std::swap(dist1, dist2);

    minDist = std::max(minDist, dist1);
    maxDist = std::min(maxDist, dist2);

    if (minDist > maxDist)
        return -1;

    // Y Axis
    axis = vec3(box.rotationMatrix[1].x, box.rotationMatrix[1].y, box.rotationMatrix[1].z);

    e = dot(axis, translation);
    f = dot(direction, axis);

    dist1 = (e - box.halfLength.y) / (f ? f : 1.0f);
    dist2 = (e + box.halfLength.y) / (f ? f : 1.0f);

    if (dist1 > dist2)
        std::swap(dist1, dist2);

    minDist = std::max(minDist, dist1);
    maxDist = std::min(maxDist, dist2);

    if (minDist > maxDist)
        return -1;

    // Z Axis
    axis = vec3(box.rotationMatrix[2].x, box.rotationMatrix[2].y, box.rotationMatrix[2].z);

    e = dot(axis, translation);
    f = dot(direction, axis);

    dist1 = (e - box.halfLength.z) / (f ? f : 1.0f);
    dist2 = (e + box.halfLength.z) / (f ? f : 1.0f);

    if (dist1 > dist2)
        std::swap(dist1, dist2);

    minDist = std::max(minDist, dist1);
    maxDist = std::min(maxDist, dist2);

    if (minDist > maxDist)
        return -1;

    return minDist;
}
