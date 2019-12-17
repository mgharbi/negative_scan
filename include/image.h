#pragma once

#include <HalideBuffer.h>

class RawProcessor;

class Image
{

friend RawProcessor;

public:
    Image(int w, int h);
    virtual ~Image ();
    std::shared_ptr<Image> getPreview(int preview_size);

    int width() { return mBuffer.extent(1); }
    int height() { return mBuffer.extent(2); }

    uint16_t* data() { return mBuffer.data(); }
    Halide::Runtime::Buffer<uint16_t> & buffer() { return mBuffer; };

protected:
    Image(uint16_t* data, int w, int h);

    Halide::Runtime::Buffer<uint16_t> mBuffer;
};


class Histograms
{

public:
    Histograms(int nbins);
    Histograms(Image im, int nbins);
    float* red();
    float* green();
    float* blue();

    int nbins;

protected:
    Halide::Runtime::Buffer<float> mBuffer;
};
