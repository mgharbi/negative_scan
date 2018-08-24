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
  setupInterface();
  // setPresets();
  connectControls();
  reset();
}

ControlsWidget::~ControlsWidget() {
}

void ControlsWidget::setupInterface() {
  QVBoxLayout *layout = new QVBoxLayout(this);

  // QLabel *presets_label = new QLabel("Presets", this);
  // layout->addWidget(presets_label);
  // presets_list = new QComboBox(this);
  // layout->addWidget(presets_list);

  grayscale_button = new QCheckBox("b/w", this);
  layout->addWidget(grayscale_button);

  invert_button = new QCheckBox("invert", this);
  layout->addWidget(invert_button);

  wp = new RGBControls(0.0, 50.0, 1000, false, "white point");
  layout->addWidget(wp);

  bp = new RGBControls(0.0, 10.0, 1000, false, "black point");
  layout->addWidget(bp);

  gamma = new RGBControls(0.0, 3.0, 1000, false, "gamma point");
  layout->addWidget(gamma);

  QWidget *positive_widget = new QWidget();
  QVBoxLayout *positive_layout = new QVBoxLayout(positive_widget);
  QLabel *positive_header = new QLabel("Exposure, out gamma", this);
  positive_layout->addWidget(positive_header);
  layout->addWidget(positive_widget);

  exposure_slider = new QSlider(Qt::Horizontal, this);
  out_gamma_slider = new QSlider(Qt::Horizontal, this);
  positive_layout->addWidget(exposure_slider);
  positive_layout->addWidget(out_gamma_slider);

  load_button = new QPushButton("load", this);
  load_button->setToolTip(tr("load a raw file for processing"));
  layout->addWidget(load_button);

  save_button = new QPushButton("save", this);
  save_button->setToolTip(tr("save processed file"));
  layout->addWidget(save_button);

  reset_button = new QPushButton("reset", this);
  reset_button->setToolTip(tr("reset settings"));
  layout->addWidget(reset_button);
}

void ControlsWidget::connectControls() {
  QObject::connect(
      grayscale_button, &QCheckBox::clicked, 
      this, &ControlsWidget::grayscaleToggle);
  QObject::connect(
      invert_button, &QCheckBox::clicked, 
      this, &ControlsWidget::invertToggle);
  QObject::connect(
      wp, &RGBControls::valueUpdated, 
      this, &ControlsWidget::whitePointChanged);
  QObject::connect(
      bp, &RGBControls::valueUpdated, 
      this, &ControlsWidget::blackPointChanged);
  QObject::connect(
      gamma, &RGBControls::valueUpdated, 
      this, &ControlsWidget::gammaChanged);
  QObject::connect(
      exposure_slider, &QSlider::valueChanged, 
      this, [=](int val) { this->sliderChanged(6, val); });
  QObject::connect(
      out_gamma_slider, &QSlider::valueChanged, 
      this, [=](int val) { this->sliderChanged(8, val); });
  QObject::connect(
      load_button, &QPushButton::clicked, 
      this, &ControlsWidget::loadImage);
  QObject::connect(
      save_button, &QPushButton::clicked, 
      this, [=] () { ControlsWidget::requestSave(data); } );

  QObject::connect(
      reset_button, &QPushButton::clicked, 
      this, &ControlsWidget::reset);
}

void ControlsWidget::whitePointChanged(int idx, float val) {
  data.wp[idx] = val;
  emit updateControlData(data);
}

void ControlsWidget::blackPointChanged(int idx, float val) {
  data.bp[idx] = val;
  emit updateControlData(data);
}

void ControlsWidget::gammaChanged(int idx, float val) {
  data.gamma[idx] = val;
  emit updateControlData(data);
}

void ControlsWidget::sliderChanged(int idx, int val) {
  QString s;
  float value = 0.0f;

  if (idx == 6){ // exposure
    value = qPow(2.0f, val*8.0f/1000);
    data.exposure = value;
  } else if (idx == 8){ // gamma
    value = 2.2f*qPow(2.0f, val*2.0f/1000);
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

  for (int i = 0; i < 3; ++i) {
    wp->setValue(i, 0.0);
    wp->setValue(i, 50.0);

    bp->setValue(i, 1.0);
    bp->setValue(i, 0.0);

    gamma->setValue(i, 0.0);
    gamma->setValue(i, 1.0);
    
  }

  exposure_slider->setRange(-1000, 1000);
  exposure_slider->setValue(1);
  exposure_slider->setValue(0);

  out_gamma_slider->setRange(-1000, 1000);
  out_gamma_slider->setValue(1);
  out_gamma_slider->setValue(0);

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
