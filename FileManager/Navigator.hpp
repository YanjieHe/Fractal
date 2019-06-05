#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include "PreDefs.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
class Navigator : public QHBoxLayout
{
    Q_OBJECT
  public:
    MainWindow* mainWindow;
    QPushButton* previousFolder;
    QPushButton* nextFolder;
    QPushButton* parentFolder;
    QLineEdit* lineEdit;
    Navigator(MainWindow* mainWindow);
  public slots:
    void ClickPreviousFolder();
    void ClickNextFolder();
    void ClickGoToParentFolder();
};

#endif // NAVIGATOR_HPP
