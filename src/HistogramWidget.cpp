#include "HistogramWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QtDebug>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QTransform>
#include <QShowEvent>


HistogramWidget::HistogramWidget() :
  min(0), max(1), black_point(0.1), white_point(0.5), selected(false)
{
  counts = std::vector<float>(10, 0.3);
  counts[0] = 0.2;
  counts[1] = 0.7;
  counts[2] = 0.1;
  counts[3] = 0.5;
  counts[4] = 0.2;
  counts[5] = 0.8;
  max_count = 1.0;

  setMouseTracking(true);

  QGraphicsScene *scene = new QGraphicsScene(0, 0, 512, 256);
  setScene(scene);

  scene->setBackgroundBrush(QBrush(QColor(60, 60, 60)));

  QBrush brush(QColor(255, 0, 0));
  int w = scene->width();
  int h = scene->height();
  qDebug() << w << " " << h;

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  int n = counts.size();
  float step = w*1.0/n;

  for(int i = 0; i < counts.size(); ++i) {
    float height = (counts[i]/max_count) * h;
    scene->addRect(i*step, h-height, std::ceil(step), height, Qt::NoPen, brush);
  }
  
  QPen pen(Qt::white, 2);
  white_line = scene->addLine(white_pos(), 0, white_pos(), h, pen);
  white_label = scene->addText(QString::number(white_point));
}

HistogramWidget::~HistogramWidget() {
  QGraphicsScene *scene = this->scene();
  if(scene) {
    delete scene;
  }
}

void HistogramWidget::mousePressEvent(QMouseEvent* event) {
  if(white_line == itemAt(event->x(), event->y())) {
    setCursor(Qt::ClosedHandCursor);
    selected = true;
  }
}

void HistogramWidget::mouseReleaseEvent(QMouseEvent* event) {
  selected = false;
  setCursor(Qt::ArrowCursor);
}

void HistogramWidget::mouseMoveEvent(QMouseEvent* event) {
  if(selected) {
    QPointF p = mapToScene(event->pos());
    set_white_pos(p.x());
  } else {
    if (itemAt(event->x(), event->y()) == white_line) {
      setCursor(Qt::OpenHandCursor);
    } else {
      setCursor(Qt::ArrowCursor);
    }
  }
}

float HistogramWidget::set_white_pos(int x) { 
  int w = scene()->width();
  int h = scene()->height();
  x = std::max(std::min(x, w-1), 0);
  white_point = (1.0*x)/w; 
  if(white_line) {
    white_line->setLine(x, 0, x, h);
  }
  return 0.0f;
}

void HistogramWidget::sceneRectChanged(const QRectF &rect) {
}

void HistogramWidget::showEvent(QShowEvent *event) {
}

void HistogramWidget::resizeEvent(QResizeEvent *event) {
  fitInView(this->sceneRect());
}

void HistogramWidget::updateHistogram() {
}
