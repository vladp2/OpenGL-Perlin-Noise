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



char* Buttons::keybdKey = new char [350];
char* Buttons::mouseKey = new char [350];




Buttons::~Buttons() {
    delete [] keybdKey;
    delete [] mouseKey;
}



bool Buttons::CheckKeybd(const uint keyId, const uint action) {
    if (glfwGetKey(window.GetHandle(), keyId) == GLFW_PRESS) {
        if (action == GLFW_REPEAT)
            return true;

        ef (keybdKey[keyId] != 'P') {
            keybdKey[keyId] = 'P';

            return true;
        }
    }
    else
        keybdKey[keyId] = 0;

    return false;
}


bool Buttons::CheckMouse(const uint keyId, const uint action) {
    if (glfwGetMouseButton(window.GetHandle(), keyId) == GLFW_PRESS) {
        if (action == GLFW_REPEAT)
            return true;

        ef (mouseKey[keyId] != 'P') {
            mouseKey[keyId] = 'P';
            return true;
        }
    }
    else
        mouseKey[keyId] = 0;

    return false;
}
