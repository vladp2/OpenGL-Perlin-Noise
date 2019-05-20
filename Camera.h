#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


#define ef else if

#define PI 3.1415926535897932f
#define RAD 57.29577951308232f


using namespace std;
using namespace glm;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


const float PIdiv2 = PI / 2.0f;
const float PImult2 = PI * 2.0f;


class Camera {
    private:
        static double mouseX;
        static double mouseY;
        static double centerX;
        static double centerY;
        static float speed;
        static float fov;
        static float _ratio;
        static float nearPlane;
        static float farPlane;
        static vec3 pos;
        static vec3 dir;
        static vec3 right;
        static vec3 up;
        static float playerDist;
        static float pitch;
        static float yaw;
        static float roll;

        static void NormalizeAngle(double& angle);
        static void NormalizeAngle(float& angle);

    public:
        static mat4 projMatrix;
        static mat4 viewMatrix;
        static float frustum[6][4];

        static void Init();
        static bool CheckMouse();
        static void CheckKeys();
        static void MoveFPS();
        static void Move360();

        static void SetThirdPerson();

        static double GetMouseX();
        static double GetMouseY();
        static float GetSpeed();
        static float GetPitch();
        static float GetYaw();
        static float GetRoll();
        static float GetFOV();
        static float GetRatio();
        static float GetNearPlane();
        static float GetFarPlane();
        static vec3 GetPos();
        static vec3 GetDir();
        static vec3 GetRight();
        static vec3 GetUp();
        static float GetPlayerDist();

        static void CalcProjMatrix();
        static void CalcViewMatrix();

        static void ExtractFrustum();
        static bool SphereInFrustum(const vec3, const float);

        static void Pitch();
        static void Yaw();
        static void Roll(const float);
};

#endif // CAMERA_H_INCLUDED
