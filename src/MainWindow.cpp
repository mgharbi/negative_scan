#include <QDesktopWidget>

#include "MainWindow.h"


MainWindow::MainWindow() {
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

  // raw_processor = new RawProcessor();

  preview = new PreviewWidget();
  setCentralWidget(preview);
  
  // histogram = new HistogramWidget();
  // setCentralWidget(histogram);

  // tools = new QDockWidget(tr("tools"), this);
  // tools->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  // tools->setWidget(histogram);

  // addDockWidget(Qt::RightDockWidgetArea, tools);
  setWindowTitle("Negative Scan");
}

MainWindow::~MainWindow() {
  if (!raw_processor) {
    delete raw_processor;
    raw_processor = nullptr;
  }
}
