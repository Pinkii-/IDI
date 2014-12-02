#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    
signals:
    
public slots:
    void setTextMay(QString s);
    
};

#endif // MYLABEL_H
