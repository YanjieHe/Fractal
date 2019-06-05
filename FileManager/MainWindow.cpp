#include "MainWindow.hpp"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QVBoxLayout>
MainWindow::MainWindow()
    : menuBar{new QMenuBar()}, navigator{new Navigator(this)},
      iconPreviewArea{new IconPreviewArea(size(), QDir("/home/heyanjie/"))}
{
    QScreen* screen  = QGuiApplication::screens().front();
    int screenWidth  = screen->geometry().width();
    int screenHeight = screen->geometry().height();

    setGeometry(100, 100, screenWidth / 3, screenHeight / 2);

    QWidget* widget		= new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(menuBar);
    layout->addLayout(navigator);

    QWidget* area = new QWidget();
    area->setSizePolicy(QSizePolicy::Policy::Expanding,
                        QSizePolicy::Policy::Expanding);
    area->setStyleSheet("background-color: white");
    area->setLayout(iconPreviewArea);

    this->setCentralWidget(widget);
    layout->addWidget(area);
    widget->setLayout(layout);

    AddItems(menuBar->addMenu("File"),
             {"New Window", "New Tab", "Create New", "Folder Properties",
              "Close Tab", "Close Window"});
    menuBar->addMenu("Edit")->addAction("Open");
    menuBar->addMenu("View");
    menuBar->addMenu("Tools");
    menuBar->addMenu("Help");
}

void MainWindow::AddItems(QMenu* menu, const QVector<QString>& items)
{
    for (const auto& item : items)
    {
        menu->addAction(item);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    iconPreviewArea->UpdateSize(event->size());
}
