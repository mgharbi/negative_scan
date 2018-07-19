#include "RawProcessor.h"
#include <QDebug>

RawProcessor::RawProcessor() {
}

void RawProcessor::load() {
  iProcessor.imgdata.params.half_size = true;

  iProcessor.imgdata.params.gamm[0] = 1.0;
  iProcessor.imgdata.params.gamm[1] = 1.0;

  iProcessor.imgdata.params.cropbox[0] = 500;
  iProcessor.imgdata.params.cropbox[1] = 500;
  iProcessor.imgdata.params.cropbox[2] = 4000;
  iProcessor.imgdata.params.cropbox[3] = 4000;

  // iProcessor.imgdata.params.colors = 3;
  iProcessor.imgdata.params.output_bps = 16;
  // iProcessor.imgdata.params.no_auto_bright = true

  // iProcessor.imgdata.params.output_color = 0;  // raw space

  qDebug() << "use camera wb" << iProcessor.imgdata.params.use_camera_wb;
  qDebug() << "use auto wb" << iProcessor.imgdata.params.use_auto_wb;
  qDebug() << "use camera matrix" << iProcessor.imgdata.params.use_camera_matrix;
  qDebug() << "no auto bright" << iProcessor.imgdata.params.no_auto_bright;
  qDebug() << "tone curve" << iProcessor.imgdata.params.gamm[0]
                           << iProcessor.imgdata.params.gamm[1];

  iProcessor.open_file("../data/FilmScans20180451.CR2");
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
  // delete[] data;

  // libraw_processed_image_t * processed = iProcessor.dcraw_make_mem_image();
  // unsigned short* other_data = (unsigned short*) processed->data;
  // qDebug() << "copied mem image 2nd option" << other_data[0] << other_data[10];
  // LibRaw::dcraw_clear_mem(processed);
  // qDebug() << "image type" << iProcessor.imgdata.type;

  // unsigned short* raw = iProcessor.imgdata.rawdata.raw_image;

  emit updateImage(data, w, h);

  // printf("Raw 0 %d\n", raw[1]);

//   iProcessor.imgdata.filter;
//   int flip;
// Image orientation (0 if does not require rotation; 3 if requires 180-deg rotation; 5 if 90 deg counterclockwise, 6 if 90 deg clockwise).
}

RawProcessor::~RawProcessor() {
}
