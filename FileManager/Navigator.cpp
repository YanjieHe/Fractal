#include "Navigator.hpp"
#include "MainWindow.hpp"
#include <QDebug>
#include <QDir>

Navigator::Navigator(MainWindow* mainWindow)
    : mainWindow{mainWindow}, previousFolder{new QPushButton()},
      nextFolder{new QPushButton()},
      parentFolder{new QPushButton()}, lineEdit{new QLineEdit()}
{
    previousFolder->setText("previous folder");
    connect(previousFolder, SIGNAL(clicked()), this,
            SLOT(ClickPreviousFolder()));

    nextFolder->setText("next folder");
    connect(nextFolder, SIGNAL(clicked()), this, SLOT(ClickNextFolder()));

    parentFolder->setText("go to parent folder");
    connect(parentFolder, SIGNAL(clicked()), this,
            SLOT(ClickGoToParentFolder()));

    this->addWidget(previousFolder);
    this->addWidget(nextFolder);
    this->addWidget(parentFolder);
    this->addWidget(lineEdit);
}

void Navigator::ClickPreviousFolder()
{
    auto area = mainWindow->iconPreviewArea;
    if (area->history.HasPreviousDirectory())
    {
        auto directory = area->history.PreviousDirectory();
        area->ChangeDirectory(directory);
    }
}

void Navigator::ClickNextFolder()
{
    auto area = mainWindow->iconPreviewArea;
    if (area->history.HasNextDirectory())
    {
        auto directory = area->history.NextDirectory();
        area->ChangeDirectory(directory);
    }
}

void Navigator::ClickGoToParentFolder()
{
    auto area = mainWindow->iconPreviewArea;
    QDir directory(area->currentDir);
    if (directory.cdUp())
    {
        mainWindow->iconPreviewArea->ChangeDirectory(directory.path());
    }
}
