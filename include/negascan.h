#pragma once

#include <memory>

#include "mgl/application.h"

#include "image.h"
#include "raw_processor.h"
#include "control_data.h"


class Negascan : public Application
{
public:
    Negascan(int width, int height, std::string name);
    virtual ~Negascan();

    void loadImage(std::string path);

    void updateHistograms(Image &im);

protected:
    virtual void render(double currentTime);
    virtual void ui();
    virtual void setup();

    RawProcessor mProcessor;

    std::shared_ptr<Image> mCurrentImage;
    std::shared_ptr<Image> mCurrentPreview;
    const int mHistogramBins = 256;
    Histograms mHistograms;

    ControlData mImageSettings;

    int mSelectedPreset = 0;
    std::vector<const char*> mPresets = {
        "<no_preset>",
        "portra160",
        "portra400",
        "ektar100",
    };

    ImVec4 mBackgroundColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    GLuint mVAO;
    GLuint mTexture;
    GLuint mProgram;
};
