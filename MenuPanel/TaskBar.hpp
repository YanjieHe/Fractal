#ifndef TASKBAR_HPP
#define TASKBAR_HPP
#include "TaskButton.hpp"
#include <QHBoxLayout>
#include <QTimer>
#include <QVector>

class TaskBar : public QHBoxLayout
{
    Q_OBJECT
  public:
    QTimer timer;
    QVector<TaskButton*> taskList;
    TaskBar();
  public slots:
    void UpdateTasks();
};

#endif // TASKBAR_HPP
