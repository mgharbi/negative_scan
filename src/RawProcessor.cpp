#include "RawProcessor.h"
#include <QDebug>

RawProcessor::RawProcessor() {
}

void RawProcessor::load() {
  iProcessor.imgdata.params.half_size = true;

  // rotate
  iProcessor.imgdata.params.user_flip = 3;

  iProcessor.imgdata.params.gamm[0] = 1.0;
  iProcessor.imgdata.params.gamm[1] = 1.0;

  // iProcessor.imgdata.params.cropbox[0] = 0;
  // iProcessor.imgdata.params.cropbox[1] = 0;
  // iProcessor.imgdata.params.cropbox[2] = 4500;
  // iProcessor.imgdata.params.cropbox[3] = 3000;

  iProcessor.imgdata.params.output_bps = 16;
  iProcessor.imgdata.params.no_auto_bright = true;
  iProcessor.imgdata.params.output_color = 1;  // linear sRGB

  iProcessor.imgdata.params.use_camera_wb = true;
  iProcessor.imgdata.params.use_camera_matrix = true;

  qDebug() << "use camera wb" << iProcessor.imgdata.params.use_camera_wb;
  qDebug() << "use auto wb" << iProcessor.imgdata.params.use_auto_wb;
  qDebug() << "use camera matrix" << iProcessor.imgdata.params.use_camera_matrix;
  qDebug() << "no auto bright" << iProcessor.imgdata.params.no_auto_bright;
  qDebug() << "tone curve" << iProcessor.imgdata.params.gamm[0]
                           << iProcessor.imgdata.params.gamm[1];

  iProcessor.open_file("../data/silvia.dng");
  // iProcessor.open_file("../data/FilmScans20180451.CR2");
  iProcessor.unpack();
  iProcessor.dcraw_process();

  // int w = 0;
  // int h = 0;
  // int c = 0;
  // int bpp = 0;
  // iProcessor.get_mem_image_format(&w, &h, &c, &bpp);
  // qDebug() << "post processed image" << w << "x" << h << "with" << c <<
  //   "colors and" << bpp << "bpp";
  // if(bpp != 16) {
  //   qDebug() << "unexpected bit depth";
  // }
  // unsigned short* data = new unsigned short[w*h*c];
  // iProcessor.copy_mem_image(data, w*(bpp/8)*c, 0);
  // qDebug() << "copied mem image" << data[0] << data[10];
  // // TODO:delete[] data;

  libraw_processed_image_t * processed = iProcessor.dcraw_make_mem_image();
  unsigned short* data = (unsigned short*) processed->data;
  int w = processed->width;
  int h = processed->height;

  int w2 = w / 2;
  int h2 = h / 2;
  unsigned short* ds_data = new unsigned short[w2*h2*3];

  qDebug() << "subsample and convert uint16";
  for (int y = 0; y < h2; ++y)
  for (int x = 0; x < w2; ++x)
  {
    int idx = 3*(2*x + 2*y*w);
    int idx_ds = 3*(x + y*w2);
    ds_data[idx_ds + 0] = data[idx + 0];
    ds_data[idx_ds + 1] = data[idx + 1];
    ds_data[idx_ds + 2] = data[idx + 2];
    
  }
  qDebug() << "subsample done";
  LibRaw::dcraw_clear_mem(processed);

  // qDebug() << "image type" << iProcessor.imgdata.type;

  qDebug() << "post processed image" << w << "x" << h;

  emit updateImage(ds_data, w2, h2);
  // emit updateImage(data, w, h);

  // TODO: HalideProcessor
  // TODO: Save to Disk
  // TODO: Batch process
  // TODO: BlackAndWhite switch
  // TODO: Pipette-style black pt / wp
  //
  // printf("Raw 0 %d\n", raw[1]);
//   iProcessor.imgdata.filter;
//   int flip;
// Image orientation (0 if does not require rotation; 3 if requires 180-deg rotation; 5 if 90 deg counterclockwise, 6 if 90 deg clockwise).
}

RawProcessor::~RawProcessor() {
}
