#pragma once

#include <libraw/libraw.h>
#include <QObject>
#include <QString>
#include <HalideBuffer.h>
#include "Timer.h"

#include "ControlData.h"

int libraw_progress_handler(void *callback_data,enum LibRaw_progress stage, int
    iteration, int expected);

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
  void whitePointPicked(float x, float y);

signals:
  void updateImage(uint16_t *imdata, unsigned long w, unsigned long h,
                   float* camera_rgb);
  void updateHistogram(const float *data, const int);
  void setWhitePoint(float wp, int channel);

private:
  Timer timer;
  Halide::Runtime::Buffer<uint16_t> *currentImage;
  LibRaw iProcessor;
  QString *currentFilename;

  float* camera_rgb;

  void write_tiff(Halide::Runtime::Buffer<uint16_t> &buf, std::string path);
};
