#ifndef PANEL_HPP
#define PANEL_HPP
#include "Settings.hpp"
#include "StartMenu.hpp"
#include "TaskBar.hpp"
#include <QMainWindow>
#include <memory>

class Panel : public QMainWindow
{
    Q_OBJECT
  public:
    std::unique_ptr<StartMenu> startMenu;
    TaskBar* taskBar;
    Panel();
};

#endif // PANEL_HPP
