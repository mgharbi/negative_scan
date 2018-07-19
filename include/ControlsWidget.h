#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include "ControlData.h"

class ControlsWidget : public QWidget
{

  Q_OBJECT

public:
  ControlsWidget(QWidget *parent = nullptr);
  virtual ~ControlsWidget ();

public:
  void sliderChanged(int idx, int value);

signals:
  void updateControlData(ControlData data);
  void loadImage();

private:
  ControlData data;

  int wp_steps;
  int gamma_steps;

  // UI
  QLabel *wp_header_label;
  QSlider *wp_sliders[3];
  QLabel *wp_labels[3];

  QLabel *gamma_header_label;
  QSlider *gamma_sliders[3];
  QLabel *gamma_labels[3];
  QPushButton *load_button;

  QSlider *exposure_slider;
  QSlider *out_gamma_slider;
  QSlider *bp_slider;
};
