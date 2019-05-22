#include "TaskBar.hpp"
#include "TaskButton.hpp"
#include "XLibrary.hpp"
#include <QTextCodec>
#include <iostream>
using namespace std;

TaskBar::TaskBar() : timer(this)
{
    this->setContentsMargins(0, 0, 0, 0);
    setSpacing(20);
    connect(&timer, SIGNAL(timeout()), this, SLOT(CheckWindows()));
    timer.start(waitingTime);
}

void TaskBar::BindButtons()
{
    for (auto button : buttonList)
    {
        delete button;
    }
    buttonList.clear();
    for (auto window : windowList)
    {
        TaskButton* button = new TaskButton(window.id, window.title);
        button->setFixedHeight(30);
        buttonList.push_back(button);
        this->addWidget(button);
    }
}

void TaskBar::CheckWindows()
{
    timer.stop();
    Display* display  = XOpenDisplay(nullptr);
    unsigned long len = 0;
    auto windows	  = XLibrary::GetWindowList(display, &len);
    windowList.clear();
    for (unsigned long i = 0; i < len; i++)
    {
        Window id = windows[i];
        //        cout << hex << id << endl;
        QString title = XLibrary::GetWindowName(display, windows[i]);
        windowList.push_back(WinInfo(id, title));
    }
    XFree(windows);
    BindButtons();
    XLibrary::CurrentActiveWindow(display);
    XCloseDisplay(display);
    timer.start(waitingTime);
}
