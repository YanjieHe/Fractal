#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <QMenu>

class Application : public QAction
{
    Q_OBJECT
  public:
    Application(QString name, QString command, bool terminal);

  private:
    QString command;
    bool terminal;
  public slots:
    void ProcessTerminalApp();
    void RunApplication();
};

#endif // APPLICATION_HPP
