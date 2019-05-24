#include "XUtil.hpp"
#include <X11/Xatom.h>
#include <glib.h>

gchar* XUtil::GetProperty(Display* disp, Window win, Atom xa_prop_type,
                          const gchar* prop_name, unsigned long* size)
{
    Atom xa_prop_name;
    Atom xa_ret_type;
    int ret_format;
    unsigned long ret_nitems;
    unsigned long ret_bytes_after;
    unsigned long tmp_size;
    unsigned char* ret_prop;
    gchar* ret;

    xa_prop_name = XInternAtom(disp, prop_name, False);

    if (XGetWindowProperty(disp, win, xa_prop_name, 0, 1024, False,
                           xa_prop_type, &xa_ret_type, &ret_format, &ret_nitems,
                           &ret_bytes_after, &ret_prop) != Success)
    {
        return nullptr;
    }
    if (xa_ret_type != xa_prop_type)
    {
        XFree(ret_prop);
        return nullptr;
    }

    /* null terminate the result to make string handling easier */
    tmp_size = static_cast<unsigned long>(ret_format / 8) * ret_nitems;
    ret		 = reinterpret_cast<gchar*>(g_malloc(tmp_size + 1));

    memcpy(ret, ret_prop, tmp_size);
    ret[tmp_size] = '\0';
    if (size)
    {
        *size = tmp_size;
    }

    XFree(ret_prop);
    return ret;
}
Window* XUtil::GetWindowList(Display* display, unsigned long* length)
{
    Atom property = XInternAtom(display, "_NET_CLIENT_LIST", false);
    Atom type;
    int format;
    unsigned long remain;
    unsigned char* data = nullptr;

    if (XGetWindowProperty(display, XDefaultRootWindow(display), property, 0,
                           1024, false, XA_WINDOW, &type, &format, length,
                           &remain, &data) >= Success)
    {
        return reinterpret_cast<Window*>(data);
    }
    else
    {
        return nullptr;
    }
}

std::string XUtil::GetWindowName(Display* display, Window window)
{
    GCharUniquePtr windowName(
        GetProperty(display, window, XA_STRING, "WM_NAME", nullptr),
        freeGCharPtr);
    GCharUniquePtr netWindowName(
        GetProperty(display, window, XInternAtom(display, "UTF8_STRING", False),
                    "_NET_WM_NAME", nullptr),
        freeGCharPtr);
    if (netWindowName)
    {
        GCharUniquePtr title_utf8(g_strdup(netWindowName.get()), freeGCharPtr);
        return title_utf8.get();
    }
    else
    {
        if (windowName)
        {
            GCharUniquePtr title_utf8(g_locale_to_utf8(windowName.get(), -1,
                                                       nullptr, nullptr,
                                                       nullptr),
                                      freeGCharPtr);
            return title_utf8.get();
        }
        else
        {
            return "";
        }
    }
}

Window XUtil::CurrentActiveWindow(Display* display)
{
    Atom property = XInternAtom(display, "_NET_ACTIVE_WINDOW", false);
    Atom type;
    int format = 32;
    unsigned long remain;
    unsigned char* data  = nullptr;
    unsigned long length = 0;

    if (XGetWindowProperty(display, XDefaultRootWindow(display), property, 0,
                           1024, false, XA_WINDOW, &type, &format, &length,
                           &remain, &data) >= Success)
    {
        unsigned long longProperty = static_cast<unsigned long>(data[0]) +
                                     static_cast<unsigned long>(data[1] << 8) +
                                     static_cast<unsigned long>(data[2] << 16) +
                                     static_cast<unsigned long>(data[3] << 24);
        XFree(data);
        return static_cast<Window>(longProperty);
    }
    else
    {
        return 0;
    }
}
