#ifndef MYFORM_H
#define MYFORM_H

#include "ui_widget.h"

class MyForm : public QWidget
{
    Q_OBJECT
public:
    MyForm(QWidget *parent = 0);
private:
    Ui::MyForm ui;
    bool visible;
public slots:
    void setVisible();
};

#endif // MYFORM_H
