#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
    word = "DICCIONARI";
    guess = "**********";
    this->setText(guess);
}
