#include "myform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyForm f;
    f.show();

    return a.exec();
    /* QPalette palette = ui->pLabel->palette();
 palette.setColor(ui->pLabel->backgroundRole(), Qt::yellow);
 palette.setColor(ui->pLabel->foregroundRole(), Qt::yellow);
 ui->pLabel->setPalette(palette);*/
}
