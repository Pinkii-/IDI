#include "myqlabel.h"

MyQLabel::MyQLabel(QString s, QWidget *parent) : QLabel(parent){
    text = s;
    setText(s);
}

void MyQLabel::getText(QString s) {
    text = s;
}

void MyQLabel::setText2() {
    setText(text);
}

