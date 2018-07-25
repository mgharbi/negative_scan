#include "RawProcessor.h"
#include <QDebug>

#include <tiffio.h>

#include "subsampler.h"
#include "invert_negative.h"


using Halide::Runtime::Buffer;


RawProcessor::RawProcessor() : currentImage(nullptr), currentFilename(nullptr) {
}

void RawProcessor::load(QString filename) {
  if(filename.isEmpty()) {
    return;
  }

  if (currentImage) {
    delete currentImage;
    iProcessor.recycle();
  }

  // currentFilename = filename;
  
  iProcessor.imgdata.params.half_size = true;
  // rotate
  // iProcessor.imgdata.params.user_flip = 3;

  // Linear image
  iProcessor.imgdata.params.gamm[0] = 1.0;
  iProcessor.imgdata.params.gamm[1] = 1.0;
  iProcessor.imgdata.params.output_bps = 16;  // 16-bits
  iProcessor.imgdata.params.no_auto_bright = true;
  iProcessor.imgdata.params.output_color = 1;  // linear sRGB
  iProcessor.imgdata.params.use_camera_wb = true;
  iProcessor.imgdata.params.use_camera_matrix = true;

  iProcessor.open_file(filename.toStdString().c_str());
  iProcessor.unpack();
  iProcessor.dcraw_process();

  int w = 0;
  int h = 0;
  int c = 0;
  int bpp = 0;
  iProcessor.get_mem_image_format(&w, &h, &c, &bpp);
  qDebug() << "post processed image" << w << "x" << h << "with" << c <<
    "colors and" << bpp << "bpp";
  if(bpp != 16) {
    qDebug() << "unexpected bit depth";
  }
  unsigned short* data = new unsigned short[w*h*c];
  iProcessor.copy_mem_image(data, w*(bpp/8)*c, 0);
  qDebug() << "copied mem image" << data[0] << data[10];
  
  currentImage = new Buffer<uint16_t>(data, 3, w, h);
  // TODO:delete[] data;

  // Make preview
  float aspect = ((float) w) / h;
  int preview_w = 0, preview_h = 0;
  if ( w > h ) {
    preview_w = 1024;
    preview_h = preview_w / aspect;
  } else {
    preview_h = 1024;
    preview_w = preview_h * aspect;
  }

  Buffer<uint16_t> preview(3, preview_w, preview_h);
  subsampler(*currentImage, preview_w, preview_h, preview);
  emit updateImage(preview.data(), preview_w, preview_h);

  // libraw_processed_image_t * processed = iProcessor.dcraw_make_mem_image();
  // unsigned short* data = (unsigned short*) processed->data;
  // int w = processed->width;
  // int h = processed->height;

  // LibRaw::dcraw_clear_mem(processed);
  //
  // qDebug() << "post processed image" << w << "x" << h;

  // TODO: Save state
  // TODO: Save to Disk
  // TODO: Pipette-style black pt / wp
  // TODO: Presets
  // TODO: multiple images
  // TODO: crop
  // TODO: Batch process
}

void RawProcessor::save(ControlData data) {
  if (!currentImage) {
    qDebug() << "no image open, abort";
    return;
  }
  int width = currentImage->dim(1).extent();
  int height = currentImage->dim(2).extent();
  Buffer<uint16_t> processed(3, width, height);
  qDebug() << "inverting negative"
           << "wp" << data.wp[0] 
                      << data.wp[1]
                      << data.wp[2]
           << "gamma" << data.gamma[0] 
                      << data.gamma[1]
                      << data.gamma[2]
           << "output gamma" << data.output_gamma;
  invert_negative(
      *currentImage, 
      Buffer<float>(data.gamma.data(), 3),
      Buffer<float>(data.wp.data(), 3),
      data.exposure,
      data.bp,
      data.output_gamma,
      processed);
  qDebug() << "saving";

  TIFF *out= TIFFOpen("new.tif", "w");
  TIFFSetField (out, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField (out, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField (out, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 16);
  TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
  TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  tsize_t linebytes = 3 * width * sizeof(uint16_t);
  unsigned char *buf = NULL;        
  //    Allocating memory to store the pixels of current row
  if (TIFFScanlineSize(out) >= linebytes) {
    buf =(unsigned char *)_TIFFmalloc(linebytes);
  }
  else {
    buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));
  }

  // We set the strip size of the file to be size of one row of pixels
  TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, width * 3));

  //Now writing image to the file one strip at a time
  for (uint32 row = 0; row < height; row++)
  {
    memcpy(buf, &processed.data()[(height-row-1)*3*width], linebytes);
    // check the index here, and figure out why not using h*linebytes
    if (TIFFWriteScanline(out, buf, row, 0) < 0)
      break;
  }
  TIFFClose(out);

  qDebug() << ".tiff file saved";

}

RawProcessor::~RawProcessor() {
}
