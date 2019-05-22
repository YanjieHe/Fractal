#include "TaskButton.hpp"
#include <QDebug>
#include <QProcess>
#include <iostream>

#include "XLibrary.hpp"
using namespace std;
TaskButton::TaskButton(WindowID window, const QString& text, QWidget* parent)
    : QPushButton(Truncate(text), parent), window{window}
{
    connect(this, SIGNAL(pressed()), this, SLOT(ClickTask()));
    //    setFlat(true);
    setStyleSheet("border:none;"
                  "border-bottom:3px solid rgb(0,131,221);"
                  "color: rgb(255, 255, 255)");
    auto font = QFont("Roboto 12");
    font.setPixelSize(14);
    this->setFont(font);
}

QString TaskButton::Truncate(QString text)
{
    if (text.size() > 30)
    {
        return text.left(15) + " ... " + text.right(15);
    }
    else
    {
        return text;
    }
}

void TaskButton::ClickTask()
{
    Display* display = XOpenDisplay(nullptr);
    Window current   = XLibrary::CurrentActiveWindow(display);
    XCloseDisplay(display);
    cout << "current active: " << hex << current << ", this window: " << hex
         << window << endl;
    if (current == window)
    {
        QProcess::startDetached("wmctrl -i -r " + QString::number(window) +
                                " -b toggle,hidden");
    }
    else
    {
        QProcess::startDetached("wmctrl -i -a " + QString::number(window));
    }
}
