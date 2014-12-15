#include "myform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyForm f;
    f.show();

    return a.exec();
}
