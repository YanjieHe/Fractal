#include "Panel.hpp"
#include "Settings.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    try
    {
        Settings::LoadSettings("/home/heyanjie/.config/Minimalism/menu.db");
    }
    catch (SettingsLoadException&)
    {
        QMessageBox::critical(
            nullptr, "Error",
            "Cannot load menu settings. Please check the database path.");
        return 1;
    }
    catch (...)
    {
        QMessageBox::critical(
            nullptr, "Error",
            "Cannot load menu settings. Please check the format of settings.");
        return 1;
    }
    Panel panel;
    panel.show();
    return a.exec();
}
