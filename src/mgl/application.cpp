#include "imgui.h"
#include "application.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <glog/logging.h>


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Application::Application(int width, int height, std::string name) :
    m_width(width), m_height(height), m_name(name), mShouldStop(false) {
    init();
}

void Application::init() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        std::cout << "glfw could not be initialized\n";
        return;
    }

    const char* glsl_version = "#version 410";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
    if (m_window == NULL) {
        std::cout << "window could not be created";
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    bool err = glewInit() != GLEW_OK;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    // Setup bindings
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Application::teardown() {
}

Application::~Application() {
    teardown();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    m_window = nullptr;
    glfwTerminate();
}


void Application::draw() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ui();
    ImGui::Render();

    render(0.0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void Application::run() {
    setup();
    while (!mShouldStop && !glfwWindowShouldClose(m_window)) {
        draw();
    }
}

std::string read_file(std::string path)  {
    std::ifstream file(path);
    std::string line;
    std::string out;
    out.assign(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());

    // if (file.is_open()) {
    //     while (getline (file, line) ) {
    //         out += line + "\n";
    //     }
    //     file.close();
    // }
    return out;
}

void checkGLError() {
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
        return;
    }
    switch(err) {
        case GL_INVALID_ENUM:
            LOG(INFO) << "GL error: invalid enum\n";
            break;
        case GL_INVALID_VALUE:
            LOG(INFO) << "GL error: invalid value\n";
            break;
        case GL_INVALID_OPERATION:
            LOG(INFO) << "GL error: invalid operation\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOG(INFO) << "GL error: invalid framebuffer operation\n";
            break;
        case GL_OUT_OF_MEMORY:
            LOG(INFO) << "GL error: out of memory\n";
            break;
        case GL_STACK_OVERFLOW:
            LOG(INFO) << "GL error: stack overflow\n";
            break;
        case GL_STACK_UNDERFLOW:
            LOG(INFO) << "GL error: stack underflow\n";
            break;
        default:
            LOG(ERROR) << "OpenGL error" << err << "!\n";
    }
    throw;
}

GLuint shaderFromFile(std::string filename, GLenum shaderType) {
    std::string src_str = read_file(filename);
    const GLchar* src = src_str.c_str();
    LOG(INFO) << "shader source " << src  << "\n";

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar**) &src, NULL);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        LOG(ERROR) << "Shader " << filename << " did not compile\n";
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        LOG(ERROR) << errorLog.data();
        glDeleteShader(shader);
        throw;
    }
    return shader;
}

GLuint createGLProgram(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        LOG(ERROR) << "Program with vs="<< vertex_shader
                   << "and fs=" << fragment_shader 
                   << " could not be linked\n";
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

        LOG(ERROR) << errorLog.data();
        glDeleteProgram(program);
        throw;


    }
    LOG(INFO) << "Created program " << program << "\n";
    return program;
}
