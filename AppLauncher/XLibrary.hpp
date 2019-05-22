#ifndef XLIBRARY_HPP
#define XLIBRARY_HPP
#include <QString>

#include <X11/Xlib.h>
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
class XLibrary
{
  public:
    static Window* GetWindowList(Display* display, unsigned long* length);
    static QString GetWindowName(Display* display, Window window);
    static Window CurrentActiveWindow(Display* display);
};

#endif // XLIBRARY_HPP
