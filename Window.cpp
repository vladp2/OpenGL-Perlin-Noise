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

#include "Window.h"
#include "Render.h"
#include "Buttons.h"


using namespace std;


const uint WIDTH = 1024;
const uint HEIGHT = 640;
const string TITLE = "Game";


GLFWwindow* Window::handle;
string Window::title;
ushort Window::width;
ushort Window::height;


void Window::Init() {
    if(!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        exit(0);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    SetTitle(TITLE);
    SetWidth(WIDTH);
    SetHeight(HEIGHT);

    // Open a window and create its OpenGL context
    handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (handle == NULL) {
        std::cout << "Failed to open GLFW window\n";
        glfwTerminate();
        exit(0);
    }

    glfwMakeContextCurrent(handle); // Initialize GLEW
    glewExperimental = true; // Needed in core profile

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n";
        exit(0);
    }

    glfwSetWindowPos(handle, 250, 50);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glfwSetInputMode(handle, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(handle, width / 2, height / 2);
}


GLFWwindow* Window::GetHandle() {
    return handle;
}


string Window::GetTitle() {
    return title;
}


ushort Window::GetWidth() {
    return width;
}


ushort Window::GetHeight() {
    return height;
}


void Window::SetTitle(const string name) {
    title = name;
}


void Window::SetWidth(const int val) {
    width = val;
}


void Window::SetHeight(const int val) {
    height = val;
}


bool Window::IsActive() {
    if (Buttons::CheckKeybd(GLFW_KEY_ESCAPE, GLFW_PRESS))
        return false;

    if (glfwWindowShouldClose(handle))
        return false;

    return true;
}
