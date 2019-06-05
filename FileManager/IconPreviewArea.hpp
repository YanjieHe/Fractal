#ifndef ICONPREVIEWAREA_HPP
#define ICONPREVIEWAREA_HPP

#include "PreDefs.hpp"
#include <QDir>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QToolButton>
class IconView : public QToolButton
{
    Q_OBJECT
  public:
    IconPreviewArea* area;
    QFileInfo fileInfo;
    IconView(IconPreviewArea* area, const QFileInfo& fileInfo);
    QString WrapText(QString text);
  public slots:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

class IconPreviewArea : public QGridLayout
{
    Q_OBJECT
  public:
    class History
    {
      public:
        QVector<QDir> list;
        QVector<QDir>::iterator current;
        History();
        History(const QDir& directory);
        void VisitNewDirectory(const QDir& directory);
        const QDir& CurrentDirectory() const;
        const QDir& PreviousDirectory();
        const QDir& NextDirectory();
        bool HasPreviousDirectory() const;
        bool HasNextDirectory() const;
    };
    QVector<IconView*> icons;
    QSize currentSize;
    QDir currentDir;
    History history;
    IconPreviewArea(QSize size, QDir directory);
    void UpdateSize(const QSize& size);
    void ChangeDirectory(const QDir& directory);
};

#endif // ICONPREVIEWAREA_HPP
