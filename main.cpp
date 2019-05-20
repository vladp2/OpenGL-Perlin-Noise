#include <iostream>
#include <ctime>
#include <windows.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Window.h"
#include "Render.h"
#include "Buttons.h"
#include "Camera.h"
#include "FileManager.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Map.h"
#include "Collision.h"
#include "Mechanics.h"
#include "Algorithms.h"
#include "Terrain.h"
#include "Character.h"
#include "Animation.h"


using namespace std;
using namespace glm;


extern vector<mat4> matrices;


vector<Model> gModels;
vector<Texture> gTextures;
vector<Object> gObjects;
Object skybox;
Object crosshair;
Object player;


void Init() {
    srand(time(0));

    ReadModels();
    ReadTextures();

    Window::Init();

    Render::LoadShaders();
    Render::EnableZBuffer();
    Render::EnableCullFace();

    Model::Init();
    Texture::Init();

    Camera::Init();

    Terrain::Generate();
    Map::SpawnObjects();
}


void Update() {
    glfwPollEvents();

    if (Camera::CheckMouse()) {
        Camera::MoveFPS();
        //Camera::Move360();
        //Camera::ExtractFrustum(); // Posibil conflict cu Camera::SetThirdPerson
    }

    Camera::CheckKeys();

    Map::CheckKeys();

    Mechanics::CheckKeys();
    Mechanics::Move();

    Collision::CheckTerrain();
    Collision::CheckObjects();

    Camera::SetThirdPerson();

    Camera::CalcProjMatrix();
    Camera::CalcViewMatrix();
}


void Render() {
    glfwSwapBuffers(Window::GetHandle());

    Render::ClearScreen();

    Render::DrawTerrain();
    Render::DrawObjectsInstanced();
    Render::DrawCharacters();
    Render::DrawObjects();
    Render::DrawSkybox();
    Render::DrawHUD();
}


void Terminate() {
    Model::Clean();
    Texture::Clean();
    Render::Clean();

    glfwTerminate();
}

// A, S, D, W - move character
// SPACE - move upwards
// CTRL - move downwards
// SHIFT - increase moving speed (hold)
// F - spawn object
// G - delete object
// MOUSE1 - pick & move object
// MOUSE2 - rotate object
// R - shoot bullet
int main() {
    Init();

    while (Window::IsActive()) {
        Update();

        Render();

        CountFPS();
    }

    Terminate();

    return 0;
}
