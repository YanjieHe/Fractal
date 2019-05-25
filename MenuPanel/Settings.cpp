#include "Settings.hpp"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>

int Settings::panelHeight = 50;

int Settings::startButtonHeight = 50;
int Settings::startButtonWidth  = 50;

QString Settings::startButtonIcon = "/home/heyanjie/Downloads/arch_linux.svg";

int Settings::startButtonIconHeight = 40;
int Settings::startButtonIconWidth  = 40;

int Settings::refreshingInterval = 500;

QColor Settings::panelBackgroundColor = QColor(47, 52, 63);

QFont Settings::font		= MakeFont();
int Settings::fontPixelSize = 18;

QFont Settings::MakeFont() noexcept
{
    QFont font("Roboto");
    font.setPixelSize(fontPixelSize);
    return font;
}

void Settings::LoadSettings(QString path)
{
    auto containsAll = [](const QHash<QString, QString>& record,
                          const QVector<QString>& keyList) -> bool {
        for (auto key : keyList)
        {
            if (!record.contains(key))
            {
                return false;
            }
        }
        return true;
    };
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(path);
    if (!database.open())
    {
        throw SettingsLoadException();
    }
    else
    {
        QSqlQuery query("SELECT * from Menu_Look;");
        int keyIndex   = query.record().indexOf("key");
        int valueIndex = query.record().indexOf("value");
        QHash<QString, QString> record;
        while (query.next())
        {
            QString key   = query.value(keyIndex).toString();
            QString value = query.value(valueIndex).toString();
            record.insert(key, value);
        }
        if (containsAll(record,
                        {"Start_Button_Height", "Start_Button_Width",
                         "Start_Button_Icon", "Start_Button_Icon_Height",
                         "Start_Button_Icon_Width", "Refreshing_Interval",
                         "Panel_Background_Color", "Font", "Font_Pixel_Size"}))
        {
            Settings::startButtonHeight = record["Start_Button_Height"].toInt();
            Settings::startButtonWidth  = record["Start_Button_Width"].toInt();
            Settings::startButtonIcon   = record["Start_Button_Icon"];
            Settings::startButtonIconHeight =
                record["Start_Button_Icon_Height"].toInt();
            Settings::startButtonIconWidth =
                record["Start_Button_Icon_Width"].toInt();
            Settings::refreshingInterval =
                record["Refreshing_Interval"].toInt();
            Settings::panelBackgroundColor =
                QColor(record["Panel_Background_Color"]);
            Settings::font			= QFont(record["Font"]);
            Settings::fontPixelSize = record["Font_Pixel_Size"].toInt();
            Settings::font.setPixelSize(Settings::fontPixelSize);
        }
        else
        {
            throw SettingsLoadException();
        }
    }
}
