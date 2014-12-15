#include "mycheckbox.h"

MyCheckBox::MyCheckBox(QWidget *parent) : QCheckBox(parent) {

}

void MyCheckBox::setClicked() {
    if (marca == "Marca")
        setChecked(true);
    else if (marca == "Desmarca")
        setChecked(false);
}

void MyCheckBox::getText(QString s) {
    marca = s;
}
