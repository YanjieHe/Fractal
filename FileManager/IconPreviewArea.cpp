#include "IconPreviewArea.hpp"
#include "Settings.hpp"
#include <QDebug>
#include <QDir>
#include <QProcess>

IconView::IconView(IconPreviewArea* area, const QFileInfo& fileInfo)
    : area{area}, fileInfo{fileInfo}
{
    setIcon(QIcon("/home/heyanjie/.icons/file-manager.svg"));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setIconSize(QSize(Settings::iconWidth, Settings::iconHeight));
    setFixedSize(Settings::filePreviewWidth, Settings::filePreviewHeight);
    setStyleSheet("border: none");
    QFont font("Roboto");
    font.setPixelSize(16);
    setFont(font);
    setText(WrapText(fileInfo.fileName()));
}

QString IconView::WrapText(QString text)
{
    QFontMetrics fm(font());
    QStringList lines;
    QString current;
    int index = 0;
    while (index < text.size())
    {
        current.push_back(text.at(index));
        if (fm.width(current) > Settings::filePreviewWidth - 10)
        {
            lines.push_back(current);
            current = "";
            if (lines.size() >= 3)
            {
                return lines.join("\r\n") + "...";
            }
        }
        index++;
    }
    lines.push_back(current);
    return lines.join("\r\n");
}

void IconView::mouseDoubleClickEvent(QMouseEvent*)
{
    if (fileInfo.isDir())
    {
        auto area = this->area;
        qDebug() << "change directory ->" << fileInfo.filePath();
        area->SetDirectory(fileInfo.filePath());
        qDebug() << "update size";
        qDebug() << area->currentSize;
        area->UpdateSize(area->currentSize);
    }
    else
    {
        QString extension = fileInfo.completeSuffix();
        if (extension == "xml")
        {
            QProcess::startDetached("konsole -e vim " + fileInfo.fileName());
        }
    }
}

IconPreviewArea::IconPreviewArea()
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void IconPreviewArea::SetDirectory(const QString& path)
{
    QDir directory(path);
    QDir::setCurrent(path);
    QFileInfoList fileInfoList = directory.entryInfoList();
    for (auto icon : icons)
    {
        delete icon;
    }
    icons.clear();
    qDebug() << "clear icons";
    for (const auto& item : fileInfoList)
    {
        if (item.fileName() != "." && item.fileName() != "..")
        {
            IconView* iconView = new IconView(this, item);
            icons.push_back(iconView);
        }
    }
    qDebug() << "successfully load all icons";
}

void IconPreviewArea::UpdateSize(const QSize& size)
{
    currentSize = size;
    setSpacing(30);
    auto computeNumberOfItems = [this](int wholeWidth, int itemWidth) {
        int count		 = 0;
        int currentWidth = itemWidth;
        while (currentWidth < wholeWidth)
        {
            count++;
            currentWidth = currentWidth + itemWidth + spacing();
        }
        qDebug() << "current width =" << currentWidth
                 << ", whole width =" << wholeWidth;
        if (currentWidth - spacing() < wholeWidth)
        {
            return std::max(count, 1);
        }
        else
        {
            return std::max(count - 1, 1);
        }
    };
    int itemsInARow =
        computeNumberOfItems(size.width(), Settings::filePreviewWidth);
    qDebug() << "items in a row = " << itemsInARow;
    int row = 0;
    int col = 0;
    for (const auto& item : icons)
    {
        this->addWidget(item, row, col, Qt::AlignCenter);
        col++;
        if (col >= itemsInARow)
        {
            col = 0;
            row++;
        }
    }
}
