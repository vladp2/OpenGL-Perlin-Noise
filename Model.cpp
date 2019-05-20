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

#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Render.h"
#include "Map.h"
#include "Camera.h"


using namespace std;
using namespace glm;


extern vector<Texture> gTextures;
extern vector<Model> gModels;
extern vector<Object> gObjects;


map<string, uint> Model::nameMap;


vector<mat4> matrices;


void Model::Init() {
    for (uint index = 0; index < gModels.size(); ++index) {
        if (gModels[index].name == "cube") {
            gModels[index].LoadInstanced();
        }
        else {
            gModels[index].Load();
        }
    }
}


uint Model::GetVAO() {
    return VAO;
}


uint Model::GetVBO(const uint index) {
    return VBOs[index];
}


vector<uint> Model::GetVBOs() {
    return VBOs;
}


uint Model::GetEBO(const uint index) {
    return EBOs[index];
}


uint Model::GetVertexCount() {
    return vertexCount;
}


void Model::BindVAO() {
    glBindVertexArray(VAO);
}


void Model::SetVAO(const uint VAO) {
    this->VAO = VAO;
}


void Model::SetVBOs(const vector<uint>& VBOs) {
    this->VBOs = VBOs;
}


void Model::UnbindVAO() {
    glBindVertexArray(0);
}


void Model::EnableAttribute(const uint attributeID) {
    glEnableVertexAttribArray(attributeID);
}


void Model::DisableAttribute(const uint attributeID) {
    glDisableVertexAttribArray(attributeID);
}


void Model::Load() {
    CreateVAO();

    CreateVBO(0, 3, positions);
    UnbindVBO();
    SetMinMaxVertex(positions);
    CleanData(positions);

    CreateVBO(1, 2, UVs);
    UnbindVBO();
    CleanData(UVs);

    CreateVBO(2, 3, normals);
    UnbindVBO();
    CleanData(normals);

    UnbindVAO();
}


void Model::LoadInstanced() {
    CreateVAO();

    CreateVBO(0, 3, positions);
    UnbindVBO();
    SetMinMaxVertex(positions);
    CleanData(positions);

    CreateVBO(1, 2, UVs);
    UnbindVBO();
    CleanData(UVs);

    CreateVBO(2, 3, normals);
    UnbindVBO();
    CleanData(normals);

    matrices.resize(1000);

    for (unsigned i = 0; i < matrices.size(); ++i) {
        matrices[i] = mat4(1.0f);
        matrices[i][3].x = 999999.0f;
        matrices[i][3].y = 999999.0f;
        matrices[i][3].z = 999999.0f;
    }

    VBOs.push_back(uint());

    glGenBuffers(1, &VBOs.back());
    glBindBuffer(GL_ARRAY_BUFFER, VBOs.back());
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(mat4), matrices.data(), GL_STATIC_DRAW);

    for (unsigned i = 0; i < 4; ++i) {
        glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(i * sizeof(vec4)));
        glVertexAttribDivisor(i + 3, 1);
        glEnableVertexAttribArray(i + 3);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    UnbindVAO();
}


void Model::CreateVAO() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}


void Model::CreateVBO(const uint attributeID, const uint coordSize, const vector<float>& data) {
    VBOs.push_back(uint());

    glGenBuffers(1, &VBOs.back());
    glBindBuffer(GL_ARRAY_BUFFER, VBOs.back());
    glBufferData(GL_ARRAY_BUFFER, vertexCount * coordSize * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(attributeID, coordSize, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(attributeID, 0);
    glEnableVertexAttribArray(attributeID);
}


void Model::UnbindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Model::SetMinMaxVertex(const vector<float>& vertices) {
    minVertex = vec3(vertices[0], vertices[1], vertices[2]);
    maxVertex = vec3(vertices[0], vertices[1], vertices[2]);

    for (uint i = 3; i < vertices.size(); i += 3) {
        minVertex.x = std::min(minVertex.x, vertices[i]);
        minVertex.y = std::min(minVertex.y, vertices[i + 1]);
        minVertex.z = std::min(minVertex.z, vertices[i + 2]);

        maxVertex.x = std::max(maxVertex.x, vertices[i]);
        maxVertex.y = std::max(maxVertex.y, vertices[i + 1]);
        maxVertex.z = std::max(maxVertex.z, vertices[i + 2]);
    }
}


uint Model::FindIndex(const string& name) {
    for (uint index = 0; index < gModels.size(); ++index)
        if (gModels[index].name == name)
            return index;

    cout << "Nu s-a gasit modelul " << name;
    exit(0);
}


void Model::CleanData(vector<float>& data) {
    data.clear();
}


void Model::CleanData(vector<uint>& data) {
    data.clear();
}


void Model::Clean() {
    for (uint i = 0; i < gModels.size(); ++i) {
        glDeleteVertexArrays(1, &gModels[i].VAO);

        for (uint j = 0; j < gModels[i].VBOs.size(); ++j)
            glDeleteBuffers(1, &gModels[i].VBOs[j]);
    }
}
