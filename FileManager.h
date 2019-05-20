#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED


using namespace std;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


void ReadModels();
void ReadTextures();
void ReadModel(string);
void ReadTexture(string);
uint ReadShaders(const string&, const string&);


#endif // FILEMANAGER_H_INCLUDED
