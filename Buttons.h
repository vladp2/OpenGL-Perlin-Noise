#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#define ef else if


typedef unsigned int uint;



class Buttons {
    public:
        Buttons();
        ~Buttons();

        static bool CheckKeybd(const uint, const uint);
        static bool CheckMouse(const uint, const uint);

    private:
        static char* keybdKey;
        static char* mouseKey;
};


#endif // BUTTONS_H_INCLUDED
