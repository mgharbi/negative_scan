#include <iostream>
#include <QApplication>
// #include <QResource>
// #include <QDebug>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mw;
  // bool f = QResource::registerResource(":shaders.qrc");
  // qDebug() << "res" << f;

  mw.show();
  return app.exec();
  // QCoreApplication::set
}
