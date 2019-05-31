#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include <QLineEdit>
#include <QVBoxLayout>
class Navigator : public QHBoxLayout
{
    Q_OBJECT
  public:
    QLineEdit* lineEdit;
    Navigator();
};

#endif // NAVIGATOR_HPP
