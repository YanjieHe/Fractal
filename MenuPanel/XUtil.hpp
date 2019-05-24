#ifndef XUTIL_HPP
#define XUTIL_HPP
#include <glib.h>
#include <memory>
#include <string>

#include <X11/Xlib.h>
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose

auto freeGCharPtr	= [](gchar* p) { g_free(p); };
using GCharUniquePtr = std::unique_ptr<gchar, decltype(freeGCharPtr)>;

class XWindowException
{
};

class XUtil
{
  public:
    static gchar* GetProperty(Display* disp, Window win, Atom xa_prop_type,
                              const gchar* prop_name, unsigned long* size);
    static Window* GetWindowList(Display* display, unsigned long* length);
    static std::string GetWindowName(Display* display, Window window);
    static Window CurrentActiveWindow(Display* display);
};

#endif // XUTIL_HPP
