#include "ControlsWidget.h"
#include <QDebug>
#include <QtMath>
#include <QHBoxLayout>
#include <QVBoxLayout>

ControlsWidget::ControlsWidget(QWidget *parent)
  : QWidget(parent)
{
  wp_steps = 5000;
  gamma_steps = 1000;
  QVBoxLayout *layout = new QVBoxLayout(this);

  QWidget *wp_widget = new QWidget();
  QVBoxLayout *wp_layout = new QVBoxLayout(wp_widget);
  wp_header_label = new QLabel("Negative White Point", this);
  wp_layout->addWidget(wp_header_label);
  layout->addWidget(wp_widget);

  QWidget *gamma_widget = new QWidget();
  QVBoxLayout *gamma_layout = new QVBoxLayout(gamma_widget);
  gamma_header_label = new QLabel("Gamma", this);
  gamma_layout->addWidget(gamma_header_label);
  layout->addWidget(gamma_widget);

  QWidget *positive_widget = new QWidget();
  QVBoxLayout *positive_layout = new QVBoxLayout(positive_widget);
  QLabel *positive_header = new QLabel("Exposure, BP, out gamma", this);
  positive_layout->addWidget(positive_header);
  layout->addWidget(positive_widget);

  exposure_slider = new QSlider(Qt::Horizontal, this);
  bp_slider = new QSlider(Qt::Horizontal, this);
  out_gamma_slider = new QSlider(Qt::Horizontal, this);
  positive_layout->addWidget(exposure_slider);
  positive_layout->addWidget(bp_slider);
  positive_layout->addWidget(out_gamma_slider);

  QObject::connect(
      exposure_slider, &QSlider::valueChanged, 
      this, [=](int val) { this->sliderChanged(6, val); });
  QObject::connect(
      bp_slider, &QSlider::valueChanged, 
      this, [=](int val) { this->sliderChanged(7, val); });
  QObject::connect(
      out_gamma_slider, &QSlider::valueChanged, 
      this, [=](int val) { this->sliderChanged(8, val); });

  exposure_slider->setRange(-1000, 1000);
  exposure_slider->setValue(1);
  exposure_slider->setValue(0);

  bp_slider->setRange(0, 1000);
  bp_slider->setValue(1);
  bp_slider->setValue(0);

  out_gamma_slider->setRange(-gamma_steps, gamma_steps);
  out_gamma_slider->setValue(1);
  out_gamma_slider->setValue(0);

  // TODO: refactor as separate widget
  for (int i = 0; i < 3; ++i) {
    QSlider *s = new QSlider(Qt::Horizontal, this);
    QLabel *l = new QLabel("", this);
    l->setFixedWidth(40);
    wp_sliders[i] = s;
    wp_labels[i] = l;

    QWidget *sub_widget = new QWidget(this);
    QHBoxLayout *sub_layout = new QHBoxLayout(sub_widget);
    sub_layout->addWidget(l);
    sub_layout->addWidget(s);
    wp_layout->addWidget(sub_widget);

    QObject::connect(
        s, &QSlider::valueChanged, 
        this, [=](int val) { this->sliderChanged(i, val); });

    s->setRange(1, wp_steps);
    s->setSingleStep(1);
    s->setValue(1);
    s->setValue(wp_steps);

    // Gamma
    s = new QSlider(Qt::Horizontal, this);
    l = new QLabel("", this);
    l->setFixedWidth(40);
    gamma_sliders[i] = s;
    gamma_labels[i] = l;

    sub_widget = new QWidget(this);
    sub_layout = new QHBoxLayout(sub_widget);
    sub_layout->addWidget(l);
    sub_layout->addWidget(s);
    gamma_layout->addWidget(sub_widget);

    QObject::connect(
        s, &QSlider::valueChanged, 
        this, [=](int val) { this->sliderChanged(3 + i, val); });

    s->setRange(-gamma_steps, gamma_steps);
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
  float value = 0.0f;

  if(idx < 3) { // white-balance
    value = val*1.0f/wp_steps;
    // value = val*1.0f/((1<<16) - 1);
    data.wp[idx] = value;
    s.setNum(value, 'f', 4);
    wp_labels[idx]->setText(s);
  } else if ( idx < 6 ){ // gamma
    value = qPow(2.0f, val*2.0f/gamma_steps);
    data.gamma[idx - 3] = value;
    s.setNum(value, 'f', 4);
    gamma_labels[idx - 3]->setText(s);
  } else if (idx == 6){ // exposure
    value = qPow(2.0f, val*3.0f/1000);
    data.exposure = value;
  } else if (idx == 7){ // black point
    value = val*1.0f/1000;
    data.bp = value;
  } else if (idx == 8){ // gamma
    value = qPow(2.0f, val*2.0f/gamma_steps);
    data.output_gamma = value;
  }
  emit updateControlData(data);
}

ControlsWidget::~ControlsWidget() {
}
