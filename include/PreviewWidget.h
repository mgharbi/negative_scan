#pragma once

#include <QtGlobal>
#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QVector2D>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPushButton>

#include "ControlData.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture);
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);

class PreviewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

  Q_OBJECT

public:
  PreviewWidget(QWidget *parent=nullptr);
  virtual ~PreviewWidget ();

  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseDoubleClickEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void wheelEvent(QWheelEvent *e);

signals:
  void pickWhitePoint(float x, float y);
  
public slots:
  void controlDataChanged(ControlData cdata);
  void imageChanged(unsigned short* imdata, unsigned long w, unsigned long h, float* camera_rgb);

private:
  QOpenGLTexture *texture;
  QOpenGLShaderProgram *program;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vert_pos_vbo;
  QOpenGLBuffer m_tex_coord_vbo;
  int m_projLoc;
  int m_gammaLoc;
  int m_wpLoc;
  int m_bpLoc;
  int m_exposureLoc;
  int m_outGammaLoc;
  int m_grayscaleLoc;
  int m_invertLoc;
  int m_cameraRGBLoc;

  // Camera transform
  QMatrix4x4 image_ratio;
  QMatrix4x4 projection;
  QMatrix4x4 translation;
  QMatrix4x4 rotation;
  float scale;

  // UI interaction
  QVector2D mousePreviousPosition;

  QPushButton *rotate_button;
  QString display_gamma;

  void rotate_camera();
  void reset_camera();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void setupVertexAttribs();
};
