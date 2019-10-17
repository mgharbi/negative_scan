#include "raw_processor.h"
#include <glog/logging.h>

#include <tiffio.h>

#include "subsample.h"
#include "minmax.h"
#include "invert_negative.h"
#include "invert_negative_bw.h"

using Halide::Runtime::Buffer;

RawProcessor::RawProcessor() {
  iProcessor.set_progress_handler(&libraw_progress_handler, &timer);
  camera_rgb = new float[3*4];
}


std::shared_ptr<Image> RawProcessor::load(std::string filename) {
  if(!filename.c_str()) {
    LOG(INFO) << "Filename is empty: " << filename;
    return nullptr;
  }

  iProcessor.recycle();
  memset(camera_rgb, 0.0f, 3*4*sizeof(float));

  // TODO: remove, quarter res, no demosaicking
  // iProcessor.imgdata.params.half_size = true;

  // Linear image with minimal processing
  iProcessor.imgdata.params.gamm[0] = 1.0;  // linear
  iProcessor.imgdata.params.gamm[1] = 1.0;  // linear
  iProcessor.imgdata.params.output_bps = 16;  // 16-bits
  iProcessor.imgdata.params.no_auto_bright = true;
  iProcessor.imgdata.params.no_auto_scale = false;
  iProcessor.imgdata.params.output_color = 1;  // 0: raw, 1: sRGB. We convert to sRGB ourselves
  iProcessor.imgdata.params.use_camera_wb = true;
  iProcessor.imgdata.params.use_camera_matrix = true;
  // iProcessor.imgdata.params.use_auto_wb = true;

  timer.reset();
  if (int open_err = iProcessor.open_file(filename.c_str()) != LIBRAW_SUCCESS) {
    LOG(INFO) << "Error loading" << LibRaw::strerror(open_err);
  }

  // for (int i = 0; i < 4; ++i) {
  //   qDebug() << "camera mul" << iProcessor.imgdata.color.cam_mul[i];
  //   qDebug() << "user mul" << iProcessor.imgdata.params.user_mul[i];
  //   iProcessor.imgdata.params.user_mul[i] = iProcessor.imgdata.color.cam_mul[i];
  // }
  //
  // float mul = -0.5;
  // iProcessor.imgdata.params.user_mul[0] *= 1.25*mul;
  // iProcessor.imgdata.params.user_mul[1] *= 1.0*mul;
  // iProcessor.imgdata.params.user_mul[2] *= 2*mul;
  // iProcessor.imgdata.params.user_mul[3] *= 1.25*mul;

  // TODO(mgharbi): dcraw error checking
  if (int unpack_err = iProcessor.unpack() != LIBRAW_SUCCESS) { 
    LOG(INFO) << "Unpacking errored" << LibRaw::strerror(unpack_err);
  }

  if (int process_err = iProcessor.dcraw_process() != LIBRAW_SUCCESS) {
    LOG(INFO) << "Raw2Image errored" << LibRaw::strerror(process_err);
  }

  memcpy(camera_rgb, iProcessor.imgdata.color.rgb_cam, 3*4*sizeof(float));

  int w = 0;
  int h = 0;
  int c = 0;
  int bpp = 0;
  iProcessor.get_mem_image_format(&w, &h, &c, &bpp);

  std::shared_ptr<Image> im = std::make_shared<Image>(w, h);
  int err = iProcessor.copy_mem_image(im->data(), w*(bpp/8)*c, 0);
  iProcessor.recycle();

  if (err) {
    LOG(ERROR) << "copy mem err" << LibRaw::strerror(err);
  }
  return im;
  // qDebug() << "profiles" << iProcessor.imgdata.params.output_profile 
                         // << iProcessor.imgdata.params.camera_profile ;

  // // emit updateImage(preview.data(), preview_w, preview_h, camera_rgb);
  //
  // // emit updateHistogram(hist.data(), nbins);
  //
}


// void RawProcessor::save(ControlData data) {
//   if (!currentImage) {
//     qDebug() << "no image open, abort";
//     return;
//   }
//   int width = currentImage->dim(1).extent();
//   int height = currentImage->dim(2).extent();
//   Buffer<uint16_t> processed(3, width, height);
//   Buffer<float> rgb_buf(camera_rgb, 4,3);
//   qDebug() << "inverting negative"
//            << "wp" << data.wp[0] 
//                       << data.wp[1]
//                       << data.wp[2]
//            << "bp" << data.bp[0] 
//                       << data.bp[1]
//                       << data.bp[2]
//            << "gamma" << data.gamma[0] 
//                       << data.gamma[1]
//                       << data.gamma[2]
//            << "output gamma" << data.output_gamma;
//   if(data.grayscale) {
//     qDebug() << "gray";
//     invert_negative_bw(
//         *currentImage, 
//         rgb_buf,
//         Buffer<float>(data.gamma.data(), 3),
//         Buffer<float>(data.wp.data(), 3),
//         Buffer<float>(data.bp.data(), 3),
//         data.exposure,
//         data.output_gamma,
//         processed);
//   } else {
//     invert_negative(
//         *currentImage, 
//         rgb_buf,
//         Buffer<float>(data.gamma.data(), 3),
//         Buffer<float>(data.wp.data(), 3),
//         Buffer<float>(data.bp.data(), 3),
//         data.exposure,
//         data.output_gamma,
//         processed);
//   }
//   qDebug() << "saving";
//   write_tiff(*currentImage, "0_input.tiff");
//   write_tiff(processed, "1_output.tiff");
// }


// void RawProcessor::write_tiff(Buffer<uint16_t> &buffer, std::string path) {
//   int width = buffer.dim(1).extent();
//   int height = buffer.dim(2).extent();
//
//   TIFF *out= TIFFOpen(path.c_str(), "w");
//   TIFFSetField (out, TIFFTAG_IMAGEWIDTH, width);
//   TIFFSetField (out, TIFFTAG_IMAGELENGTH, height);
//   TIFFSetField (out, TIFFTAG_SAMPLESPERPIXEL, 3);
//   TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 16);
//   TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
//   TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
//   TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
//
//   tsize_t linebytes = 3 * width * sizeof(uint16_t);
//   unsigned char *buf = NULL;        
//   //    Allocating memory to store the pixels of current row
//   if (TIFFScanlineSize(out) >= linebytes) {
//     buf =(unsigned char *)_TIFFmalloc(linebytes);
//   }
//   else {
//     buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));
//   }
//
//   // We set the strip size of the file to be size of one row of pixels
//   TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, width * 3));
//
//   // Now writing image to the file one strip at a time
//   for (uint32 row = 0; row < height; row++)
//   {
//     memcpy(buf, &buffer.data()[row*3*width], linebytes);
//     if (TIFFWriteScanline(out, buf, row, 0) < 0)
//       break;
//   }
//   TIFFClose(out);
//
//   qDebug() << ".tiff file saved";
// }


int libraw_progress_handler(void *callback_data,enum LibRaw_progress stage, int
    iteration, int expected) {
  Timer* timer = static_cast<Timer*>(callback_data);
  LOG(INFO) << "LibRAW stage:" << LibRaw::strprogress(stage)
           << (iteration+1) << " of " << expected
           << timer->elapsed() << " ms" ;
  timer->reset();
  return 0;
}


// void RawProcessor::whitePointPicked(float x, float y) {
//   if (!currentImage) {
//     qDebug() << "no image, cannot pick white point";
//   }
//   qDebug() << "RAW wp picked at " << x << y;
//
//   // convert quad to img coordinates
//   int w = currentImage->width();
//   int h = currentImage->height();
//   int x_i = x*w;
//   int y_i = y*h;
//
//   // sample from image/texture for white point
//   float r = (*currentImage)(0, x_i, y_i) / 65535.0f;
//   float g = (*currentImage)(1, x_i, y_i) / 65535.0f; 
//   float b = (*currentImage)(2, x_i, y_i) / 65535.0f;
//   qDebug() << "wp" << r << g << b;
//
//   // update histograms white point
//   emit setWhitePoint(r, 0);
//   emit setWhitePoint(g, 1);
//   emit setWhitePoint(b, 2);
// }


RawProcessor::~RawProcessor() {
  // if (currentImage) {
  //   delete[] currentImage->data();
  //   delete currentImage;
  //   iProcessor.recycle();
  // }
  // if(camera_rgb) {
  //   delete[] camera_rgb;
  // }
}
