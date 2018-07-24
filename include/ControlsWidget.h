#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QCheckBox>

#include "ControlData.h"

class ControlsWidget : public QWidget
{

  Q_OBJECT

public:
  ControlsWidget(QWidget *parent = nullptr);
  virtual ~ControlsWidget ();

public:
  void sliderChanged(int idx, int value);
  void grayscaleToggle(bool checked);
  void invertToggle(bool checked);
  void loadImage();

public slots:
  void imageChanged();

signals:
  void updateControlData(ControlData data);
  void requestImage(QString filename);
  void requestSave();

private:
  void reset();
  ControlData data;

  int wp_steps;
  int gamma_steps;

  // UI
  QCheckBox *grayscale_button;
  QCheckBox *invert_button;

  QLabel *wp_header_label;
  QSlider *wp_sliders[3];
  QLabel *wp_labels[3];

  QLabel *gamma_header_label;
  QSlider *gamma_sliders[3];
  QLabel *gamma_labels[3];

  QSlider *exposure_slider;
  QSlider *out_gamma_slider;
  QSlider *bp_slider;

  QPushButton *load_button;
  QPushButton *save_button;
  QPushButton *reset_button;

};
