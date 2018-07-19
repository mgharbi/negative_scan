#pragma once

#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>


QT_FORWARD_DECLARE_CLASS(QOpenGLTexture);
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);

class PreviewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
  PreviewWidget(QWidget *parent=nullptr);
  // virtual ~PreviewWidget ();

private:
  QOpenGLTexture *texture;
  QOpenGLShaderProgram *program;
  QOpenGLBuffer vbo;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_rectangle_vbo;

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void draw();
  void setupVertexAttribs();

};
