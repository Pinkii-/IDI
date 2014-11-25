#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>

class MyQLabel : public QLabel
{
    Q_OBJECT
private:
    QString text;
public:
    explicit MyQLabel(QString s,QWidget *parent);
    
signals:
    
public slots:
    void getText(QString s);
    void setText2();
    
};

#endif // MYQLABEL_H
