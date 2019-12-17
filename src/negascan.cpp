#include <cmath>
#include <glog/logging.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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
    if (mPreviewTexture) {
        glDeleteTextures(1, &mPreviewTexture);
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

    if (mPreviewTexture) {
        LOG(INFO) << "deleting texture";
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &mPreviewTexture);
        mPreviewTexture = 0;
    }

    if (mCurrentImage) {
        mCurrentPreview = mCurrentImage->getPreview(512);
        mHistograms = Histograms(*mCurrentPreview, mHistogramBins);

        mAspectRatio = ((float) mCurrentPreview->width()) /  mCurrentPreview->height();

        glGenTextures(1, &mPreviewTexture);
        glBindTexture(GL_TEXTURE_2D, mPreviewTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mCurrentPreview->width(),
                mCurrentPreview->height(), 0, GL_RGB, GL_UNSIGNED_SHORT, mCurrentPreview->data());
        glBindTexture(GL_TEXTURE_2D, mPreviewTexture);
        checkGLError();
        LOG(INFO) << "changed texture";
    }
}


void Negascan::render(double currentTime)
{
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    float screenAspect = ((float) display_w) / display_h;

    glClearBufferfv(GL_COLOR, 0, (const float *) &mBackgroundColor);

    glUseProgram(mProgram);

    // Scale the patch to match the image's aspect ratio
    glm::mat4 model = glm::mat4(1.0f);
    if (mAspectRatio > 1.0f) {
        model = glm::scale(model, glm::vec3(1.0f, 1.0f/mAspectRatio, 1.0f));
    } else {
        model = glm::scale(model, glm::vec3(1.0f*mAspectRatio, 1.0f, 1.0f));
    }

    float dragSpeed = 4.0f;

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(
            view, glm::vec3(dragSpeed*mTranslation.x/display_w,
                -dragSpeed*mTranslation.y/display_h, 0.0f));
    view = glm::rotate(view, mRotation, glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::scale(view, glm::vec3(mScale, mScale, 1.0f));

    // Correct for screen aspect
    float view_w = 1.0f;
    float view_h = 1.0f;
    if (screenAspect > 0.0f) {
        view_h /= screenAspect;
    } else {
        view_w *= screenAspect;
    }
    glm::mat4 proj = glm::ortho(-view_w, view_w, -view_h, view_h);

    glm::mat4 xform = proj * view * model;

    // LOG(INFO) << "view " << view_w << " " << view_h;
    LOG(INFO) << "view " << glm::to_string(view);
    // LOG(INFO) << "image " << mAspectRatio << " screen " << screenAspect;

    glUniformMatrix4fv(glGetUniformLocation(mProgram, "xform"), 1, false,
            &xform[0][0]);
    glUniform1i(glGetUniformLocation(mProgram, "grayscale"),
            mImageSettings.grayscale);
    glUniform1i(glGetUniformLocation(mProgram, "invert"),
            mImageSettings.invert);
    glUniform1f(glGetUniformLocation(mProgram, "output_gamma"),
            mImageSettings.output_gamma);
    glUniform1f(glGetUniformLocation(mProgram, "exposure"),
            mImageSettings.exposure);
    glUniform3fv(glGetUniformLocation(mProgram, "gamma"),
            1, mImageSettings.gamma.data());
    glUniform3fv(glGetUniformLocation(mProgram, "white_point"),
            1, mImageSettings.wp.data());
    glUniform3fv(glGetUniformLocation(mProgram, "black_point"),
            1, mImageSettings.bp.data());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGLError();

    // TODO: render to texture and recompute histograms
}


void Negascan::ui()
{
    ImGui::Begin("Info");

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1, 0.3, 0.3, 1));
    ImGui::PlotHistogram("red", mHistograms.red(), mHistogramBins);
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.3, 1, 0.3, 1));
    ImGui::PlotHistogram("green", mHistograms.green(), mHistogramBins);
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.3, 0.3, 1, 1));
    ImGui::PlotHistogram("blue", mHistograms.blue(), mHistogramBins);
    ImGui::PopStyleColor();

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
        ImGui::SliderFloat("film gamma",  &mImageSettings.gamma[0], 0.3f, 3.0f, "%.1f");
    } else {
        ImGui::SliderFloat3("film gamma",  mImageSettings.gamma.data(), 0.3f, 3.0f, "%.1f");
    }

    ImGui::SliderFloat3("white point",  mImageSettings.wp.data(), 0.4f, 3.0f, "%.1f");

    // ImGui::ListBox("presets", &mPresets, mPresets.data(), 3, 3);

    ImGui::Separator();

    ImGui::SliderFloat("exposure", &mImageSettings.exposure, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("output gamma", &mImageSettings.output_gamma, 1.0f, 3.0f, "%.1f");
    ImGui::End();

    ImGui::Begin("View");
    ImGui::SliderFloat("scale", &mScale, 0.1f, 4.0f, "%.2f");

    if (ImGui::Button("Rotate Left")) {
        mRotation = fmod(mRotation + M_PI/2, 2.0*M_PI);
    }

    if (ImGui::Button("Rotate Right")) {
        mRotation = fmod(mRotation - M_PI/2, 2.0*M_PI);
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
            ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::IsMouseDragging()) {
        ImVec2 drag = ImGui::GetMouseDragDelta();
        ImGui::ResetMouseDragDelta();
        mTranslation.x += drag.x;
        mTranslation.y += drag.y;
    }

    // Update zoom if needed
    float scroll = ImGui::GetIO().MouseWheel;
    mScale *= glm::exp(0.01*scroll);
    // LOG(INFO) << "scroll " << scroll;

    ImGui::End();
}


void Negascan::setup()
{
    GLuint vs = shaderFromFile(
            "src/shaders/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = shaderFromFile(
            "src/shaders/simple.frag", GL_FRAGMENT_SHADER);

    mProgram = createGLProgram(vs, fs);

    glGenTextures(1, &mPreviewTexture);
    glBindTexture(GL_TEXTURE_2D, mPreviewTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    uint16_t data[3] = {10000, 0, 0};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_SHORT, data);
    glBindTexture(GL_TEXTURE_2D, mPreviewTexture);
    checkGLError();

    // TODO: draw to another frame buffer to compute histograms
    // glGenFramebuffers(1, &mFBO);
    // glBindFramebuffer(GL_FRAMEBUFFER, mFBO);  
    // glBindFramebuffer(GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER);  
    // glDeleteFramebuffers(1, &fbo);

    // glGenTextures(1, &mOutputTexture);
    // glBindTexture(GL_TEXTURE_2D, mOutputTexture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 16, 0, GL_RGB, GL_UNSIGNED_SHORT, 0);
    // glBindTexture(GL_TEXTURE_2D, mOutputTexture);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mOutputTexture, 0);

    // if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //     LOG(ERROR) << "error in the framebuffer";
    // }

    checkGLError();


    // GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, DrawBuffers);


    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    checkGLError();
    LOG(INFO) << "setup done";

    std::string test_image_path = "/Users/mgharbi/Pictures/film_tests/robi_guadalupe2.CR2";
    loadImage(test_image_path);
}
