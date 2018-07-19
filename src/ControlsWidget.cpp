#include "ControlsWidget.h"
#include <QDebug>
#include <QtMath>
#include <QHBoxLayout>
#include <QVBoxLayout>

ControlsWidget::ControlsWidget(QWidget *parent)
  : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  QWidget *wb_widget = new QWidget();
  QVBoxLayout *wb_layout = new QVBoxLayout(wb_widget);
  wb_header_label = new QLabel("White Point", this);
  wb_layout->addWidget(wb_header_label);
  layout->addWidget(wb_widget);

  QWidget *gamma_widget = new QWidget();
  QVBoxLayout *gamma_layout = new QVBoxLayout(gamma_widget);
  gamma_header_label = new QLabel("Gamma", this);
  gamma_layout->addWidget(gamma_header_label);
  layout->addWidget(gamma_widget);

  // TODO: refactor as separate widget
  for (int i = 0; i < 3; ++i) {
    QSlider *s = new QSlider(Qt::Horizontal, this);
    QLabel *l = new QLabel("", this);
    wb_sliders[i] = s;
    wb_labels[i] = l;

    QWidget *sub_widget = new QWidget(this);
    QHBoxLayout *sub_layout = new QHBoxLayout(sub_widget);
    sub_layout->addWidget(l);
    sub_layout->addWidget(s);
    wb_layout->addWidget(sub_widget);

    QObject::connect(
        s, &QSlider::valueChanged, 
        this, [=](int val) { this->sliderChanged(i, val); });

    s->setRange(0, 100);
    s->setSingleStep(1);
    s->setValue(1);
    s->setValue(100);

    // Gamma
    s = new QSlider(Qt::Horizontal, this);
    l = new QLabel("", this);
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
  if (idx < 0 || idx > 6) {
    return;
  }

  qDebug() << idx << "changed to" << val;

  QString s;
  float value = 0.0f;
  if(idx < 3) { // white-balance
    value = val*1.0f/100;
    s.setNum(value, 'g', 2);
    wb_labels[idx]->setText(s);
  } else { // gamma
    value = qExp(val*0.1f);
    s.setNum(value, 'g', 2);
    gamma_labels[idx - 3]->setText(s);
  }
  // emit setGamma(idx, value);
}

ControlsWidget::~ControlsWidget() {
}
