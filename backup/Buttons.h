#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED


struct Buttons {
    char keybdKey[350];
    char mouseKey[350];

    bool CheckKeybd(GLuint, int);
    bool CheckMouse(GLuint, int);
};


#endif // BUTTONS_H_INCLUDED
