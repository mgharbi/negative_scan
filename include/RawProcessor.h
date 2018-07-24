#pragma once

#include <libraw/libraw.h>
#include <QObject>
#include <QString>
#include <HalideBuffer.h>

/**
 * This class takes ownership of data's memory
 */
class Image
{
public:
  Image(unsigned short* data, int width, int height);
  Halide::Runtime::Buffer<uint16_t> wrap();
  virtual ~Image();

private:
  unsigned short* data;
  int width;
  int height;
};

class RawProcessor: public QObject
{

  Q_OBJECT

public:
  RawProcessor();
  virtual ~RawProcessor ();

public slots:
  void load(QString filename);
  void save();

signals:
  void updateImage(unsigned short *imdata, unsigned long w, unsigned long h);

private:
  LibRaw iProcessor;
  Image *currentImage;
  QString *currentFilename;
};
