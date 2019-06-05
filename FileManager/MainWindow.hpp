#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "IconPreviewArea.hpp"
#include "Navigator.hpp"
#include "PreDefs.hpp"
#include <QMainWindow>
#include <QMenuBar>
#include <QResizeEvent>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    QMenuBar* menuBar;
    Navigator* navigator;
    IconPreviewArea* iconPreviewArea;
    MainWindow();
    void AddItems(QMenu* menu, const QVector<QString>& items);
    void resizeEvent(QResizeEvent* event) override;
};

#endif // MAINWINDOW_HPP
