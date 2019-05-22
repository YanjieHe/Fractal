#include "StartMenu.hpp"
#include "Application.hpp"
#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QDesktopWidget>
#include <QGraphicsOpacityEffect>
#include <QMenu>
#include <QPalette>
#include <QPushButton>
#include <QScreen>

StartMenu::StartMenu(int height)
    : height{height}, startButton(new QPushButton(this))
{
    QScreen* screen = QGuiApplication::screens().front();
    QRect rect		= screen->geometry();
    setFixedWidth(rect.width());
    setFixedHeight(height);
    setGeometry(0, rect.height() - height, rect.width(), height);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);
    setPalette(QPalette(QColor(47, 52, 63)));
    QPixmap pixmap("/home/heyanjie/Downloads/arch_linux.svg");
    QIcon buttonIcon(pixmap);
    startButton->setIcon(buttonIcon);
    startButton->setIconSize(QSize(40, 40));
    startButton->setFixedSize(QSize(50, 50));
    startButton->setFlat(true);
    startButton->setGeometry(0, 0, 50, 50);

    QMenu* menu = new QMenu();
    menu->setPalette(QPalette(QColor(47, 52, 63)));
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint);
    menu->setStyleSheet("QMenu{background:rgba(47, 52, 63, 80%);}");
    auto font = QFont("Roboto 16");
    font.setPixelSize(16);
    menu->setFont(font);

    startButton->setMenu(menu);
    startButton->setStyleSheet("padding-top: 0px;");
    startButton->setStyleSheet("QPushButton::menu-indicator{image:none;}");
    menu->addAction("Poweroff");
    GetApplicationsInfo();

    setWindowOpacity(0.8);
}

void StartMenu::GetApplicationsInfo()
{
    auto nameTable	 = GetProperty("Desktop Entry", "Name");
    auto commandTable  = GetProperty("Desktop Entry", "Exec");
    auto terminalTable = GetProperty("Desktop Entry", "Terminal");

    QVector<QString> apps;
    for (auto key : nameTable.keys())
    {
        if (commandTable.contains(key) && terminalTable.contains(key))
        {
            apps.append(key);
        }
    }

    for (auto key : apps)
    {
        auto name		 = nameTable[key];
        auto command	 = commandTable[key];
        auto terminal	= terminalTable[key];
        auto application = new Application(name, command, terminal == "true");
        startButton->menu()->addAction(application);
    }
}

QHash<QString, QString> StartMenu::GetProperty(QString entry, QString key)
{
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("/home/heyanjie/.config/Minimalism/applications.db");
    QHash<QString, QString> table;
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
        return table;
    }
    else
    {
        qDebug() << "Database: connection ok";
        QSqlQuery query("SELECT app_name, value FROM Application "
                        "WHERE entry=\"" +
                        entry + "\" and key=\"" + key + "\";");
        int appNameIndex = query.record().indexOf("app_name");
        int valueIndex   = query.record().indexOf("value");
        while (query.next())
        {
            QString appName = query.value(appNameIndex).toString();
            QString value   = query.value(valueIndex).toString();
            table.insert(appName, value);
        }
        return table;
    }
}
