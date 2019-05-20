#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <string>


struct Map {
    GLuint programId;

    GLuint targetObj;
    float targetObjDist;

    bool LoadModels(std::string);
    bool LoadTextures(std::string);
    bool LoadShaders();
    void GenObjects();
    void CreateObject(std::string, glm::vec3 = glm::vec3(0.0f));
    void EnableZBuffer();
    void DrawObjects();
    void PlaceObject(std::string, std::string, float);
    GLuint RayCast();
    void MoveObject();
    void CheckKeys();
    void DeleteObject();
};

#endif // MAP_H_INCLUDED
