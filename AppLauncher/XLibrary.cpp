#include "XLibrary.hpp"
#include <QTextCodec>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <glib.h>
#include <iostream>

using std::cout;
using std::endl;
Window* XLibrary::GetWindowList(Display* display, unsigned long* length)
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

static gchar* get_property(Display* disp, Window win, Atom xa_prop_type,
                           gchar* prop_name, unsigned long* size)
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

    /* MAX_PROPERTY_VALUE_LEN / 4 explanation (XGetWindowProperty manpage):
     *
     * long_length = Specifies the length in 32-bit multiples of the
     *               data to be retrieved.
     */
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
QString XLibrary::GetWindowName(Display* display, Window window)
{
    gchar* title_utf8;
    gchar* wm_name;
    gchar* net_wm_name;

    wm_name		= get_property(display, window, XA_STRING, "WM_NAME", nullptr);
    net_wm_name = get_property(display, window,
                               XInternAtom(display, "UTF8_STRING", False),
                               "_NET_WM_NAME", nullptr);

    if (net_wm_name)
    {
        title_utf8	= g_strdup(net_wm_name);
        QString title = QString(title_utf8);
        free(title_utf8);
        g_free(wm_name);
        g_free(net_wm_name);
        return title;
    }
    else
    {
        if (wm_name)
        {
            title_utf8 =
                g_locale_to_utf8(wm_name, -1, nullptr, nullptr, nullptr);
            QString title = QString(title_utf8);
            free(title_utf8);
            g_free(wm_name);
            g_free(net_wm_name);
            return title;
        }
        else
        {
            title_utf8 = nullptr;
            return "";
        }
    }

    //    XTextProperty text;
    //    if (XGetWMName(display, window, &text) >= Success)
    //    {
    //        auto title_utf8 =
    //        g_locale_to_utf8(reinterpret_cast<char*>(text.value),
    //                                           -1, nullptr, nullptr, nullptr);
    //        QString title   = QString::fromUtf8(title_utf8);
    //        free(title_utf8);
    //        XFree(text.value);
    //        return title;
    //    }
    //    else
    //    {
    //        return "";
    //    }
}

Window XLibrary::CurrentActiveWindow(Display* display)
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
        unsigned long longProperty =
            data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
        XFree(data);
        return static_cast<Window>(longProperty);
    }
    else
    {
        return 0;
    }
}
