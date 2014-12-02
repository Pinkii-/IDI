#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{
}

void MyLabel::setTextMay(QString s) {
   setText(s.toUpper());
}
