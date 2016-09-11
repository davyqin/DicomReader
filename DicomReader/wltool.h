#ifndef WLTOOL_H
#define WLTOOL_H

class wlTool
{
public:
    wlTool();

    wlTool(int window, int level);

    void convert(unsigned char* pixel, int length);

private:
    int _window;
    int _level;
    int _sk;
    int _s1;
};

#endif // WLTOOL_H
