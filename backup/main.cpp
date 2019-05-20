#include <iostream>
#include <ctime>
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

using namespace std;
using namespace glm;

#define ef else if

#include "Window.h"
#include "Buttons.h"
#include "Camera.h"
#include "FileManager.h"
#include "Object.h"
#include "Map.h"


Window window;

Buttons buttons;

Camera cam;

Model model[100];
GLuint modelCount;

Texture tex[100];
GLuint texCount;

Object obj[2000];
Object crosshair;
GLuint objCount;

Map _map;


int main() {
    srand(time(NULL));

    _map.LoadModels("Game/models.txt");
    _map.LoadTextures("Game/textures.txt");

    window.Init();

    cam.Init();

    _map.LoadShaders();
    _map.GenObjects();
    _map.EnableZBuffer();

    while (window.IsActive()) {
        cam.GetDeltaTime();

        cam.CheckMouse();
        cam.CheckKeys();

        _map.CheckKeys();

        _map.DrawObjects();

        glfwPollEvents();
        glfwSwapBuffers(window.handle);
    }

    return 0;
}
