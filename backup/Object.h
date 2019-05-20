#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED


typedef unsigned char uchar;
typedef unsigned int uint;


struct Model {
    std::string name;
    float *data;
    uint size;
};

struct Texture {
    std::string name;
    uchar *data;
    uint width;
    uint height;
};

struct Object {
    bool active = false;

    GLuint vertexAO;

    std::string name;
    GLuint vertexCount = 0;
    GLuint textureId;

    glm::vec3 minVertex;
    glm::vec3 maxVertex;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 projMatrix;
    glm::mat4 viewMatrix;

    glm::mat4 uniformMatrix;
    GLuint uniformHandle;

    bool Init(std::string, std::string, glm::vec3);
    void GetBoundBox(Model);
    void Trans(glm::vec3);
    void Rotate(float, glm::vec3);
    void Scale(glm::vec3);
    void Proj(float, float, float, float);
    void Move(glm::vec3, glm::vec3, glm::vec3);
    void Draw();
    void Delete();
};


#endif // OBJECT_H_INCLUDED
