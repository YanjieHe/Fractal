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
        area->history.VisitNewDirectory(fileInfo.filePath());
        area->ChangeDirectory(fileInfo.filePath());
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

IconPreviewArea::IconPreviewArea(QSize size, QDir directory)
    : currentSize{size}, currentDir{directory}, history(directory)
{
    qDebug() << "initialize icon preview area";
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setSpacing(30);
    ChangeDirectory(directory);
}

void IconPreviewArea::UpdateSize(const QSize& size)
{
    qDebug() << "current size = " << currentSize;
    qDebug() << "new size = " << size;
    currentSize				  = size;
    auto computeNumberOfItems = [this](int wholeWidth, int itemWidth) {
        int count		 = 0;
        int currentWidth = itemWidth;
        while (currentWidth < wholeWidth)
        {
            count++;
            currentWidth = currentWidth + spacing() + itemWidth;
        }
        qDebug() << (itemWidth * count + spacing() * (count - 1));
        return std::max(count, 1);
    };
    int itemsInARow =
        computeNumberOfItems(size.width() - 37, Settings::filePreviewWidth);
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

void IconPreviewArea::ChangeDirectory(const QDir& directory)
{
    QDir::setCurrent(directory.path());
    currentDir = directory;
    QDir::Filters filters =
        QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::AllDirs;
    QFileInfoList fileInfoList = directory.entryInfoList(filters);
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
    UpdateSize(currentSize);
}

IconPreviewArea::History::History() : current{list.begin()}
{
}

IconPreviewArea::History::History(const QDir& directory)
{
    list	= QVector<QDir>{directory};
    current = list.begin();
}

void IconPreviewArea::History::VisitNewDirectory(const QDir& directory)
{
    if (list.empty())
    {
        list.push_back(directory);
        current = list.begin();
    }
    else if (current == list.end() - 1)
    {
        list.push_back(directory);
        current = list.end() - 1;
    }
    else
    {
        list.erase(current + 1, list.end());
        list.push_back(directory);
        current = list.end() - 1;
    }
}

const QDir& IconPreviewArea::History::CurrentDirectory() const
{
    return *current;
}

const QDir& IconPreviewArea::History::PreviousDirectory()
{
    current--;
    return CurrentDirectory();
}

const QDir& IconPreviewArea::History::NextDirectory()
{
    current++;
    return CurrentDirectory();
}

bool IconPreviewArea::History::HasPreviousDirectory() const
{
    if (list.empty())
    {
        return false;
    }
    else if (current == list.begin())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool IconPreviewArea::History::HasNextDirectory() const
{
    if (list.empty())
    {
        return false;
    }
    else if (current == list.end() - 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
