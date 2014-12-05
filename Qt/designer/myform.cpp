#include "myform.h"

MyForm::MyForm(QWidget* parent): QWidget(parent)
{
    ui.setupUi(this);
    visible = true;
}

void MyForm::setVisible() {
    visible = !visible;
    ui.label->setVisible(visible);
    ui.label_2->setVisible(visible);
    ui.label_3->setVisible(visible);
    ui.lineEdit->setVisible(visible);
    ui.spinBox->setVisible(visible);
    ui.horizontalSlider->setVisible(visible);
}
