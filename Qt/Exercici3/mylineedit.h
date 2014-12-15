#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit : public QLineEdit {
    Q_OBJECT
private:

public:
    explicit MyLineEdit(QWidget *parent = 0);
signals:
    
public slots:
    void setRedColor();
    void setGreenColor();
};

#endif // MYLINEEDIT_H
