#include <glog/logging.h>
#include "imgui.h"
#include "negascan.h"


Negascan::Negascan(int width, int height, std::string name)
    : Application(width, height, name), mCurrentImage(nullptr),
    mHistograms(mHistogramBins)
{
};



Negascan::~Negascan()
{
    LOG(INFO) << "Destroying app\n";
    if (mCurrentImage) {
        mCurrentImage.reset();
    }
    glDeleteProgram(mProgram);
    glDeleteVertexArrays(1, &mVAO);
}


void Negascan::loadImage(std::string path) {
    LOG(INFO) << "Loading image";
    if (mCurrentImage) {
        mCurrentImage.reset();
    }

    mCurrentImage = mProcessor.load(path);

    if (mCurrentImage) {
        mCurrentPreview = mCurrentImage->getPreview(512);
        mHistograms = Histograms(*mCurrentPreview, mHistogramBins);
    }
}


void Negascan::render(double currentTime)
{
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearBufferfv(GL_COLOR, 0, (const float *) &mBackgroundColor);

    // glPointSize(42.0f);
    glUseProgram(mProgram);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGLError();
}


void Negascan::ui()
{
    ImGui::Begin("Info");
    ImGui::PlotHistogram("red", mHistograms.red(), mHistogramBins);
    ImGui::PlotHistogram("green", mHistograms.green(), mHistogramBins);
    ImGui::PlotHistogram("blue", mHistograms.blue(), mHistogramBins);
    if (ImGui::Button("Quit")) {
        LOG(INFO) << "exiting\n";
        mShouldStop = true;
    }
    if (ImGui::Button("Save")) {
        LOG(INFO) << "saving image to disk";
    }
    ImGui::End();


    ImGui::Begin("Controls");

    ImGui::Checkbox("grayscale", &mImageSettings.grayscale);
    ImGui::Checkbox("invert", &mImageSettings.invert);

    ImGui::Separator();

    if( mImageSettings.grayscale ) {
        ImGui::SliderFloat("film gamma",  &mImageSettings.gamma[0], 1.0f, 3.0f, "%.1f");
    } else {
        ImGui::SliderFloat3("film gamma",  mImageSettings.gamma.data(), 1.0f, 3.0f, "%.1f");
    }
    // ImGui::ListBox("presets", &mPresets, mPresets.data(), 3, 3);

    ImGui::Separator();

    ImGui::SliderFloat("exposure", &mImageSettings.exposure, -2.0f, 2.0f, "%.2f");
    ImGui::SliderFloat("output gamma", &mImageSettings.output_gamma, 1.0f, 3.0f, "%.1f");
    ImGui::End();

    // if (ImGui::BeginMenuBar()) {
    //     ImGui::EndMenuBar();
    //     if (ImGui::BeginMenu("File"))
    //     {
    //         ImGui::EndMenu();
    //     }
    // }

    // ImGui::ColorEdit3("clear color", (float*)&mBackgroundColor);
    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
    //         ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    

}


void Negascan::setup()
{
    GLuint vs = shaderFromFile(
            "src/shaders/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = shaderFromFile(
            "src/shaders/simple.frag", GL_FRAGMENT_SHADER);

    mProgram = createGLProgram(vs, fs);

    // glCreateTextures();
    // glText
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float data[4] = {0.5f, 0.0f, 0.1f, 1.0f};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    checkGLError();
    // glDeleteTextures(1, &g_FontTexture);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    checkGLError();
}
