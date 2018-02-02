#include <QDesktopWidget>

#include "MainWindow.h"


MainWindow::MainWindow() {
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

  preview = new PreviewWidget();
  histogram = new HistogramWidget();
  setCentralWidget(preview);

  tools = new QDockWidget(tr("tools"), this);
  tools->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  tools->setWidget(histogram);

  addDockWidget(Qt::RightDockWidgetArea, tools);
  setWindowTitle("Negative Scan");
}

MainWindow::~MainWindow() {
}
