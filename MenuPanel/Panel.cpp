#include "Panel.hpp"

#include <QGuiApplication>
#include <QScreen>

Panel::Panel() : startMenu(new StartMenu), taskBar(new TaskBar())
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignLeft);
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    layout->addWidget(startMenu.get());
    layout->addLayout(taskBar);
    setCentralWidget(widget);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);
    QScreen* screen  = QGuiApplication::screens().front();
    int screenHeight = screen->geometry().height();
    int screenWidth  = screen->geometry().width();
    setGeometry(0, screenHeight - Settings::panelHeight, screenWidth,
                Settings::panelHeight);
    setPalette(QPalette(Settings::panelBackgroundColor));
}
