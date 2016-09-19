#include "wltool.h"

wlTool::wlTool()
    :_window(0),_level(0) {}


wlTool::wlTool(int window, int level)
    :_window(window), _level(level) {}

void wlTool::convert(unsigned char* pixel, int length) {
    const double halfWindow = _window / 2.0;
    while (length-- > 0)
    {
      if (*pixel < (_level - halfWindow)) {
            *pixel = 0;
        }
      else if (*pixel > (_level + halfWindow)) {
            *pixel = 0;
        }
        else {
          if (_window == 0) 
            *pixel = 255;
          else
            *pixel = (*pixel - _level) * 255.0 /_window;
        }
        pixel++;
    }
}

void wlTool::convertShortPixel(unsigned short* pixel, int length)
{
  const double halfWindow = _window / 2.0;
  while (length-- > 0)
  {
    if (*pixel < (_level - halfWindow)) {
      *pixel = 0;
    }
    else if (*pixel > (_level + halfWindow)) {
      *pixel = 0;
    }
    else {
      if (_window == 0)
        *pixel = 65535;
      else
        *pixel = (*pixel - _level) * 65535.0 / _window;
    }
    pixel++;
  }
}
