#include "Application.hpp"
#include <QDebug>
#include <QDir>
#include <QProcess>

Application::Application(QString name, QString command, bool terminal)
    : QAction(name), command{command}, terminal{terminal}
{
    ProcessTerminalApp();
    connect(this, SIGNAL(triggered()), this, SLOT(RunApplication()));
}

void Application::ProcessTerminalApp()
{
    QVector<QString> toRemove = {"%f", "%F", "%u", "%U", "%i", "%c", "%k",
                                 "%d", "%D", "%n", "%N", "%v", "%m"};
    for (auto item : toRemove)
    {
        command = command.replace(item, "");
    }
    if (terminal)
    {
        command = "konsole -e \"" + command + "\"";
    }
    //    qDebug() << command;
}

void Application::RunApplication()
{
    QDir current = QDir::current();
    QDir::setCurrent("/home/heyanjie");
    if (!QProcess::startDetached(command))
    {
        qDebug() << "cannot open" << text();
    }
    QDir::setCurrent(current.path());
}
