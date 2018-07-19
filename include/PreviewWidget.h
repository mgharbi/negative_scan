#pragma once

#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "ControlData.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture);
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);

class PreviewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

  Q_OBJECT

public:
  PreviewWidget(QWidget *parent=nullptr);
  // virtual ~PreviewWidget ();
  
public slots:
  void controlDataChanged(ControlData cdata);
  void imageChanged(unsigned short* imdata, unsigned long w, unsigned long h);

private:
  QOpenGLTexture *texture;
  QOpenGLShaderProgram *program;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vert_pos_vbo;
  QOpenGLBuffer m_tex_coord_vbo;
  int m_gammaLoc;
  int m_wpLoc;
  int m_exposureLoc;
  int m_bpLoc;
  int m_outGammaLoc;

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void setupVertexAttribs();
  unsigned short *texture_data;

};
