#pragma once

#include <QObject>
#include <QColor>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QtMath>
#include <vector>

class HistogramWidget : public QGraphicsView
{

  Q_OBJECT

signals:
  void whitePointChanged(float value, int channel);
  void blackPointChanged(float value);

public slots:
  void setData(const float* data, int nbins);
  void setWhitePoint(float wp, int channel);

public:
  HistogramWidget(int channel, QColor color, QWidget *parent=nullptr);
  virtual ~HistogramWidget ();

  // TODO: signals for level change
  // TODO: slot for histogram change

protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void showEvent(QShowEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void sceneRectChanged(const QRectF &rect);

private:
  int channel;
  QColor color;

  // UI
  QGraphicsLineItem* white_line;
  QGraphicsLineItem* black_line;
  QGraphicsTextItem* white_label;

  // UI behavior
  bool selected;

  // float min;
  // float max;
  
  // Data
  // std::vector<float> counts;
  // float max_count;
  float white_point;
  float black_point;

  float white_pos() {return width()*white_point; }
  void set_white_pos(int x);
};
