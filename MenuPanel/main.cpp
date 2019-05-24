#include "Panel.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Panel panel;
    panel.show();
    return a.exec();
}
