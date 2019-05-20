#include <iostream>
#include <ctime>
#include <windows.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Render.h"
#include "FileManager.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Terrain.h"
#include "Character.h"
#include "Mechanics.h"
#include "Collision.h"
#include "Buttons.h"


using namespace std;
using namespace glm;

// DE SEPARAT METODELE UPDATE DE METODELE RENDER

extern vector<Model> gModels;
extern vector<Texture> gTextures;
extern vector<Object> gObjects;
extern Object crosshair;
extern Object skybox;
extern Object player;
extern vector<mat4> matrices;


uint Render::programID;


int condition;
vec3 lightPosition = vec3(3000.0f, 2000.0f, 3000.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
float shineDamper = 12;
float reflectivity = 1;


vector<Object> bullets;
bool shot = false;


void Render::DrawObjects() {
    static vec3 dir;
    static double startTime;

    if (!shot)
    if (Buttons::CheckKeybd(GLFW_KEY_R, GLFW_PRESS)) {
        shot = true;
        bullets.push_back(Object());
        bullets.back().Create("cube", "yellow", vec3(player.modelMatrix[3].x, player.modelMatrix[3].y, player.modelMatrix[3].z));
        bullets.back().Scale(vec3(0.1f, 0.1f, 0.1f));
        dir = vec3(Camera::GetDir().x, Camera::GetDir().y, Camera::GetDir().z);
        startTime = glfwGetTime();
        lightColor = vec3(1.0f, 1.0f, 0.0f);
    }

    if (!shot) {
        return;
    }

    double currentTime = glfwGetTime();
    if (currentTime - startTime >= 3.0) {
        shot = false;
        bullets.pop_back();
        lightColor = vec3(1.0f, 1.0f, 1.0f);
        return;
    }

    bullets.back().modelMatrix[3].x += dir.x * 1.0f;
    bullets.back().modelMatrix[3].y += dir.y * 1.0f;
    bullets.back().modelMatrix[3].z += dir.z * 1.0f;

    lightPosition = vec3(bullets.back().modelMatrix[3].x, bullets.back().modelMatrix[3].y, bullets.back().modelMatrix[3].z);

    UpdateShaders(Camera::projMatrix, Camera::viewMatrix, 1, 0);

    SetUniform("modelMatrix", bullets.back().modelMatrix);

    Draw(gModels[bullets.back().GetModelID()], gTextures[bullets.back().GetTextureID()]);
}


bool CheckDistance(const Object& obj1, const Object& obj2, const float dist) {
    if (abs(obj1.modelMatrix[3].x - obj2.modelMatrix[3].x) > dist)
        return false;

    if (abs(obj1.modelMatrix[3].y - obj2.modelMatrix[3].y) > dist)
        return false;

    if (abs(obj1.modelMatrix[3].z - obj2.modelMatrix[3].z) > dist)
        return false;

    return true;
}


void UpdateInstanced() {
    vec3 direction;
    vec3 position;
    vec3 rotation;
    vec3 scaling;

    for (uint i = 0; i < gObjects.size(); ++i) {
        if (!CheckDistance(player, gObjects[i], 1000)) {
            direction = vec3(
                float(rand() % 2001 - 1000) / 1000.0f,
                float(rand() % 2001 - 1000) / 1000.0f,
                float(rand() % 2001 - 1000) / 1000.0f
            );

            position = vec3(
                player.modelMatrix[3].x + 1000.0f * direction.x,
                player.modelMatrix[3].y + 1000.0f * direction.y,
                player.modelMatrix[3].z + 1000.0f * direction.z
            );

            rotation = vec3(
                rand() % 360,
                rand() % 360,
                rand() % 360
            );

            scaling = vec3(
                float(rand() % 200 + 10) / 10.0f,
                float(rand() % 200 + 10) / 10.0f,
                float(rand() % 200 + 10) / 10.0f
            );

            gObjects[i].rotationMatrix = mat4(1.0f);

            gObjects[i].Place(position);
            gObjects[i].Rotate(rotation);
            gObjects[i].Scale(scaling);

            matrices[i] = gObjects[i].modelMatrix;
        }
    }
}


void Render::DrawObjectsInstanced() {
    UpdateShaders(Camera::projMatrix, Camera::viewMatrix, 2, 1);

    UpdateInstanced();

    Model model = gModels[gObjects[0].GetModelID()];
    Texture texture = gTextures[gObjects[0].GetTextureID()];

    model.BindVAO();

    glBindBuffer(GL_ARRAY_BUFFER, model.GetVBO(3));
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(mat4), matrices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetID());

    glDrawArraysInstanced(GL_TRIANGLES, 0, model.GetVertexCount(), matrices.size());

    model.UnbindVAO();
}


void Render::DrawCharacters() {
    UpdateShaders(Camera::projMatrix, Camera::viewMatrix, 2, 0);

    player.box.center.x = player.positionMatrix[3].x;
    player.box.center.y = player.positionMatrix[3].y;
    player.box.center.z = player.positionMatrix[3].z;

    player.box.rotationMatrix[0] = vec3(player.rotationMatrix[0].x, player.rotationMatrix[0].y, player.rotationMatrix[0].z);
    player.box.rotationMatrix[1] = vec3(player.rotationMatrix[1].x, player.rotationMatrix[1].y, player.rotationMatrix[1].z);
    player.box.rotationMatrix[2] = vec3(player.rotationMatrix[2].x, player.rotationMatrix[2].y, player.rotationMatrix[2].z);

    player.box.halfLength.x = player.scalingMatrix[0].x;
    player.box.halfLength.y = player.scalingMatrix[1].y;
    player.box.halfLength.z = player.scalingMatrix[2].z;

    player.modelMatrix = player.positionMatrix * player.rotationMatrix * player.scalingMatrix;

    lightPosition = vec3(player.modelMatrix[3].x, player.modelMatrix[3].y, player.modelMatrix[3].z);

    SetUniform("modelMatrix", player.modelMatrix);

    Draw(gModels[player.GetModelID()], gTextures[player.GetTextureID()]);
}


void Render::DrawSkybox() {
    UpdateShaders(Camera::projMatrix, Camera::viewMatrix, 1, 0);

    skybox.modelMatrix[3].x = Camera::GetPos().x;
    skybox.modelMatrix[3].y = Camera::GetPos().y;
    skybox.modelMatrix[3].z = Camera::GetPos().z;

    SetUniform("modelMatrix", skybox.modelMatrix);

    Draw(gModels[skybox.GetModelID()], gTextures[skybox.GetTextureID()]);
}


void Render::DrawHUD() {
    mat4 viewMatrix = lookAt(vec3(0.0f, 0.0f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    UpdateShaders(Camera::projMatrix, viewMatrix, 1, 0);

    SetUniform("modelMatrix", crosshair.modelMatrix);

    Draw(gModels[crosshair.GetModelID()], gTextures[crosshair.GetTextureID()]);
}


void Render::DrawTerrain() {
    UpdateShaders(Camera::projMatrix, Camera::viewMatrix, 2, 0);

    Terrain::Update();

    int chunkZ;
    int chunkX;
    uint z;
    uint x;

    for (z = 0, chunkZ = Terrain::centerChunkZ - (Terrain::chunkCount >> 1); z < Terrain::chunkCount; ++z, ++chunkZ)
    for (x = 0, chunkX = Terrain::centerChunkX - (Terrain::chunkCount >> 1); x < Terrain::chunkCount; ++x, ++chunkX) {
        SetUniform("modelMatrix", mat4(1.0f));

        Model model = Terrain::models[z][x];
        Texture texture = Terrain::textures[z][x];

        model.BindVAO();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.GetID());

        glDrawArrays(GL_TRIANGLES, 0, model.GetVertexCount());

        model.UnbindVAO();
    }
}


void Render::UpdateShaders(const mat4& projMatrix, const mat4& viewMatrix, const int lightingMode, const int instanced) {
    SetUniform("projectionMatrix", projMatrix);
    SetUniform("viewMatrix", viewMatrix);
    SetUniform("lightPosition", lightPosition);
    SetUniform("lightColor", lightColor);
    SetUniform("shineDamper", shineDamper);
    SetUniform("reflectivity", reflectivity);
    SetUniform("lightingMode", lightingMode);
    SetUniform("instanced", instanced);
}


void Render::Draw(Model& model, Texture& texture) {
    model.BindVAO();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetID());

    glDrawArrays(GL_TRIANGLES, 0, model.GetVertexCount());

    model.UnbindVAO();
}


void Render::SetUniform(const string name, const int value) {
    uint location = glGetUniformLocation(programID, name.c_str());
    glUniform1i(location, value);
}


void Render::SetUniform(const string name, const mat4 value) {
    uint location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}


void Render::SetUniform(const string name, const vec3 value) {
    uint location = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(location, 1, &value[0]);
}


void Render::SetUniform(const string name, const float value) {
    uint location = glGetUniformLocation(programID, name.c_str());
    glUniform1f(location, value);
}


void Render::LoadShaders() {
    string vertexFile = "VertexShader.txt";
    string fragmentFile = "FragmentShader.txt";

    programID = ReadShaders(vertexFile, fragmentFile);

    if (!programID) {
        cout << "Nu s-au putut initializa shaderele";
        exit(0);
    }

    glUseProgram(programID);
}


void Render::EnableZBuffer() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void Render::EnableCullFace() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


void Render::ClearScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}


uint Render::GetProgramID() {
    return programID;
}


void Render::Clean() {
    glDeleteProgram(programID);
}
