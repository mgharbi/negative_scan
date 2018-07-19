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
  void setGamma(int idx, float value);
  void loadImage();

private:
  ControlData data;

  // UI
  QLabel *wb_header_label;
  QSlider *wb_sliders[3];
  QLabel *wb_labels[3];

  QLabel *gamma_header_label;
  QSlider *gamma_sliders[3];
  QLabel *gamma_labels[3];
  QPushButton *load_button;
};
