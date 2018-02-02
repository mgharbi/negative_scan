#include "PreviewWidget.h"
#include <QHBoxLayout>
#include <QLabel>

PreviewWidget::PreviewWidget() {
  QHBoxLayout *layout = new QHBoxLayout;

  QLabel *label = new QLabel("preview");
  layout->addWidget(label);
  setLayout(layout);
}
