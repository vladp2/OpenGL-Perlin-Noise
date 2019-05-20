#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


struct OBB {
    vec3 center;
    vec3 rotationMatrix[3];
    vec3 halfLength;
};


class Object {
    private:
        uint modelID;
        uint textureID;

    public:
        mat4 positionMatrix;
        mat4 rotationMatrix;
        mat4 scalingMatrix;

        mat4 modelMatrix;
        OBB box;

        void Create(const string&, const string&, const vec3);
        void Create(const uint, const uint, const vec3);

        void SetPos(const vec3);
        void SetOBB();
        void ResetOBB();

        void Place(const vec3);
        void Rotate(const vec3);
        void Face(const vec3);
        void Scale(const vec3);

        vec3 GetEulerAngles() const;

        uint GetModelID() const;
        void SetModelID(const uint);
        uint GetTextureID();
        void SetTextureID(const uint);

        void UpdateOBB();
};


#endif // OBJECT_H_INCLUDED
