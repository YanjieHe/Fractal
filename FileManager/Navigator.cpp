#include "Navigator.hpp"

Navigator::Navigator() : lineEdit{new QLineEdit()}
{
    this->addWidget(lineEdit);
}
