#include "HistogramWidget.h"
#include <QHBoxLayout>
#include <QLabel>

HistogramWidget::HistogramWidget() {
  QHBoxLayout *layout = new QHBoxLayout;

  QLabel *label = new QLabel("histogram");
  layout->addWidget(label);
  setLayout(layout);
}
