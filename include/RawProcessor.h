#pragma once

#include <libraw/libraw.h>
#include <QObject>
#include <QString>

class RawProcessor: public QObject
{

  Q_OBJECT

public:
  RawProcessor();
  virtual ~RawProcessor ();

public slots:
  void load(QString filename);

signals:
  void updateImage(unsigned short *imdata, unsigned long w, unsigned long h);

private:
  LibRaw iProcessor;
};
