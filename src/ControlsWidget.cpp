#include "ControlsWidget.h"
#include <QDebug>
#include <QtMath>
#include <QHBoxLayout>
#include <QVBoxLayout>

ControlsWidget::ControlsWidget(QWidget *parent)
  : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  setLayout(layout);

  for (int i = 0; i < 3; ++i) {
    QSlider *s = new QSlider(Qt::Horizontal, this);
    QLabel *l = new QLabel("joe", this);

    QWidget *sub_widget = new QWidget(this);
    QHBoxLayout *sub_layout = new QHBoxLayout();
    sub_layout->addWidget(l);
    sub_layout->addWidget(s);
    sub_widget->setLayout(sub_layout);
    layout->addWidget(sub_widget);
    gamma_sliders[i] = s;
    gamma_labels[i] = l;

    QObject::connect(
        s, &QSlider::valueChanged, 
        this, [=](int val) { this->sliderChanged(i, val); });

    s->setRange(-10, 10);
    s->setSingleStep(1);
    s->setValue(-1);
    s->setValue(0);
  }
  load_button = new QPushButton("load", this);
  layout->addWidget(load_button);
  QObject::connect(
      load_button, &QPushButton::clicked, 
      this, &ControlsWidget::loadImage);
}

void ControlsWidget::sliderChanged(int idx, int val) {
  QString s;
  float value = encodeSliderValue(val);
  s.setNum(value, 'g', 2);
  gamma_labels[idx]->setText(s);
  emit setGamma(idx, value);
}

ControlsWidget::~ControlsWidget() {
}

float ControlsWidget::encodeSliderValue(int val) {
  return qExp(val*0.1f);
}
