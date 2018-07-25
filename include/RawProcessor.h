#pragma once

#include <libraw/libraw.h>
#include <QObject>
#include <QString>
#include <HalideBuffer.h>

#include "ControlData.h"

/**
 * This class takes ownership of data's memory
 */

class RawProcessor: public QObject
{

  Q_OBJECT

public:
  RawProcessor();
  virtual ~RawProcessor ();

public slots:
  void load(QString filename);
  void save(ControlData data);

signals:
  void updateImage(unsigned short *imdata, unsigned long w, unsigned long h);

private:
  Halide::Runtime::Buffer<uint16_t> *currentImage;
  LibRaw iProcessor;
  QString *currentFilename;
};
