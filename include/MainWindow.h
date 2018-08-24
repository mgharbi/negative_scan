#pragma once

#include <QMainWindow>
#include <QDockWidget>

#include "PreviewWidget.h"
#include "HistogramWidget.h"
#include "ControlsWidget.h"
#include "RawProcessor.h"

class MainWindow : public QMainWindow
{
public:
  MainWindow();
  virtual ~MainWindow ();


private:
  PreviewWidget *preview;
  HistogramWidget *histograms[3];
  // QDockWidget *tools;
  ControlsWidget* controls;
  RawProcessor *raw_processor;
};
