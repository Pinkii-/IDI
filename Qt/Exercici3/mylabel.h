#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT
private:
    QString word;
    QString guess;
public:
    explicit MyLabel(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // MYLABEL_H
