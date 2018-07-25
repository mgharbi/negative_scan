#include "ControlsWidget.h"
#include <QDebug>
#include <QtMath>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QtGlobal>

ControlsWidget::ControlsWidget(QWidget *parent)
  : QWidget(parent)
{
  wp_steps = 5000;
  gamma_steps = 5000;
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *presets_label = new QLabel("Presets", this);
  layout->addWidget(presets_label);
  presets_list = new QComboBox(this);
  layout->addWidget(presets_list);
  setPresets();

  grayscale_button = new QCheckBox("b/w", this);
  layout->addWidget(grayscale_button);
  QObject::connect(
      grayscale_button, &QCheckBox::clicked, 
      this, &ControlsWidget::grayscaleToggle);

  invert_button = new QCheckBox("invert", this);
  layout->addWidget(invert_button);
  QObject::connect(
      invert_button, &QCheckBox::clicked, 
      this, &ControlsWidget::invertToggle);

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

  }
  
  load_button = new QPushButton("load", this);
  load_button->setToolTip(tr("load a raw file for processing"));
  layout->addWidget(load_button);

  save_button = new QPushButton("save", this);
  save_button->setToolTip(tr("save processed file"));
  layout->addWidget(save_button);

  reset_button = new QPushButton("reset", this);
  reset_button->setToolTip(tr("reset settings"));
  layout->addWidget(reset_button);

  QObject::connect(
      load_button, &QPushButton::clicked, 
      this, &ControlsWidget::loadImage);

  QObject::connect(
      save_button, &QPushButton::clicked, 
      this, [=] () { ControlsWidget::requestSave(data); } );

  QObject::connect(
      reset_button, &QPushButton::clicked, 
      this, &ControlsWidget::reset);

  reset();
}

ControlsWidget::~ControlsWidget() {
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
    value = 1.0f + 2.0f*val/gamma_steps;
    // value = qPow(2.0f, val*1.5f/gamma_steps);
    data.gamma[idx - 3] = value;
    s.setNum(value, 'f', 4);
    gamma_labels[idx - 3]->setText(s);
  } else if (idx == 6){ // exposure
    value = qPow(2.0f, val*8.0f/1000);
    data.exposure = value;
  } else if (idx == 7){ // black point
    value = val*4.0f/1000;
    data.bp = value;
  } else if (idx == 8){ // gamma
    value = 1.0f*qPow(2.0f, val*2.0f/gamma_steps);
    data.output_gamma = value;
  }
  emit updateControlData(data);
}

void ControlsWidget::grayscaleToggle(bool checked) {
  data.grayscale = checked;
  emit updateControlData(data);
}

void ControlsWidget::invertToggle(bool checked) {
  data.invert = checked;
  emit updateControlData(data);
}

void ControlsWidget::loadImage() {
  QString filename = QFileDialog::getOpenFileName(this,
      tr("Open Image"), "../data",
      tr("RAW Files (*.cr2 *.dng *.CR2)"));
  qDebug() << "loading image" << filename;
  emit requestImage(filename);
}


void ControlsWidget::imageChanged() {
  // reset();
}

void ControlsWidget::reset() {
  data = ControlData();

  qDebug() << "reset";

  exposure_slider->setRange(-1000, 1000);
  exposure_slider->setValue(1);
  exposure_slider->setValue(0);

  bp_slider->setRange(0, 1000);
  bp_slider->setValue(1);
  bp_slider->setValue(0);

  out_gamma_slider->setRange(-gamma_steps, gamma_steps);
  out_gamma_slider->setValue(1);
  out_gamma_slider->setValue(0);
  exposure_slider->setRange(-1000, 1000);
  exposure_slider->setValue(1);
  exposure_slider->setValue(0);

  bp_slider->setRange(-100, 1000);
  bp_slider->setValue(1);
  bp_slider->setValue(0);

  out_gamma_slider->setRange(-gamma_steps, gamma_steps);
  out_gamma_slider->setValue(1);
  out_gamma_slider->setValue(0);

  for(int idx = 0; idx < 3; ++idx) {
    QSlider *s = wp_sliders[idx];
    s->setRange(1, wp_steps);
    s->setSingleStep(1);
    s->setValue(1);
    s->setValue(wp_steps);

    s = gamma_sliders[idx];
    s->setRange(0, gamma_steps);
    s->setSingleStep(1);
    s->setValue(1);
    s->setValue(0);
  }

  grayscale_button->setChecked(false);
  invert_button->setChecked(true);

  emit updateControlData(data);
}

void ControlsWidget::setPresets() {
  // TODO: presets should be elsewhere: e.g. .json resources
  presets_list->addItem(tr("custom"), 0);
  presets_list->addItem(tr("Ektar 100"), 1);
  presets_list->addItem(tr("Portra 160"), 2);
  presets_list->addItem(tr("Portra 400"), 3);

  presets.append(ControlData());

  ControlData ektar;
  ektar.gamma[0] = 1.7966454;
  ektar.gamma[1] = 1.75318663;
  ektar.gamma[2] = 1.52816018;
  presets.append(ektar);

  ControlData portra160;
  portra160.gamma[0] = 1.90471932;
  portra160.gamma[1] = 1.86499212;
  portra160.gamma[2] = 1.6708782;
  presets.append(portra160);

  ControlData portra400;
  portra400.gamma[0] = 1.84946131;
  portra400.gamma[1] = 1.81149687;
  portra400.gamma[2] = 1.61283489;
  presets.append(portra400);

  QObject::connect(
      presets_list, 
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), 
      this,
      &ControlsWidget::selectPreset);
}

void ControlsWidget::selectPreset(int index) {
  data = presets[index];
  emit updateControlData(data);
}
