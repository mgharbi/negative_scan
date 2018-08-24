#include <RGBControls.h>
#include <QVBoxLayout>
#include <QDebug>

RGBControls::RGBControls(float min_, float max_, int steps, bool isLog, QString header,
            QWidget *parent)
 : min_(min_), max_(max_), steps(steps), isLog(isLog), QWidget(parent)
{

  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel* label = new QLabel(header, this);
  layout->addWidget(label);
  for (int i = 0; i < 3; ++i) {
    sliders[i] = new QSlider(Qt::Horizontal, this);
    labels[i] = new QLabel("", this);
    labels[i]->setFixedWidth(40);

    QWidget *sub_widget = new QWidget(this);
    QHBoxLayout *sub_layout = new QHBoxLayout(sub_widget);
    sub_layout->addWidget(labels[i]);
    sub_layout->addWidget(sliders[i]);
    layout->addWidget(sub_widget);

    sliders[i]->setRange(0, steps);
    sliders[i]->setSingleStep(0);
    sliders[i]->setValue(0);
    sliders[i]->setValue(steps);

    QObject::connect(
        sliders[i], &QSlider::valueChanged, 
        this, [=](int step) { this->sliderChanged(i, step); });
  }
}

RGBControls::~RGBControls() {
}

void RGBControls::sliderChanged(int idx, int step) {
  float fval = stepToValue(step);
  QString s;
  s.setNum(fval, 'f', 4);
  labels[idx]->setText(s);
  emit valueUpdated(idx, fval);
}

void RGBControls::setValue(int idx, float value) {
  int pos = valueToStep(value);
  sliders[idx]->setValue(pos);
  emit valueUpdated(idx, value);
}

int RGBControls::valueToStep(float value) {
  int step = steps * (value - min_) / (max_ - min_);
  return step;
}
float RGBControls::stepToValue(int step) {
  float val = step * (max_ - min_) / steps + min_;
  return val;
}
