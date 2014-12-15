#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QCheckBox>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
private:
    QString marca;
public:
    explicit MyCheckBox(QWidget *parent = 0);
    
signals:
    
public slots:
    void setClicked();
    void getText(QString s);
};

#endif // MYCHECKBOX_H
