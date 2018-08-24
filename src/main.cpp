#include <iostream>
#include <QApplication>
#include <QSurfaceFormat>
// #include <QResource>
// #include <QDebug>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // QSurfaceFormat format;
  // format.setDepthBufferSize(24);
  // format.setStencilBufferSize(8);
  // format.setVersion(3, 3);
  // format.setProfile(QSurfaceFormat::CoreProfile);
  // QSurfaceFormat::setDefaultFormat(format);

  MainWindow mw;
  // bool f = QResource::registerResource(":shaders.qrc");
  // qDebug() << "res" << f;

  mw.show();
  return app.exec();
  // QCoreApplication::set
}
