#include "Settings.hpp"

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
