#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED


struct Window {
    GLFWwindow *handle;

    GLfloat width;
    GLfloat height;

    char title[30];

    void Init();
    bool IsActive();
};


#endif // WINDOW_H_INCLUDED
