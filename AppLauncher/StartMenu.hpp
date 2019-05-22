#ifndef STARTMENU_HPP
#define STARTMENU_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
class MenuItem
{
  public:
    QString name;
    QString command;
    MenuItem()
    {
    }
    MenuItem(QString name, QString command) : name{name}, command{command}
    {
    }

    QString ToString()
    {
        return "[" + name + "] " + command;
    }
};

class StartMenu : public QMainWindow
{
    Q_OBJECT
  public:
    int height;
    QPushButton* startButton;

  public:
    StartMenu(int height);

    void GetApplicationsInfo();

    QHash<QString, QString> GetProperty(QString entry, QString key);
};

#endif // STARTMENU_HPP
