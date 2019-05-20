#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Texture.h"
#include "Model.h"
#include "Object.h"
#include "Render.h"
#include "Map.h"
#include "Camera.h"


using namespace std;
using namespace glm;


extern vector<Texture> gTextures;
extern vector<Model> gModels;
extern vector<Object> gObjects;


map<string, uint> Texture::nameMap;


void Texture::Init() {
    for (uint index = 0; index < gTextures.size(); ++index) {
        gTextures[index].Load();
    }
}


void Texture::Load() {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // UNELE MODELE NU AU NEVOIE DE MIPMAP
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    CleanData();
}


uint Texture::GetID() {
    return ID;
}


void Texture::SetID(const uint ID) {
    this->ID = ID;
}


void Texture::CleanData() {
    data.clear();
}


uint Texture::FindIndex(const string& name) {
    for (uint index = 0; index < gTextures.size(); ++index)
        if (gTextures[index].name == name)
            return index;

    cout << "Nu s-a gasit textura " << name;
    exit(0);
}


void Texture::Clean() {
    for (uint i = 0; i < gTextures.size(); ++i)
        glDeleteTextures(1, &gTextures[i].ID);
}
