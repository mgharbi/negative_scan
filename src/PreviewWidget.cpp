#include <QHBoxLayout>
#include <QLabel>
#include <QImage>
#include <QColor>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector>
#include <QVector3D>
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

  const char *vsrc =
    "attribute vec3 vertex;\n"
    "attribute vec2 vert_texcoord;\n"
    "varying vec2 screen_pos;\n"
    "varying vec2 texcoord;\n"
    "void main() {\n"
    "   gl_Position.xyz = vertex;\n"
    "   gl_Position.w = 1.0;\n"
    "   texcoord = vert_texcoord;\n"
    "   screen_pos = 0.5*(vertex.xy + 1.0);\n"
    "}\n";

  const char *fsrc =
    "varying vec2 screen_pos;\n"
    "varying vec2 texcoord;\n"
    "uniform sampler2D texture;\n"
    "uniform vec3 gamma;\n"
    "uniform vec3 white_point;\n"
    "void main() {\n"
    " //  gl_FragColor = vec4(texcoord, 1.0, 1.0);\n"
    "   gl_FragColor = texture2D(texture, texcoord);\n"
    " //  gl_FragColor = vec4(gamma*vec3(screen_pos.x), 1.0);\n"
    "}\n";

  program = new QOpenGLShaderProgram;
  program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
  program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
  program->bindAttributeLocation("vertex", 0);
  program->bindAttributeLocation("texcoord", 1);
  program->link();

  program->bind();
  m_gammaLoc = program->uniformLocation("gamma");
  m_wpLoc = program->uniformLocation("white_point");

  program->setUniformValue("texture", 0);
  program->setUniformValue(m_gammaLoc, QVector3D(0, 1, 0));
  program->setUniformValue(m_wpLoc, QVector3D(0, 1, 0));

  // Rectangle vertices
  static const GLfloat rectangle_vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
  };

  static const GLfloat tex_coords[] = { 0.0f, 0.0f, 
                                        1.0f, 0.0f,
                                        1.0f, 1.0f,
                                        0.0f, 1.0f};

  m_vao.create();
  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
  m_vert_pos_vbo.create();
  m_vert_pos_vbo.bind();
  m_vert_pos_vbo.allocate(rectangle_vertices, 4*3*sizeof(GLfloat));

  m_tex_coord_vbo.create();
  m_tex_coord_vbo.bind();
  m_tex_coord_vbo.allocate(tex_coords, 4*2*sizeof(GLfloat));
  
  setupVertexAttribs();

  program->release();

  texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  QImage img(512, 512, QImage::Format_RGB888);
  img.fill(QColor::fromRgb(255, 0, 0));
  texture->setData(img);
}

void PreviewWidget::setupVertexAttribs()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_vert_pos_vbo.bind();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    m_vert_pos_vbo.release();

    m_tex_coord_vbo.bind();
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    m_tex_coord_vbo.release();
}

void PreviewWidget::paintGL()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
  program->bind();
  texture-> bind();
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  qDebug() << "paint";
  program->release();

  // QMatrix4x4 m;
  // m.ortho(-2.0f, +2.0f, +2.0f, -2.0f, 0.01f, 2.0f);
  // program->setUniformValue("matrix", m);
}

void PreviewWidget::resizeGL(int width, int height)
{
  // TODO(mgharbi): click+hold to drag, wheel to zoom, preserve aspect ratio, camera matrix transform
  // TODO(mgharbi): sliders to adjust image properties
  qDebug() << "resizing " << width << " " << height;
}
