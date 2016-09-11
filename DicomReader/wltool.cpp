#include "wltool.h"

wlTool::wlTool()
    :_window(0),_level(0),_sk(255),_s1(0){}


wlTool::wlTool(int window, int level)
    :_window(window), _level(level),
     _sk(255),_s1(0){
}

void wlTool::convert(unsigned char* pixel, int length) {
    while (length-- > 0)
    {
        if (*pixel < _level) {
            *pixel = 0;
        }
        else if (*pixel > (_level + _window)) {
            *pixel = 0;
        }
        else {
            *pixel = (*pixel - _level) * (_sk - _s1)/_window;
        }
        pixel++;
    }
}
