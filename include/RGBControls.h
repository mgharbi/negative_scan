#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QList>

#include "ControlData.h"

class RGBControls : public QWidget
{
  Q_OBJECT

public:
  RGBControls(float min_, float max_, int steps, bool isLog, QString header,
              QWidget *parent = nullptr);
  void setValue(int idx, float f);
  virtual ~RGBControls ();

signals:
  void valueUpdated(int idx, float val);

private:
  float min_, max_;
  int steps;
  bool isLog;
  QSlider *sliders[3];
  QLabel *labels[3];

  void sliderChanged(int idx, int step);
  int valueToStep(float value);
  float stepToValue(int step);
};
