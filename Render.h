#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED


#include "Model.h"
#include "Texture.h"
#include "Object.h"


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Render {
    private:
        static uint programID;

        static void UpdateShaders(const mat4&, const mat4&, const int, const int);

        static void SetUniform(const string, const int);
        static void SetUniform(const string, const mat4);
        static void SetUniform(const string, const vec3);
        static void SetUniform(const string, const float);

    public:
        static void DrawObjects();
        static void DrawObjectsInstanced();
        static void DrawCharacters();
        static void DrawSkybox();
        static void DrawHUD();
        static void DrawTerrain();

        static void Draw(Model&, Texture&);

        static void LoadShaders();

        static void EnableZBuffer();
        static void EnableCullFace();
        static void ClearScreen();

        static uint GetProgramID();

        static void Clean();

        friend void Function();
};


#endif // RENDER_H_INCLUDED
