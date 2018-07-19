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
      &ControlsWidget::setGamma,
      preview,
      &PreviewWidget::gammaChanged);

  QObject::connect(raw_processor, 
      &RawProcessor::updateImage,
      preview,
      &PreviewWidget::imageChanged);

  raw_processor->load();
  
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
