#pragma once

#include <libraw/libraw.h>
#include <HalideBuffer.h>
#include "Timer.h"

#include "image.h"


int libraw_progress_handler(void *callback_data,enum LibRaw_progress stage, int
    iteration, int expected);


class RawProcessor
{

public:
  RawProcessor();
  virtual ~RawProcessor ();

// public slots:
  std::shared_ptr<Image> load(std::string filename);
  // void save(ControlData data);
  // void whitePointPicked(float x, float y);

  // void updateImage(uint16_t *imdata, unsigned long w, unsigned long h,
  //                  float* camera_rgb);
  // void updateHistogram(const float *data, const int);
  // void setWhitePoint(float wp, int channel);

private:
  Timer timer;
  // Halide::Runtime::Buffer<uint16_t> *currentImage;
  LibRaw iProcessor;
  std::string *currentFilename;

  float* camera_rgb;

  // void write_tiff(Halide::Runtime::Buffer<uint16_t> &buf, std::string path);
};
