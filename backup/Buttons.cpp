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

#include "Buttons.h"
#include "Window.h"


extern Window window;


bool Buttons::CheckKeybd(GLuint keyId, int action) {
    if (glfwGetKey(window.handle, keyId) == GLFW_PRESS) {
        if (action == GLFW_REPEAT)
            return true;

        else if (this->keybdKey[keyId] != 'P') {
            this->keybdKey[keyId] = 'P';
            return true;
        }
    }
    else
        keybdKey[keyId] = 0;

    return false;
}

bool Buttons::CheckMouse(GLuint keyId, int action) {
    if (glfwGetMouseButton(window.handle, keyId) == GLFW_PRESS) {
        if (action == GLFW_REPEAT)
            return true;

        else if (this->mouseKey[keyId] != 'P') {
            this->mouseKey[keyId] = 'P';
            return true;
        }
    }
    else
        this->mouseKey[keyId] = 0;

    return false;
}
