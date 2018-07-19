#include "ControlsWidget.h"
#include <QDebug>

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

}

void ControlsWidget::sliderChanged(int idx, int val) {
  QString s;
  s.setNum(val);
  gamma_labels[idx]->setText(s);
  emit setGamma(idx, val);
}

ControlsWidget::~ControlsWidget() {
}
