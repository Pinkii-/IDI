#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>

class MyQLabel : public QLabel{
    Q_OBJECT
private:
    int numero;
    bool open;
public:
    explicit MyQLabel(QWidget *parent = 0);
    
signals:
    
public slots:
    void addOne();
    void addFour();
    void setOpen();
    void setClose();
    void setGreenBackground();
    void setRedBackground();
};

#endif // MYQLABEL_H
