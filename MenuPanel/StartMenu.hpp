#ifndef STARTMENU_HPP
#define STARTMENU_HPP

#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <memory>

class ExecutableAction : public QAction
{
    Q_OBJECT
  public:
    QString name;
    QString command;
    ExecutableAction(QString name, QString command)
        : QAction(name), name{name}, command{command}
    {
        connect(this, SIGNAL(triggered()), this, SLOT(Execute()));
    }
  public slots:
    void Execute()
    {
        QDir current = QDir::current();
        QDir::setCurrent("/home/heyanjie");
        if (!QProcess::startDetached(command))
        {
            QMessageBox::critical(nullptr, tr("Cannot open program"), name);
        }
        QDir::setCurrent(current.path());
    }
};

class StartMenu : public QPushButton
{
    Q_OBJECT
  public:
    QMenu* rootMenu;
    StartMenu();
    class MenuItem
    {
      public:
        int id;
        int parent;
        QString name;
        QString command;
        QString action;
        QString type;
        MenuItem()
        {
        }
        MenuItem(int id, int parent, QString name, QString command,
                 QString action, QString type)
            : id{id}, parent{parent}, name{name}, command{command},
              action{action}, type{type}
        {
        }
    };
    void GetAppInfo();
    QVector<MenuItem> GetRows();
    QAction* CreateAction(QString name, QString command);
    void SetMenuAttributes(QMenu* menu);
};

#endif // STARTMENU_HPP
