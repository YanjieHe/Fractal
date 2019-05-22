#ifndef TASKBAR_HPP
#define TASKBAR_HPP
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTimer>

typedef unsigned long WindowID;

class WinInfo
{
  public:
    WindowID id;
    QString title;
    WinInfo() : id{0}
    {
    }
    WinInfo(WindowID id, QString title) : id{id}, title{title}
    {
    }
};

class TaskBar : public QHBoxLayout
{
    Q_OBJECT
  public:
    QTimer timer;
    QVector<WinInfo> windowList;
    QVector<QPushButton*> buttonList;
    const int waitingTime = 500;
    TaskBar();

    void BindButtons();
  public slots:
    void CheckWindows();
};

#endif // TASKBAR_HPP
