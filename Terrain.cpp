#include <iostream>
#include <ctime>
#include <windows.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Terrain.h"
#include "Object.h"
#include "Camera.h"
#include "Render.h"
#include "Character.h"
#include "Collision.h"
#include "Algorithms.h"


using namespace std;
using namespace glm;


extern vector<Model> gModels;
extern vector<Texture> gTextures;
extern vector<Object> gObjects;
extern Character player;


uint Terrain::chunkCount = 7;
uint Terrain::chunkLength = 32;
uint Terrain::squareLength = 5;
int Terrain::centerChunkZ;
int Terrain::centerChunkX;
vector<vector<Model>> Terrain::models(chunkCount, vector<Model>(chunkCount));
vector<vector<Texture>> Terrain::textures(chunkCount, vector<Texture>(chunkCount));


PerlinNoise noise(0.2, 0.02, 70, 3, 1);


void Terrain::Generate() {
    centerChunkZ = Camera::GetPos().z / (chunkLength * squareLength);
    centerChunkX = Camera::GetPos().x / (chunkLength * squareLength);

    int chunkZ;
    int chunkX;
    uint z;
    uint x;

    for (z = 0, chunkZ = centerChunkZ - (chunkCount >> 1); z < chunkCount; ++z, ++chunkZ)
    for (x = 0, chunkX = centerChunkX - (chunkCount >> 1); x < chunkCount; ++x, ++chunkX) {
        GenerateChunkModel(models[z][x], chunkZ, chunkX);
        GenerateChunkTexture(textures[z][x]);
    }
}


void Terrain::GenerateChunkModel(Model& model, const int chunkZ, const int chunkX) {
    model.name = "Terrain Chunk";
    model.vertexCount = chunkLength * chunkLength * 6;

    model.positions.clear();
    model.UVs.clear();
    model.normals.clear();

    vector<vector<vec3>> positions(chunkLength + 1, vector<vec3>(chunkLength + 1));

    int positionZ;
    int positionX;
    uint z;
    uint x;

    for (z = 0, positionZ = chunkZ * chunkLength * squareLength; z <= chunkLength; ++z, positionZ += squareLength)
    for (x = 0, positionX = chunkX * chunkLength * squareLength; x <= chunkLength; ++x, positionX += squareLength) {
        positions[z][x].x = positionX;
        positions[z][x].y = noise.GetHeight(abs(positionX), abs(positionZ));
        positions[z][x].z = positionZ;
    }

    vec3 position;
    vec2 UV;
    vec3 normal;

    for (uint z = 0; z < chunkLength; ++z)
    for (uint x = 0; x < chunkLength; ++x) {
        // vertex 1
        position = positions[z][x];
        UV = vec2(0, 1);
        normal = Collision::PlaneNormal(positions[z][x], positions[z + 1][x], positions[z + 1][x + 1]);

        AddVertex(model, position, UV, normal);

        // vertex 2
        position = positions[z + 1][x];
        UV = vec2(0, 0);

        AddVertex(model, position, UV, normal);

        // vertex 3
        position = positions[z + 1][x + 1];
        UV = vec2(1, 0);

        AddVertex(model, position, UV, normal);

        // vertex 4
        position = positions[z][x];
        UV = vec2(0, 1);
        normal = Collision::PlaneNormal(positions[z][x], positions[z + 1][x + 1], positions[z][x + 1]);

        AddVertex(model, position, UV, normal);

        // vertex 5
        position = positions[z + 1][x + 1];
        UV = vec2(1, 0);

        AddVertex(model, position, UV, normal);

        // vertex 6
        position = positions[z][x + 1];
        UV = vec2(1, 1);

        AddVertex(model, position, UV, normal);
    }

    model.Load();
}


void Terrain::GenerateChunkTexture(Texture& texture) {
    texture.SetID(gTextures[Texture::FindIndex("cube2")].GetID());
}


void Terrain::AddVertex(Model& model, const vec3 position, const vec2 UV, const vec3 normal) {
    model.positions.push_back(position.x);
    model.positions.push_back(position.y);
    model.positions.push_back(position.z);

    model.UVs.push_back(UV.x);
    model.UVs.push_back(UV.y);

    model.normals.push_back(normal.x);
    model.normals.push_back(normal.y);
    model.normals.push_back(normal.z);
}


void Terrain::Update() {
    static int last_centerChunkZ = -1337;
    static int last_centerChunkX = -1337;

    centerChunkZ = Camera::GetPos().z / (chunkLength * squareLength);
    centerChunkX = Camera::GetPos().x / (chunkLength * squareLength);

    if (centerChunkZ - last_centerChunkZ == -1 &&
        centerChunkX - last_centerChunkX == 0)
    {
        //cout << "sus\n";

        UpdateUpChunks();
    }

    ef (centerChunkZ - last_centerChunkZ == 1 &&
        centerChunkX - last_centerChunkX == 0)
    {
        //cout << "jos\n";

        UpdateDownChunks();
    }

    ef (centerChunkZ - last_centerChunkZ == 0 &&
        centerChunkX - last_centerChunkX == 1)
    {
        //cout << "dreapta\n";

        UpdateRightChunks();
    }

    ef (centerChunkZ - last_centerChunkZ == 0 &&
        centerChunkX - last_centerChunkX == -1)
    {
        //cout << "stanga\n";

        UpdateLeftChunks();
    }

    last_centerChunkZ = centerChunkZ;
    last_centerChunkX = centerChunkX;
}


void Terrain::UpdateUpChunks() {
    vector<Model> temp(chunkCount);
    int chunkZ;
    int chunkX;
    uint z;
    uint x;

    for (x = 0; x < chunkCount; ++x) {
        temp[x].SetVAO(models[chunkCount - 1][x].GetVAO());
        temp[x].SetVBOs(models[chunkCount - 1][x].GetVBOs());
    }

    for (z = chunkCount - 1; z > 0; --z)
    for (x = 0; x < chunkCount; ++x) {
        models[z][x].SetVAO(models[z - 1][x].GetVAO());
        models[z][x].SetVBOs(models[z - 1][x].GetVBOs());
    }

    for (x = 0; x < chunkCount; ++x) {
        models[0][x].SetVAO(temp[x].GetVAO());
        models[0][x].SetVBOs(temp[x].GetVBOs());
    }

    chunkZ = centerChunkZ - (chunkCount >> 1);
    chunkX = centerChunkX - (chunkCount >> 1);

    for (x = 0; x < chunkCount; ++x, ++chunkX) {
        UpdateChunk(models[0][x], chunkZ, chunkX);
        UpdateBuffers(models[0][x]);
    }
}


void Terrain::UpdateDownChunks() {
    vector<Model> temp(chunkCount);
    int chunkZ;
    int chunkX;
    uint z;
    uint x;

    for (x = 0; x < chunkCount; ++x) {
        temp[x].SetVAO(models[0][x].GetVAO());
        temp[x].SetVBOs(models[0][x].GetVBOs());
    }

    for (z = 0; z < chunkCount - 1; ++z)
    for (x = 0; x < chunkCount; ++x) {
        models[z][x].SetVAO(models[z + 1][x].GetVAO());
        models[z][x].SetVBOs(models[z + 1][x].GetVBOs());
    }

    for (x = 0; x < chunkCount; ++x) {
        models[chunkCount - 1][x].SetVAO(temp[x].GetVAO());
        models[chunkCount - 1][x].SetVBOs(temp[x].GetVBOs());
    }

    chunkZ = centerChunkZ + (chunkCount >> 1);
    chunkX = centerChunkX - (chunkCount >> 1);

    for (x = 0; x < chunkCount; ++x, ++chunkX) {
        UpdateChunk(models[chunkCount - 1][x], chunkZ, chunkX);
        UpdateBuffers(models[chunkCount - 1][x]);
    }
}


void Terrain::UpdateRightChunks() {
    vector<Model> temp(chunkCount);
    int chunkZ;
    int chunkX;
    uint z;
    uint x;

    for (z = 0; z < chunkCount; ++z) {
        temp[z].SetVAO(models[z][0].GetVAO());
        temp[z].SetVBOs(models[z][0].GetVBOs());
    }

    for (z = 0; z < chunkCount; ++z)
    for (x = 0; x < chunkCount - 1; ++x) {
        models[z][x].SetVAO(models[z][x + 1].GetVAO());
        models[z][x].SetVBOs(models[z][x + 1].GetVBOs());
    }

    for (z = 0; z < chunkCount; ++z) {
        models[z][chunkCount - 1].SetVAO(temp[z].GetVAO());
        models[z][chunkCount - 1].SetVBOs(temp[z].GetVBOs());
    }

    chunkZ = centerChunkZ - (chunkCount >> 1);
    chunkX = centerChunkX + (chunkCount >> 1);

    for (z = 0; z < chunkCount; ++z, ++chunkZ) {
        UpdateChunk(models[z][chunkCount - 1], chunkZ, chunkX);
        UpdateBuffers(models[z][chunkCount - 1]);
    }
}


void Terrain::UpdateLeftChunks() {
    vector<Model> temp(chunkCount);
    int chunkZ;
    int chunkX;
    uint z;
    uint x;

    for (z = 0; z < chunkCount; ++z) {
        temp[z].SetVAO(models[z][chunkCount - 1].GetVAO());
        temp[z].SetVBOs(models[z][chunkCount - 1].GetVBOs());
    }

    for (z = 0; z < chunkCount; ++z)
    for (x = chunkCount - 1; x > 0; --x) {
        models[z][x].SetVAO(models[z][x - 1].GetVAO());
        models[z][x].SetVBOs(models[z][x - 1].GetVBOs());
    }

    for (z = 0; z < chunkCount; ++z) {
        models[z][0].SetVAO(temp[z].GetVAO());
        models[z][0].SetVBOs(temp[z].GetVBOs());
    }

    chunkZ = centerChunkZ - (chunkCount >> 1);
    chunkX = centerChunkX - (chunkCount >> 1);

    for (z = 0; z < chunkCount; ++z, ++chunkZ) {
        UpdateChunk(models[z][0], chunkZ, chunkX);
        UpdateBuffers(models[z][0]);
    }
}


void Terrain::UpdateChunk(Model& model, const int chunkZ, const int chunkX) {
    model.positions.clear();
    model.UVs.clear();
    model.normals.clear();

    vector<vector<vec3>> positions(chunkLength + 1, vector<vec3>(chunkLength + 1));

    int positionZ;
    int positionX;
    uint z;
    uint x;

    for (z = 0, positionZ = chunkZ * chunkLength * squareLength; z <= chunkLength; ++z, positionZ += squareLength)
    for (x = 0, positionX = chunkX * chunkLength * squareLength; x <= chunkLength; ++x, positionX += squareLength) {
        positions[z][x].x = positionX;
        positions[z][x].y = noise.GetHeight(abs(positionX), abs(positionZ));
        positions[z][x].z = positionZ;
    }

    vec3 position;
    vec2 UV;
    vec3 normal;

    for (uint z = 0; z < chunkLength; ++z)
    for (uint x = 0; x < chunkLength; ++x) {
        // vertex 1
        position = positions[z][x];
        UV = vec2(0, 1);
        normal = Collision::PlaneNormal(positions[z][x], positions[z + 1][x], positions[z + 1][x + 1]);

        AddVertex(model, position, UV, normal);

        // vertex 2
        position = positions[z + 1][x];
        UV = vec2(0, 0);

        AddVertex(model, position, UV, normal);

        // vertex 3
        position = positions[z + 1][x + 1];
        UV = vec2(1, 0);

        AddVertex(model, position, UV, normal);

        // vertex 4
        position = positions[z][x];
        UV = vec2(0, 1);
        normal = Collision::PlaneNormal(positions[z][x], positions[z + 1][x + 1], positions[z][x + 1]);

        AddVertex(model, position, UV, normal);

        // vertex 5
        position = positions[z + 1][x + 1];
        UV = vec2(1, 0);

        AddVertex(model, position, UV, normal);

        // vertex 6
        position = positions[z][x + 1];
        UV = vec2(1, 1);

        AddVertex(model, position, UV, normal);
    }
}


void Terrain::UpdateBuffers(Model& model) {
    model.BindVAO();

    glBindBuffer(GL_ARRAY_BUFFER, model.GetVBO(0));
    glBufferData(GL_ARRAY_BUFFER, model.vertexCount * 3 * sizeof(float), model.positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, model.GetVBO(2));
    glBufferData(GL_ARRAY_BUFFER, model.vertexCount * 3 * sizeof(float), model.normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    model.UnbindVAO();

    model.positions.clear();
    model.normals.clear();
}
