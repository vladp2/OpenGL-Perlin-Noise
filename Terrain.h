#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Model;
class Texture;


class Terrain {
    public:
        static uint chunkCount;
        static uint chunkDist;
        static uint chunkLength;
        static uint squareLength;
        static int centerChunkZ;
        static int centerChunkX;
        static vector<vector<Model>> models;
        static vector<vector<Texture>> textures;

        static void Generate();
        static void GenerateChunkModel(Model&, const int, const int);
        static void GenerateChunkTexture(Texture&);

        static void AddVertex(Model&, const vec3, const vec2, const vec3);

        static void Update();
        static void UpdateMargins();
        static void UpdateUpChunks();
        static void UpdateDownChunks();
        static void UpdateRightChunks();
        static void UpdateLeftChunks();
        static void UpdateChunk(Model&, const int, const int);
        static void UpdateBuffers(Model&);
};


#endif // TERRAIN_H_INCLUDED
