#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED



struct Camera {
    struct Coord {
        double x;
        double y;
    } mouse;

    float mouseSpeed;
    float keySpeed;
    float horizAngle;
    float vertAngle;
    float fov;

    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 dir = glm::vec3(0.0f);
    glm::vec3 right = glm::vec3(0.0f);
    glm::vec3 up = glm::vec3(0.0f);

    float deltaTime;
    float lastFrame;

    void Init();
    void GetDeltaTime();
    bool CheckMouse();
    bool CheckKeys();
};

#endif // CAMERA_H_INCLUDED
