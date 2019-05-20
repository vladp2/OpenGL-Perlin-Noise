#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>

using namespace std;



extern Model model[100];
extern GLuint modelCount;

extern Texture tex[100];
extern GLuint texCount;



bool ReadModel(std::string vertexFile) {
	std::ifstream vertexStream(vertexFile, std::ios::in);
	if (!vertexStream.is_open()) {
        std::cout << "Impossible to open " << vertexFile << '\n';
		return false;
	}

	std::vector<GLuint> vec;

    float value;
    while(vertexStream >> value)
        vec.push_back(value);

    vertexStream.close();

    model[modelCount] = new float[vec.size()];
    copy(vec.begin(), vec.end(), model[modelCount]);

    return true;
}


bool ReadTexture(std::string textureFile) {
    textureFile = "models/textures/" + textureFile + ".bmp";

	FILE *file = fopen(textureFile.c_str(), "rb");
    if (!file) {
        std::cout << "Impossible to open " << textureFile << '\n';
        return false;
    }

    uchar header[54];
    uint dataPos;
    uint imageSize;

    if (fread(header, 1, 54, file) != 54) {
        std::cout << "Not a correct BMP file\n";
        return false;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        std::cout << "Not a correct BMP file\n";
        return false;
    }

    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    tex[texCount].width      = *(int*)&(header[0x12]);
    tex[texCount].height     = *(int*)&(header[0x16]);

    if (!imageSize)
        imageSize = tex[texCount].width * tex[texCount].height * 3;
    if (!dataPos)
        dataPos=54;

    tex[texCount].data = new unsigned char[imageSize];
    fread(tex[texCount].data, 1, imageSize, file);

	fclose(file);

	return true;
}


GLuint ReadShaders(string vertexFile, string fragmentFile) {
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	string vertexCode;
	ifstream vertexStream(vertexFile, ios::in);
	if (vertexStream.is_open()) {
		string line = "";

		while(getline(vertexStream, line))
			vertexCode += "\n" +line;

		vertexStream.close();
	}
	else {
		cout << "Impossible to open " << vertexFile << '\n';
		return 0;
	}

	string fragmentCode;
	ifstream fragmentStream(fragmentFile, ios::in);
	if (fragmentStream.is_open()){
		string line = "";

		while(getline(fragmentStream, line))
			fragmentCode += "\n" + line;

		fragmentStream.close();
	}
	else {
		cout << "Impossible to open " << fragmentFile << '\n';
		return 0;
	}

	GLint result = GL_FALSE;
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
	GLuint programId = glCreateProgram();
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
