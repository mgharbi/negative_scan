#include <glog/logging.h>

#include "Timer.h"
#include "histogram.h"
#include "subsample.h"
#include "minmax.h"

#include "image.h"


Image::Image(uint16_t* data, int w, int h) : mBuffer(data, 3, w, h)
{
  LOG(INFO) << "created image with size " << w << "x" << h;
}

Image::Image(int w, int h) : mBuffer(3, w, h)
{
  LOG(INFO) << "created empty image with size " << w << "x" << h;
}


std::shared_ptr<Image> Image::getPreview(int preview_size) {
  int w = mBuffer.extent(1);
  int h = mBuffer.extent(2);
  float aspect = ((float) w) / h;
  int preview_w = 0, preview_h = 0;
  if ( w > h ) {
    preview_w = preview_size;
    preview_h = preview_w / aspect;
  } else {
    preview_h = preview_size;
    preview_w = preview_h * aspect;
  }
  LOG(INFO) << "making preview with size " << preview_w << "x" << preview_h;

  Timer timer;
  timer.reset();
  std::shared_ptr<Image> preview = std::make_shared<Image>(preview_w, preview_h);
  subsample(mBuffer, preview_w, preview_h, preview->mBuffer);
  LOG(INFO) << "HL: subsampled in " << timer.elapsed() << " ms";
  return preview;
}


Histograms::Histograms(int nbins) 
  : nbins(nbins), mBuffer(3, nbins) {
    mBuffer.fill(0.0f);
}

float* Histograms::red() {
  return mBuffer.data();
}

float* Histograms::green() {
  return mBuffer.data() + nbins;
}

float* Histograms::blue() {
  return mBuffer.data() + 2*nbins;
}

Histograms::Histograms(Image im, int nbins) :
  Histograms(nbins)
{
  Timer timer;
  timer.reset();
  histogram(im.buffer(), nbins, mBuffer);
  LOG(INFO) << "HL: computed histograms in " << timer.elapsed() << " ms";

  Halide::Runtime::Buffer<float> min_(3);
  Halide::Runtime::Buffer<float> max_(3);
  minmax(im.buffer(), min_, max_);

  for (int i = 0; i < 3; ++i) {
    LOG(INFO) << "range" << i << ": [" << min_.data()[i] << " -- " << max_.data()[i] <<"]";
  }
  for (int i = 0; i < 3; ++i) {
    LOG(INFO) << "range(inv)" << i << "[" << 1.0f/max_.data()[i] << 1.0f/min_.data()[i] <<"]";
  }
}


// void Image::getHistograms(int nbins) {
//
//   // Buffer<float> rgb_buf(camera_rgb, 4,3);
//   //
// }


Image::~Image () {
  LOG(INFO) << "Destroying Image";
}
