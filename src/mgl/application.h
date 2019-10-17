#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <string>


/**
 * A class
 */
class Application
{
public:
    Application(int width, int height, std::string name);
    virtual ~Application ();

    void teardown();
    void run();

protected:
    void init();
    void draw();

    virtual void render(double currentTime) = 0; // TODO add time
    virtual void setup() = 0;
    virtual void ui() = 0;

    // App info
    int m_width;
    int m_height;
    std::string m_name;
    bool mShouldStop;
    
    // Viewport
    GLFWwindow* m_window = nullptr;
};


std::string read_file(std::string path);

void checkGLError();

GLuint shaderFromFile(std::string filename, GLenum shaderType);

GLuint createGLProgram(GLuint vertex_shader, GLuint fragment_shader);


