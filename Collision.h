#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


struct OBB;
class Object;
class Character;


class Collision {
    public:
        static string face;
        static float terrainHeight;

        static void CheckTerrain();
        static void CheckObjects();

        static uint RayCast(vec3, vec3, float&);
        static float CheckRay(const vec3, const vec3, const OBB&);
        static void CheckFaces(const vec3, const vec3, const OBB);

        static float PointPlaneDistance(const vec3&, const vec3&, const vec3&);
        static float PointPointDistance(const vec3&, const vec3&);
        static vec3 PlaneNormal(const vec3&, const vec3&, const vec3&);
        static bool PointPlaneNormal(const vec3&, const vec3&, const vec3&, const vec3&);
        static vec3 ClosestPointOBB(const vec3&, const OBB&);

    private:
        static float depth;
        static vec3 dir;
        static vec3 sign;

        static bool CheckOBB(const OBB&, const OBB&);
        static bool NaiveCheckOBB(const OBB&, const OBB&, const float);
        static void PushBack(Object&);

        static void NormalizeRay(vec3&);

        static bool IsOnObject();
        static bool IsOnGround(const Object&);
        static bool IsUnderGround(const Object&);
        static void SetOnGround(Object&);
};


#endif // COLLISION_H_INCLUDED
