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

// PreviewWidget::~PreviewWidget () {
// }

void PreviewWidget::initializeGL() {
  initializeOpenGLFunctions();

  qDebug() << "OpenGL " << context()->format().majorVersion() << "."
           << context()->format().minorVersion();

  program = new QOpenGLShaderProgram;
  program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":preview.vert");
  program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":preview.frag");
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
  img.fill(QColor::fromRgb(255, 128, 128));
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
  texture->release();
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

void PreviewWidget::gammaChanged(int idx, float value) {
  qDebug() << "received gamma change signal" << idx << "val" << value;
  program->bind();
  program->setUniformValue(m_gammaLoc, QVector3D(0, value, 0));
  program->release();
  update();
}

void PreviewWidget::imageChanged(unsigned short* imdata, unsigned long w, unsigned long h) {
  // // TODO : assert / wait for opengl
  // // if (!program) {
  // //   qDebug() << "no available program, aborting";
  // // }
  //
  // // qDebug() << "image has changed" << w << "x" << h << "pixels";
  // // qDebug() << "program is" << program;
  // makeCurrent();
  // qDebug() << "prebound prog";
  // program->bind();
  // qDebug() << "bound prog";
  // if (texture) {
  //   delete texture;
  //   texture = nullptr;
  // }
  // QImage img(512, 512, QImage::Format_RGB888);
  // texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  // img.fill(QColor::fromRgb(255, 128, 128));
  // texture->setData(img);
  // doneCurrent();
  //
  // // program->release();
}
