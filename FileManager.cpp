#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>

#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Algorithms.h"


using namespace std;
using namespace glm;


extern vector<Model> gModels;
extern vector<Texture> gTextures;


void ReadModel(string filePath) {
    filePath = "Game/models/" + filePath + ".obj";
    FILE *file = fopen(filePath.c_str(), "r");

	if (!file) {
        cout << "Impossible to open " << filePath << '\n';
		exit(0);
	}

	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;
	vector<uint> vertexIndices;
	vector<uint> uvIndices;
	vector<uint> normalIndices;
	uint vertexIndex[3];
	uint uvIndex[3];
	uint normalIndex[3];
	vec3 vertex;
	vec2 uv;
	vec3 normal;
	char line[128];
	int matches;

    while (true) {
        if (fscanf(file, "%s", line) == EOF)
            break;

        if (!strcmp(line, "v")) {
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            vertices.push_back(vertex);
        }

        else if (!strcmp(line, "vt")) {
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            uvs.push_back(uv);
        }

        else if (!strcmp(line, "vn")) {
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            normals.push_back(normal);
        }

        else if (!strcmp(line, "f")) {
            matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

            if (matches != 9) {
                printf("File can't be read by the parser\n");
                exit(0);
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);

            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);

            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    fclose(file);

    gModels.back().vertexCount = vertexIndices.size();

    gModels.back().positions.clear();
    gModels.back().UVs.clear();
    gModels.back().normals.clear();

    for (uint i = 0; i < gModels.back().vertexCount; ++i) {
        vertex = vertices[vertexIndices[i] - 1];
        uv = uvs[uvIndices[i] - 1];
        normal = normals[normalIndices[i] - 1];

        gModels.back().positions.push_back(vertex.x);
        gModels.back().positions.push_back(vertex.y);
        gModels.back().positions.push_back(vertex.z);

        //gModels.back().indices.push_back(vertexIndices[i]);

        gModels.back().UVs.push_back(uv.x);
        gModels.back().UVs.push_back(uv.y);

        gModels.back().normals.push_back(normal.x);
        gModels.back().normals.push_back(normal.y);
        gModels.back().normals.push_back(normal.z);
    }
}


void ReadTexture(string filePath) {
    filePath = "Game/textures/" + filePath + ".bmp";
	FILE *file = fopen(filePath.c_str(), "rb");

    if (!file) {
        cout << "Impossible to open " << filePath << '\n';
        exit(0);
    }

    uchar header[54];
    uint dataPos;
    uint imageSize;

    if (fread(header, 1, 54, file) != 54) {
        cout << "Not a correct BMP file\n";
        exit(0);
    }

    if (header[0] != 'B' || header[1] != 'M') {
        cout << "Not a correct BMP file\n";
        exit(0);
    }

    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);

    gTextures.back().width      = *(int*)&(header[0x12]);
    gTextures.back().height     = *(int*)&(header[0x16]);

    if (!imageSize)
        imageSize = gTextures.back().width * gTextures.back().height * 3;

    if (!dataPos)
        dataPos = 54;

    gTextures.back().data.resize(imageSize);
    fread(gTextures.back().data.data(), 1, imageSize, file);

	fclose(file);
}


void ReadModels() {
    string filePath = "Game/models.txt";
    ifstream file(filePath, ios::in);

	if (!file.is_open()) {
        cout << "Impossible to open " << filePath << '\n';
		exit(0);
	}

	string name;

    while (file >> name) {
        gModels.push_back(Model());
        gModels.back().name = name;
        gModels.back().nameMap[name] = gModels.size() - 1;

        ReadModel(name);
    }

    file.close();
}


void ReadTextures() {
    string filePath = "Game/textures.txt";
    ifstream file(filePath, ios::in);

	if (!file.is_open()) {
        cout << "Impossible to open " << filePath << '\n';
		exit(0);
	}

	string name;

    while (file >> name) {
        gTextures.push_back(Texture());
        gTextures.back().name = name;
        gTextures.back().nameMap[name] = gTextures.size() - 1;

        ReadTexture(name);
    }

    file.close();
}


uint ReadShaders(const string& vertexFile, const string& fragmentFile) {
	uint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	uint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	string vertexCode;
	ifstream vertexStream(vertexFile, ios::in);
	if (vertexStream.is_open()) {
		string line;

		while (getline(vertexStream, line))
			vertexCode += "\n" +line;

		vertexStream.close();
	}
	else {
		cout << "Impossible to open " << vertexFile << '\n';
		exit(0);
	}

	string fragmentCode;
	ifstream fragmentStream(fragmentFile, ios::in);
	if (fragmentStream.is_open()){
		string line = "";

		while (getline(fragmentStream, line))
			fragmentCode += "\n" + line;

		fragmentStream.close();
	}
	else {
		cout << "Impossible to open " << fragmentFile << '\n';
		exit(0);
	}

	int result = GL_FALSE;
	int infoLogLength;

	cout << "Compiling shader " << vertexFile << '\n';
	const char *vertexSource = vertexCode.c_str();

	glShaderSource(vertexShaderId, 1, &vertexSource, NULL);
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		vector<char> vertexErrorMsg(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &vertexErrorMsg[0]);
		cout << &vertexErrorMsg[0] << '\n';
	}


	cout << "Compiling shader " << fragmentFile << '\n';
	const char* fragmentSource = fragmentCode.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0){
		vector<char> fragmentErrorMsg(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &fragmentErrorMsg[0]);
		cout << &fragmentErrorMsg[0] << '\n';
	}

	cout << "Linking program\n";
	uint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		vector<char> programErrorMsg(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMsg[0]);
		cout << &programErrorMsg[0] << '\n';
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}
