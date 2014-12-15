#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT
private:
    QString marca;
public:
    explicit MyLabel(QWidget *parent = 0);
    
signals:
    
public slots:
    void setColor();
    void getText(QString s);
};

#endif // MYLABEL_H
