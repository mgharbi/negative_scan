#pragma once

#include <libraw/libraw.h>
#include <QObject>

class RawProcessor: public QObject
{

  Q_OBJECT

public:
  RawProcessor();
  virtual ~RawProcessor ();

  void load();

signals:
  void updateImage(unsigned short *imdata, unsigned long w, unsigned long h);

private:
  LibRaw iProcessor;
};
