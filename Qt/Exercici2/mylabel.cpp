#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {

}

void MyLabel::setColor() {
    if (marca == "Marca")
        setStyleSheet("QLabel { background-color : green;}");
    else if (marca == "Desmarca")
        setStyleSheet("QLabel { background-color : white;}");
}

void MyLabel::getText(QString s) {
    marca = s;
}
