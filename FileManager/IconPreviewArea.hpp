#ifndef ICONPREVIEWAREA_HPP
#define ICONPREVIEWAREA_HPP

#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QToolButton>

class IconView;
class IconPreviewArea;

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
    QVector<IconView*> icons;
    QSize currentSize;
    IconPreviewArea();
    void SetDirectory(const QString& path);
    void UpdateSize(const QSize& size);
};

#endif // ICONPREVIEWAREA_HPP
