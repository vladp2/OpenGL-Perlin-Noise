#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED


using namespace std;


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned uint;
typedef unsigned long long ullint;
typedef long long llint;


class Window {
    private:
        static GLFWwindow* handle;

        static string title;

        static ushort width;
        static ushort height;

        static void SetTitle(const string name);
        static void SetWidth(const int value);
        static void SetHeight(const int value);


    public:
        static void Init();

        static GLFWwindow* GetHandle();
        static string GetTitle();
        static ushort GetWidth();
        static ushort GetHeight();

        static bool IsActive();
};


#endif // WINDOW_H_INCLUDED
