#include <glog/logging.h>
#include "negascan.h"


Negascan::Negascan(int width, int height, std::string name)
    : Application(width, height, name)
{
};



Negascan::~Negascan()
{
    LOG(INFO) << "Destroying app\n";
    glDeleteProgram(mProgram);
    glDeleteVertexArrays(1, &mVAO);
}


void Negascan::load_image(std::string path) {
}


void Negascan::render(double currentTime)
{
    // int display_w, display_h;
    // glfwGetFramebufferSize(m_window, &display_w, &display_h);
    // glViewport(0, 0, display_w, display_h);
    glClearBufferfv(GL_COLOR, 0, (const float *) &mBackgroundColor);

    // glPointSize(42.0f);
    // glUseProgram(mProgram);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // checkGLError();
}


void Negascan::ui()
{
    ImGui::Begin("Hello, world!");

    // std::vector<float> values = {0.1f, 0.5f, 0.3f};
    // ImGui::PlotHistogram("some data", values.data(), values.size());

    // ImGui::ColorEdit3("clear color", (float*)&mBackgroundColor);
    // ImGui::SameLine();
    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
    //         ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    // if (ImGui::Button("Quit")) {
    //     LOG(INFO) << "exiting\n";
    //     mShouldStop = true;
    // }
    ImGui::End();
}


void Negascan::setup()
{
    // GLuint vs = shaderFromFile(
    //         "src/shaders/simple.vert", GL_VERTEX_SHADER);
    // GLuint fs = shaderFromFile(
    //         "src/shaders/simple.frag", GL_FRAGMENT_SHADER);
    //
    // mProgram = createGLProgram(vs, fs);
    //
    // glDeleteShader(vs);
    // glDeleteShader(fs);
    //
    // glGenVertexArrays(1, &mVAO);
    // glBindVertexArray(mVAO);
    // checkGLError();
}
