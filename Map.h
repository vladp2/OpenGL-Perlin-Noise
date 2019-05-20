#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED


using namespace std;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Map {
    public:
        static void SpawnObjects();
        static void CheckKeys();

    private:
        static int targetObj;
        static float targetObjDist;

        static void SpawnObject(const string, const string, const float);
        static void MoveObject(const float);
        static void DeleteObject();
};

#endif // MAP_H_INCLUDED
