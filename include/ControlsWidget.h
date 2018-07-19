#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include "PreviewWidget.h"

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
  float encodeSliderValue(int val);
  QSlider *gamma_sliders[3];
  QLabel *gamma_labels[3];
  QPushButton *load_button;
};
