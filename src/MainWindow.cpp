#include <QDesktopWidget>

#include "MainWindow.h"


MainWindow::MainWindow() {
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

  raw_processor = new RawProcessor();

  preview = new PreviewWidget(this);
  setCentralWidget(preview);

  QDockWidget *dock = new QDockWidget("controls", this);
  controls = new ControlsWidget(dock);
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

  QObject::connect(raw_processor, 
      &RawProcessor::updateImage,
      controls,
      &ControlsWidget::imageChanged);

  QObject::connect(
      controls,
      &ControlsWidget::requestImage,
      raw_processor, 
      &RawProcessor::load);

  QObject::connect(
      controls,
      &ControlsWidget::requestSave,
      raw_processor, 
      &RawProcessor::save);

  // histogram = new HistogramWidget();
  // QDockWidget *tools = new QDockWidget(tr("tools"), this);
  // tools->setWidget(histogram);
  // addDockWidget(Qt::LeftDockWidgetArea, tools);

  setWindowTitle("Negative Scan");
}

MainWindow::~MainWindow() {
  if (!raw_processor) {
    delete raw_processor;
    raw_processor = nullptr;
  }
}
