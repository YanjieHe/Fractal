#include "StartMenu.hpp"
#include "Settings.hpp"
#include <QMenu>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

StartMenu::StartMenu() : rootMenu(new QMenu())
{
    setFixedHeight(Settings::startButtonHeight);
    setFixedWidth(Settings::startButtonWidth);
    QPixmap pixmap(Settings::startButtonIcon);
    QIcon buttonIcon(pixmap);
    setIcon(buttonIcon);
    setFlat(true);
    setGeometry(0, 0, Settings::startButtonHeight, Settings::startButtonWidth);
    setIconSize(
        QSize(Settings::startButtonIconWidth, Settings::startButtonIconHeight));

    setMenu(rootMenu);
    SetMenuAttributes(rootMenu);
    GetAppInfo();
}

void StartMenu::GetAppInfo()
{
    QVector<MenuItem> items = GetRows();
    QMenu* topLevelMenu		= rootMenu;
    QHash<int, MenuItem> itemTable;
    for (const auto& item : items)
    {
        itemTable.insert(item.id, item);
    }
    QVector<QVector<MenuItem>> graph(itemTable.size() + 1);
    for (const auto& item : items)
    {
        graph[item.parent].append(item);
    }

    QHash<int, QMenu*> menuTable;
    menuTable.insert(0, topLevelMenu);
    for (const auto& item : items)
    {
        if (item.type == "Menu")
        {
            QMenu* menu = new QMenu(item.name);
            SetMenuAttributes(menu);
            menuTable.insert(item.id, menu);
        }
    }
    for (const auto& key : menuTable.keys())
    {
        if (key != 0)
        {
            menuTable[itemTable[key].parent]->addMenu(menuTable[key]);
        }
    }
    QHash<int, QAction*> appTable;
    for (const auto& item : items)
    {
        if (item.type == "Item")
        {
            QAction* action = CreateAction(item.name, item.command);
            appTable.insert(item.id, action);
        }
    }
    for (const auto& key : appTable.keys())
    {
        menuTable[itemTable[key].parent]->addAction(appTable[key]);
    }
}

QVector<StartMenu::MenuItem> StartMenu::GetRows()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/home/heyanjie/.config/Minimalism/menu.db");
    QVector<MenuItem> items;
    if (!database.open())
    {
        return items;
    }
    else
    {
        QSqlQuery query("SELECT * from Menu_Item;");
        int idIndex		 = query.record().indexOf("id");
        int parentIndex  = query.record().indexOf("parent");
        int nameIndex	= query.record().indexOf("name");
        int commandIndex = query.record().indexOf("command");
        int actionIndex  = query.record().indexOf("action");
        int typeIndex	= query.record().indexOf("item_type");
        while (query.next())
        {
            int id			= query.value(idIndex).toInt();
            int parent		= query.value(parentIndex).toInt();
            QString name	= query.value(nameIndex).toString();
            QString command = query.value(commandIndex).toString();
            QString action  = query.value(actionIndex).toString();
            QString type	= query.value(typeIndex).toString();
            items.push_back(MenuItem(id, parent, name, command, action, type));
        }
        std::sort(
            items.begin(), items.end(),
            [](const MenuItem& x, const MenuItem& y) { return x.id < y.id; });
        return items;
    }
}

QAction* StartMenu::CreateAction(QString name, QString command)
{
    auto action = new ExecutableAction(name, command);
    return action;
}

void StartMenu::SetMenuAttributes(QMenu* menu)
{
    menu->setPalette(Settings::panelBackgroundColor);
    menu->setFont(Settings::font);
}
