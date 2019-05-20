#ifndef LOADSHADER_H_INCLUDED
#define LOADSHADER_H_INCLUDED



typedef unsigned char uchar;
typedef unsigned int uint;


bool ReadVertices(std::vector<float>&, std::string);

bool ReadTexture(std::string);

uint ReadShaders(std::string, std::string);

#endif // LOADSHADER_H_INCLUDED
