#include <QDesktopWidget>

#include "MainWindow.h"


MainWindow::MainWindow() {
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

  raw_processor = new RawProcessor();

  preview = new PreviewWidget(this);
  setCentralWidget(preview);

  QDockWidget *dock = new QDockWidget("controls", this);
  controls = new ControlsWidget(dock);
  // controls->connect_controls(preview);
  dock->setWidget(controls);
  addDockWidget(Qt::RightDockWidgetArea, dock);

  QObject::connect(controls, 
      &ControlsWidget::updateControlData,
      preview,
      &PreviewWidget::controlDataChanged);

  QObject::connect(raw_processor, 
      &RawProcessor::updateImage,
      preview,
      &PreviewWidget::imageChanged);

  QObject::connect(
      controls,
      &ControlsWidget::loadImage,
      raw_processor, 
      &RawProcessor::load);

  // histogram = new HistogramWidget();
  // setCentralWidget(histogram);

  // tools = new QDockWidget(tr("tools"), this);
  // tools->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  // tools->setWidget(histogram);

  setWindowTitle("Negative Scan");
}

MainWindow::~MainWindow() {
  if (!raw_processor) {
    delete raw_processor;
    raw_processor = nullptr;
  }
}
