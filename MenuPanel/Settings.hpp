#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QColor>
#include <QFont>
#include <QString>

class SettingsLoadException
{
};

class Settings
{
  public:
    static int panelHeight;
    static int startButtonHeight;
    static int startButtonWidth;
    static QString startButtonIcon;
    static int startButtonIconHeight;
    static int startButtonIconWidth;
    static int refreshingInterval;

    static QColor panelBackgroundColor;

    static QFont font;
    static int fontPixelSize;
    static QFont MakeFont() noexcept;
    static void LoadSettings(QString path);
};

#endif // SETTINGS_HPP
