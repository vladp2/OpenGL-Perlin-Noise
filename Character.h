#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include "Object.h"


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


struct OBB;
class Object;


class Character {
    public:
        vector<Object> body;
        vec3 pos;
        mat4 modelMatrix;
        OBB box;

        Character();
        void Init();
        void Update();
        void UpdateOBB();
};


#endif // CHARACTER_H_INCLUDED
