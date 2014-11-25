#ifndef MYQFRAME_H
#define MYQFRAME_H

#include <QFrame>

class MyQFrame : public QFrame
{
    Q_OBJECT
private:
    int nEjercicio;
public:
    explicit MyQFrame();
    
signals:
    
public slots:
    void nextEjercicio();
};

#endif // MYQFRAME_H
