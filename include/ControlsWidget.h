#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QList>

#include "ControlData.h"
#include "RGBControls.h"

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
  void selectPreset(int index);
  void whitePointChanged(int idx, float value);
  void blackPointChanged(int idx, float value);
  void gammaChanged(int idx, float value);

signals:
  void updateControlData(ControlData data);
  void requestImage(QString filename);
  void requestSave(ControlData data);

private:
  void reset();
  void setPresets();

  void setupInterface();
  void connectControls();

  QList<ControlData> presets;
  ControlData data;

  // UI
  QComboBox *presets_list;
  QCheckBox *grayscale_button;
  QCheckBox *invert_button;

  RGBControls* wp;
  RGBControls* bp;
  RGBControls* gamma;

  QSlider *wp_sliders[3];
  QLabel *wp_labels[3];

  QSlider *gamma_sliders[3];
  QLabel *gamma_labels[3];

  QSlider *exposure_slider;
  QSlider *out_gamma_slider;

  QPushButton *load_button;
  QPushButton *save_button;
  QPushButton *reset_button;

};
