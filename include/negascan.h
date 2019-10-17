#pragma once

#include "mgl/application.h"

class Negascan : public Application
{
public:
    Negascan(int width, int height, std::string name);
    virtual ~Negascan();

    void load_image(std::string path);

protected:
    virtual void render(double currentTime);
    virtual void ui();
    virtual void setup();

    // Data
    ImVec4 mBackgroundColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    GLuint mVAO;
    GLuint mProgram;
};
