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


extern Window window;


void Window::Init() {
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    this->width = 1000;
    this->height = 650;
    strcpy(this->title, "Game");

    // Open a window and create its OpenGL context
    this->handle = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
    if (this->handle == NULL) {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->handle); // Initialize GLEW
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    glfwSetWindowPos(this->handle, 20, 50);
    glViewport(0, 0, this->width, this->height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glfwSetInputMode(this->handle, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(this->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(this->handle, this->width / 2, this->height / 2);
}

bool Window::IsActive() {
    return (glfwGetKey(this->handle, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(this->handle));
}
