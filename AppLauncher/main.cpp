#include "StartMenu.hpp"
#include "TaskBar.hpp"
#include <QApplication>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    StartMenu startMenu(50);
    startMenu.show();
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignLeft);
    TaskBar taskBar;
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    startMenu.setCentralWidget(widget);
    layout->addWidget(startMenu.startButton);
    layout->addLayout(&taskBar);
    return a.exec();
}
