#include "myqframe.h"

MyQFrame::MyQFrame() {
    QFrame();
    nEjercicio = 0;
}

void MyQFrame::nextEjercicio() {
    ++nEjercicio;
}
