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

#include "Animation.h"
#include "Buttons.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Character.h"


#define ef else if


using namespace std;
using namespace glm;


extern Character player;


bool Animation::isHitting;


void Animation::CheckKeys() {
    if (Buttons::CheckKeybd(GLFW_KEY_E, GLFW_REPEAT)) {
        if (!isHitting) {
            isHitting = true;
            StartHitting();
        }
    }
    ef (isHitting) {
        isHitting = false;
        StopHitting();
    }
}


void Animation::StartHitting() {
    player.body[3].SetModelID(Model::FindIndex("skelet_rarm_raised"));
}


void Animation::StopHitting() {
    player.body[3].SetModelID(Model::FindIndex("skelet_rarm"));
}
