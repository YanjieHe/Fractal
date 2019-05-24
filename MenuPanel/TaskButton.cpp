#include "TaskButton.hpp"
#include "Settings.hpp"
#include "XUtil.hpp"
#include <QProcess>
TaskButton::TaskButton(WindowHandle windowHandle, const QString& text,
                       QWidget* parent)
    : QPushButton(Truncate(text), parent), windowHandle{windowHandle}
{
    connect(this, SIGNAL(pressed()), this, SLOT(ClickTask()));
    setFixedHeight(Settings::panelHeight);
    setFont(Settings::font);
}

QString TaskButton::Truncate(QString text)
{
    if (text.size() > 20)
    {
        return text.left(10) + " ... " + text.right(10);
    }
    else
    {
        return text;
    }
}

void TaskButton::SetHandle(WindowHandle handle)
{
    this->windowHandle = handle;
}

void TaskButton::SetTitle(QString title)
{
    this->setText(Truncate(title));
}

void TaskButton::ClickTask()
{
    Display* display = XOpenDisplay(nullptr);
    Window current   = XUtil::CurrentActiveWindow(display);
    XCloseDisplay(display);
    if (current == windowHandle)
    {
        QProcess::startDetached("wmctrl -i -r " +
                                QString::number(windowHandle) +
                                " -b toggle,hidden");
    }
    else
    {
        QProcess::startDetached("wmctrl -i -a " +
                                QString::number(windowHandle));
    }
}
