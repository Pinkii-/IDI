#include "myform.h"

MyForm::MyForm(QWidget* parent): QWidget(parent)
{
    ui.setupUi(this);
    visible = true;
}

void MyForm::setVisible() {
    visible = !visible;
    ui.
}
