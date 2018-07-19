#include <QHBoxLayout>
#include <QLabel>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector>
#include <QtDebug>

#include "PreviewWidget.h"

PreviewWidget::PreviewWidget(QWidget *parent)
  : QOpenGLWidget(parent){

  QHBoxLayout *layout = new QHBoxLayout;

  setLayout(layout);
}

void PreviewWidget::initializeGL() {
  initializeOpenGLFunctions();

  qDebug() << "OpenGL " << context()->format().majorVersion() << "."
           << context()->format().minorVersion();

  // QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  const char *vsrc =
    "attribute vec3 vertex;\n"
    "varying vec2 screen_pos;\n"
    "void main() {\n"
    "   gl_Position.xyz = vertex;\n"
    "   gl_Position.w = 1.0;\n"
    "   screen_pos = 0.5*(vertex.xy + 1.0);\n"
    "}\n";
  // vshader->compileSourceCode(vsrc);

  // QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  const char *fsrc =
    "varying vec2 screen_pos;\n"
    "void main() {\n"
    "   gl_FragColor = vec4(screen_pos.x, 0.0, 0.0, 1.0);\n"
    "}\n";
  // fshader->compileSourceCode(fsrc);

  program = new QOpenGLShaderProgram;
  // program->addShaderFromSourceFile(QOpenGLShader::Vertex, "/vshader.glsl");
  // program->addShaderFromSourceCode(vshader);
  program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
  program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
  // program->addShader(vshader);
  program->bindAttributeLocation("vertex", 0);
  program->bindAttributeLocation("texCoord", 1);
  program->link();

  program->bind();
  // program->setUniformValue("texture", 0);

  // Rectangle vertices
  static const GLfloat rectangle_vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
  };

  m_vao.create();
  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
  m_rectangle_vbo.create();
  m_rectangle_vbo.bind();
  m_rectangle_vbo.allocate(rectangle_vertices, 4*3*sizeof(GLfloat));

  setupVertexAttribs();

  program->release();

  // texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  // texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt16);
  // unsigned short im[32*32*3];
  // for (int i = 0; i < 32*32*3; ++i) {
  //   im[i] = 10;
  // }
  // texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt16, (void*) im);
  //
  // QVector<GLfloat> vertData;
  // const float coords[4*3] = { -1.0f,  1.0f,  1.0f, 
  //                              1.0f,  1.0f,  1.0f,
  //                              1.0f, -1.0f,  1.0f,
  //                             -1.0f, -1.0f, 1.0f};
  // const float tex_coords[4*2] = { 0.0f, 1.0f, 
  //                                 1.0f, 1.0f,
  //                                 1.0f, 0.0f,
  //                                 0.0f, 0.0f};
  //
  // for (int i = 0; i < 4*3; ++i)
  //   vertData.append(coords[i]);
  // for (int i = 0; i < 4*2; ++i)
  //   vertData.append(tex_coords[i]);
  //
  // vbo.create();
  // vbo.bind();
  // vbo.allocate(vertData.constData(), vertData.count()*sizeof(GLfloat));
  //
  // printf("GL inited\n");
}

void PreviewWidget::setupVertexAttribs()
{
    m_rectangle_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    m_rectangle_vbo.release();

    // normals
    // f->glEnableVertexAttribArray(1);
    // f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
}

void PreviewWidget::paintGL()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
  program->bind();
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  qDebug() << "paint";
  program->release();

  // QMatrix4x4 m;
  // m.ortho(-2.0f, +2.0f, +2.0f, -2.0f, 0.01f, 2.0f);
  // program->setUniformValue("matrix", m);
  // texture-> bind();


  // GLuint vertexbuffer;
  // glGenBuffers(1, &vertexbuffer);
  // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  //
  // program->enableAttributeArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // glVertexAttribPointer(
  //     0,
  //     3,
  //     GL_FLOAT,
  //     GL_FALSE,
  //     0,
  //     (void*)0
  //     );
  // glDrawArrays(GL_TRIANGLES, 0, 3);
  // glDisableVertexAttribArray(0);

  // program->enableAttributeArray(1);
  // program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));
  // program->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(GLfloat), 2, 4 * sizeof(GLfloat));
  //
  // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void PreviewWidget::resizeGL(int width, int height)
{
  qDebug() << "resizing " << width << " " << height;
}

void PreviewWidget::draw()
{
//   glBegin(GL_QUADS);
//   glNormal3f(0,0,-1);
//   glVertex3f(-1,-1,0);
//   glVertex3f(-1,1,0);
//   glVertex3f(1,1,0);
//   glVertex3f(1,-1,0);
// glEnd();
}
