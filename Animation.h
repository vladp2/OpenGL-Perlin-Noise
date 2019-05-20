#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Animation {
    public:
        static void CheckKeys();

    private:
        static void StartHitting();
        static void StopHitting();

        static bool isHitting;
};


#endif // ANIMATION_H_INCLUDED
