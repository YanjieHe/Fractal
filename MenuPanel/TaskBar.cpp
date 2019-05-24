#include "TaskBar.hpp"
#include "Settings.hpp"
#include "XUtil.hpp"
TaskBar::TaskBar() : timer(this)
{
    setContentsMargins(0, 0, 0, 0);
    setSpacing(20);
    connect(&timer, SIGNAL(timeout()), this, SLOT(UpdateTasks()));
    timer.start(Settings::refreshingInterval);
}

void TaskBar::UpdateTasks()
{
    timer.stop();
    Display* display	= XOpenDisplay(nullptr);
    unsigned long count = 0;
    auto windowList		= XUtil::GetWindowList(display, &count);
    for (int i = 0; i < static_cast<int>(count); i++)
    {
        Window handle = windowList[i];
        auto title =
            QString::fromStdString(XUtil::GetWindowName(display, handle));
        if (i < taskList.size())
        {
            taskList[i]->SetHandle(handle);
            taskList[i]->SetTitle(title);
        }
        else
        {
            auto task = new TaskButton(handle, title, layout()->widget());
            layout()->addWidget(task);
            taskList.push_back(task);
        }
    }
    while (taskList.size() > static_cast<int>(count))
    {
        layout()->removeWidget(taskList.back());
        delete taskList.back();
        taskList.pop_back();
    }
    XFree(windowList);
    XCloseDisplay(display);
    timer.start(Settings::refreshingInterval);
}
