#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include <map>


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


// DE FACUT CONSTRUCTORI
class Model {
    private:
        uint VAO;
        vector<uint> VBOs;
        vector<uint> EBOs;

    public:
        static map<string, uint> nameMap;

        string name;
        uint vertexCount;
        vector<float> positions;
        vector<uint> indices;
        vector<float> UVs;
        vector<float> normals;

        vec3 minVertex;
        vec3 maxVertex;

        static void Init();
        static uint FindIndex(const string&);
        static void Clean();

        uint GetVAO();
        uint GetVBO(const uint);
        vector<uint> GetVBOs();
        uint GetEBO(const uint);
        uint GetVertexCount();

        void SetVAO(const uint);
        void SetVBOs(const vector<uint>&);

        void Load();
        void LoadInstanced();

        void CreateVAO();
        void CreateVBO(const uint, const uint, const vector<float>&);
        void BindVAO();
        void EnableAttribute(const uint);
        void DisableAttribute(const uint);

        void SetMinMaxVertex(const vector<float>&);

        void UnbindVAO();
        void UnbindVBO();

        void CleanData(vector<float>&);
        void CleanData(vector<uint>&);
};


#endif // MODEL_H_INCLUDED
