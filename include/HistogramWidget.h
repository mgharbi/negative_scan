#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QtMath>
#include <vector>

class HistogramWidget : public QGraphicsView
{
signals:
  void whitePointChanged(float value);
  void blackPointChanged(float value);

public slots:
  void updateHistogram();

public:
  HistogramWidget(QWidget *parent=nullptr);
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
  // UI
  QGraphicsLineItem* white_line;
  QGraphicsLineItem* black_line;
  QGraphicsTextItem* white_label;

  // UI behavior
  bool selected;

  float min;
  float max;
  
  // Data
  std::vector<float> counts;
  float max_count;
  float white_point;
  float black_point;

  float white_pos() {return width()*white_point; }
  float set_white_pos(int x);
};
