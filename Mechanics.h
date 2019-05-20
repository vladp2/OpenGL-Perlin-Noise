#ifndef MECHANICS_H_INCLUDED
#define MECHANICS_H_INCLUDED


using namespace glm;


#define PI 3.1415926535897932f
#define RAD 57.29577951308232f


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Mechanics {
    public:
        static vec3 direction;

        static void CheckKeys();
        static void Move();
        static double GetDeltaTime();
        static bool IsOnGround();
        static bool IsOnObject();
        static void SetOnGround(const bool);
        static void SetOnObject(const bool);

    private:
        static double deltaTime;
        static double lastFrame;
        static float moveSpeed;
        static bool moveForward;
        static bool moveBackward;
        static bool moveLeft;
        static bool moveRight;
        static bool moveUp;
        static bool moveDown;
        static bool isMoving;
        static bool isJumping;
        static bool onGround;
        static bool onObject;
        static float force;

        static void CalcDeltaTime();
        static void ApplyGravity();
        static void Rotate();
        static void MoveForward(const float);
        static void MoveBackward(const float);
        static void MoveLeft(const float);
        static void MoveRight(const float);
        static void MoveUp(const float);
        static void MoveDown(const float);
};


#endif // MECHANICS_H_INCLUDED
