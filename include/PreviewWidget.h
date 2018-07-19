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

#include "ControlData.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture);
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);

class PreviewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

  Q_OBJECT

public:
  PreviewWidget(QWidget *parent=nullptr);
  // virtual ~PreviewWidget ();
  //
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseDoubleClickEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void wheelEvent(QWheelEvent *e);
  
public slots:
  void controlDataChanged(ControlData cdata);
  void imageChanged(unsigned short* imdata, unsigned long w, unsigned long h);

private:
  QOpenGLTexture *texture;
  QOpenGLShaderProgram *program;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vert_pos_vbo;
  QOpenGLBuffer m_tex_coord_vbo;
  int m_projLoc;
  int m_gammaLoc;
  int m_wpLoc;
  int m_exposureLoc;
  int m_bpLoc;
  int m_outGammaLoc;

  QMatrix4x4 projection;
  QMatrix4x4 translation;
  float scale;

  QVector2D mousePreviousPosition;

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void setupVertexAttribs();
  unsigned short *texture_data;

};
