#ifndef TASKBUTTON_HPP
#define TASKBUTTON_HPP
#include <QPushButton>

using WindowHandle = unsigned long;

class TaskButton : public QPushButton
{
    Q_OBJECT
  public:
    WindowHandle windowHandle;
    TaskButton(WindowHandle windowHandle, const QString& text, QWidget* parent);
    static QString Truncate(QString text);
    void SetHandle(WindowHandle handle);
    void SetTitle(QString title);
  public slots:
    void ClickTask();
};

#endif // TASKBUTTON_HPP
