#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>

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

private:
  QSlider *gamma_sliders[3];
  QLabel *gamma_labels[3];
};
