#include "MainDemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainDemo w(a);
  w.show();

  return a.exec();
}
