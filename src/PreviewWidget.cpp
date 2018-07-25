#include <QtMath>
#include <QHBoxLayout>
#include <QLabel>
#include <QImage>
#include <QColor>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QOpenGLTexture>
#include <QVector>
#include <QVector3D>
#include <QtDebug>
#include <QPainter>

#include "PreviewWidget.h"

PreviewWidget::PreviewWidget(QWidget *parent)
  : QOpenGLWidget(parent){

    QHBoxLayout *layout = new QHBoxLayout(this);

    rotate_button = new QPushButton("rotate", this);
    QObject::connect(
        rotate_button, &QPushButton::clicked, 
        this, &PreviewWidget::rotate_camera);

    reset_camera();
  }

PreviewWidget::~PreviewWidget () {
  if(texture) {
    delete texture;
    texture = nullptr;
  }
  if(program) {
    delete program;
    program = nullptr;
  }
}

void PreviewWidget::reset_camera () {
  projection.setToIdentity();
  translation.setToIdentity();
  image_ratio.setToIdentity();
  rotation.setToIdentity();
  scale = 1.0f;
}

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
  program->setUniformValue("texture", 0);
  m_gammaLoc = program->uniformLocation("gamma");
  m_wpLoc = program->uniformLocation("white_point");
  m_exposureLoc = program->uniformLocation("exposure");
  m_bpLoc = program->uniformLocation("black_point");
  m_outGammaLoc = program->uniformLocation("output_gamma");
  m_grayscaleLoc = program->uniformLocation("grayscale");
  m_invertLoc = program->uniformLocation("invert");
  m_projLoc = program->uniformLocation("proj_mtx");

  program->setUniformValue(m_gammaLoc, QVector3D(1, 1, 1));
  program->setUniformValue(m_wpLoc, QVector3D(1, 1, 1));
  program->setUniformValue(m_exposureLoc, 1.0f);
  program->setUniformValue(m_bpLoc, 0.0f);
  program->setUniformValue(m_outGammaLoc, 1.0f);
  program->setUniformValue(m_grayscaleLoc, false);
  program->setUniformValue(m_invertLoc, true);

  QMatrix4x4 m;
  m.setToIdentity();
  program->setUniformValue(m_projLoc, m);

  GLenum glErr = GL_NO_ERROR;
  while((glErr = glGetError()) != GL_NO_ERROR) {
    qDebug("ERROR Setting proj");
  }

  static const GLfloat rectangle_vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    -1.0f,  1.0f, 0.0f,
  };

  static const GLfloat tex_coords[] = { 0.0f, 1.0f, 
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f};

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
  QImage img(1, 1, QImage::Format_RGB888);
  img.fill(QColor::fromRgb(128, 128, 128));
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
  QMatrix4x4 model;
  model.setToIdentity();
  model.translate(0, 0, -1);  // Translate scene in camera's fulcrum

  QMatrix4x4 scale_mtx;
  scale_mtx.scale(scale, scale, 1);

  QPainter p(this);

  p.beginNativePainting();

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_FRAMEBUFFER_SRGB);

  program->bind();

  program->setUniformValue(m_projLoc, projection*translation*scale_mtx*rotation*model*image_ratio);

  if (texture) {
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    texture->release();
    vaoBinder.release();
  } else {
    qDebug() << "no texture available";
  }
  program->release();

  GLenum glErr = GL_NO_ERROR;
  while((glErr = glGetError()) != GL_NO_ERROR) {
    qDebug("ERROR Painting Opengl");
  }

  p.endNativePainting();

  QString s("gamma: ");
  p.drawText(QPointF(100.5, 100.5), s+display_gamma);

  // Draw the crop region
  // QRect r(10, 10, 100, 100);
  // QPen pen = p.pen();
  // pen.setStyle(Qt::DotLine);
  // p.setPen(pen);
  // p.drawRect(r);

  p.end();
}

void PreviewWidget::resizeGL(int w, int h)
{
  // Calculate aspect ratio
  qreal aspect = qreal(w) / qreal(h ? h : 1);

  // Reset projection
  projection.setToIdentity();

  qreal height = 2.0;
  qreal width = height * aspect;

  // Set perspective projection
  const qreal zNear = 0.1, zFar = 3.0, fov = 45.0;
  projection.ortho(-width*0.5f, width*0.5f, -height*0.5f, height*0.5f, 0.0f, 10.0f);
}

void PreviewWidget::controlDataChanged(ControlData cdata) {
  program->bind();
  program->setUniformValue(m_gammaLoc, QVector3D(cdata.gamma[0], cdata.gamma[1], cdata.gamma[2]));
  program->setUniformValue(m_wpLoc, QVector3D(cdata.wp[0], cdata.wp[1], cdata.wp[2]));
  program->setUniformValue(m_exposureLoc, cdata.exposure);
  program->setUniformValue(m_bpLoc, cdata.bp);
  program->setUniformValue(m_outGammaLoc, cdata.output_gamma);
  program->setUniformValue(m_grayscaleLoc, cdata.grayscale);
  program->setUniformValue(m_invertLoc, cdata.invert);
  program->release();

  display_gamma.setNum(cdata.output_gamma, 'f', 4);

  update();
}

void PreviewWidget::imageChanged(unsigned short* imdata, unsigned long w, unsigned long h) {
  if (!context()) {
    return;
  }

  makeCurrent();
  if (texture) {
    qDebug() << "Deleting old texture";
    delete texture;
    texture = nullptr;
  }

  qDebug() << "Setting new texture";
  QOpenGLTexture::PixelType pixType = QOpenGLTexture::UInt16;
  QOpenGLTexture::PixelFormat pixFormat = QOpenGLTexture::RGB;
  QOpenGLTexture::TextureFormat texFormat = QOpenGLTexture::RGB16_UNorm;
  texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
  texture->setSize(w, h);
  texture->setFormat(texFormat);
  texture->allocateStorage();
  texture->setData(pixFormat, pixType, (void*) imdata);

  GLenum glErr = GL_NO_ERROR;
  while((glErr = glGetError()) != GL_NO_ERROR) {
    qDebug("ERROR LOADING TEXTURES");
  }

  image_ratio.setToIdentity();
  qreal aspect = w*1.0 / h;
  if(aspect > 1.0) {
    image_ratio.scale(1.0, 1.0/aspect);
  } else {
    image_ratio.scale(aspect, 1.0);
  }

  doneCurrent();
  update();
}

void PreviewWidget::mousePressEvent(QMouseEvent *e) {
  mousePreviousPosition = QVector2D(e->localPos());
}

void PreviewWidget::mouseMoveEvent(QMouseEvent *e) {
  QVector2D diff = QVector2D(e->localPos()) - mousePreviousPosition;
  mousePreviousPosition = QVector2D(e->localPos());

  diff.setY(-diff.y());

  diff /= QVector2D(size().width(), size().height());

  translation.translate(diff);
  update();
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *e) {
}

void PreviewWidget::mouseDoubleClickEvent(QMouseEvent *e) {
  translation.setToIdentity();
  scale = 1.0f;
  update();
}

void PreviewWidget::rotate_camera() {
  rotation.rotate(-90, 0.0f, 0.0f, 1.0f);
  update();
}

void PreviewWidget::wheelEvent(QWheelEvent *e) {
  // bounded scaling
  scale *= qPow(1.001, e->angleDelta().y());
  scale = qMin(scale, 10.0f);
  scale = qMax(scale, 0.2f);
  update();

  // TODO: center zoom on mouse
}
