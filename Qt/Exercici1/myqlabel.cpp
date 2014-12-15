#include "myqlabel.h"
#include <stdlib.h>

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent) {
    numero = 0;
    open = true;

}

void MyQLabel::addOne() {
    if (open) {
        ++numero;
        setNum(numero);
    }
}

void MyQLabel::addFour() {
    if (open) {
        numero += 4;
        setNum(numero);
    }
}

void MyQLabel::setOpen() {
    open = true;
}

void MyQLabel::setClose() {
    open = false;
}

void MyQLabel::setGreenBackground() {
    setStyleSheet("QLabel { background-color : green;}");
}

void MyQLabel::setRedBackground() {

    setStyleSheet("QLabel { background-color : red;}");
}


