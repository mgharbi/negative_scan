#include "RawProcessor.h"

RawProcessor::RawProcessor() {
}

void RawProcessor::load() {
  iProcessor.open_file("../data/FilmScans20180451.CR2");

  printf("Image size %dx%d\n",
         iProcessor.imgdata.sizes.iwidth,
         iProcessor.imgdata.sizes.iheight);

  iProcessor.unpack();
  unsigned short* raw = iProcessor.imgdata.rawdata.raw_image;
  emit updateImage(raw, 
    iProcessor.imgdata.sizes.iwidth, iProcessor.imgdata.sizes.iheight);

  printf("Raw 0 %d\n", raw[1]);

//   iProcessor.imgdata.filter;
//   int flip;
// Image orientation (0 if does not require rotation; 3 if requires 180-deg rotation; 5 if 90 deg counterclockwise, 6 if 90 deg clockwise).
}

RawProcessor::~RawProcessor() {
}
