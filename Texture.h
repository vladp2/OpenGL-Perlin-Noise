#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED


#include <map>


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Texture {
    private:
        uint ID;

    public:
        static map<string, uint> nameMap;

        string name;
        vector<uchar> data;
        uint width;
        uint height;

        static void Init();
        static uint FindIndex(const string&);
        static void Clean();

        void Load();

        uint GetID();
        void SetID(const uint);

        void CleanData();
};


#endif // TEXTURE_H_INCLUDED
