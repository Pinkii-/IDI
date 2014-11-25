#include <QApplication>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <myqlabel.h>
#include <myqframe.h>


int main( int argc, char ** argv)
{
  // Creem tots els components
  QApplication app(argc, argv);
  MyQFrame *w=new MyQFrame();
  QVBoxLayout *mainLayout=new QVBoxLayout(w);
  MyQLabel *label_mostra   = new MyQLabel ( "Etiqueta de mostra", w); //lbl1
  QHBoxLayout *layHoriz  = new QHBoxLayout();          // lo1  Layout Horizontal
  QLabel *label_nouText  = new QLabel( "Entra el nou text:", w);  //lbl2
  QLineEdit *txtLine=new QLineEdit( "", w);
  label_mostra->getText(txtLine->text());
  QHBoxLayout *qButtonLayout = new QHBoxLayout();
  QPushButton *quitButton = new QPushButton("&Quit", w);
  QPushButton *nextButton = new QPushButton("&Next",w);

  // Insertem el label i el editor de text al primer layout horizontal
  layHoriz->addWidget(label_nouText);
  layHoriz->addWidget(txtLine);

  // Insertem l'espai i el buttó de quit al layout horizontal del botó
  qButtonLayout->addItem(new QSpacerItem(30,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
  qButtonLayout->addWidget(quitButton);
  qButtonLayout->addWidget(nextButton);
  
  // Afegim tots els elements dins del layout principal
  mainLayout->addWidget(label_mostra);
  mainLayout->addLayout(layHoriz);
  mainLayout->addItem(new QSpacerItem(20,40,QSizePolicy::Minimum,QSizePolicy::Expanding));
  mainLayout->addLayout(qButtonLayout);
  mainLayout->addWidget(nextButton);

  app.connect(quitButton, SIGNAL(clicked()), w, SLOT(close()));
  app.connect(txtLine, SIGNAL(textChanged(const QString&)), label_mostra, SLOT(getText(const QString &)));
  app.connect(txtLine, SIGNAL(returnPressed()), label_mostra, SLOT(setText2()));

  w->show();
  return app.exec();
}
