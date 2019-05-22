#ifndef TASKBUTTON_HPP
#define TASKBUTTON_HPP

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPushButton>

typedef unsigned long WindowID;
class TaskButton : public QPushButton
{
    Q_OBJECT
  public:
    WindowID window;
    TaskButton(WindowID window, const QString& text, QWidget* parent = nullptr);
    static QString Truncate(QString text);
  public slots:
    void ClickTask();
};

#endif // TASKBUTTON_HPP
