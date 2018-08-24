#include <QDesktopWidget>
#include <QHBoxLayout>

#include "MainWindow.h"


MainWindow::MainWindow() {
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

  raw_processor = new RawProcessor();

  // preview = new PreviewWidget(this);
  // setCentralWidget(preview);
  //
  // QDockWidget *dock = new QDockWidget("controls", this);
  // controls = new ControlsWidget(dock);
  // dock->setWidget(controls);
  // addDockWidget(Qt::RightDockWidgetArea, dock);
  //
  // QObject::connect(controls, 
  //     &ControlsWidget::updateControlData,
  //     preview,
  //     &PreviewWidget::controlDataChanged);
  //
  // QObject::connect(raw_processor, 
  //     &RawProcessor::updateImage,
  //     preview,
  //     &PreviewWidget::imageChanged);
  //
  // QObject::connect(raw_processor, 
  //     &RawProcessor::updateImage,
  //     controls,
  //     &ControlsWidget::imageChanged);
  //
  // QObject::connect(
  //     controls,
  //     &ControlsWidget::requestImage,
  //     raw_processor, 
  //     &RawProcessor::load);
  //
  // QObject::connect(
  //     controls,
  //     &ControlsWidget::requestSave,
  //     raw_processor, 
  //     &RawProcessor::save);

  // QWidget *histograms_widget = new QWidget();
  // QHBoxLayout *layout = new QHBoxLayout(histograms_widget);
  // histograms[0] = new HistogramWidget(this);
  // histograms[1] = new HistogramWidget(this);
  // histograms[2] = new HistogramWidget(this);
  // layout->addWidget(histograms[0]);
  // layout->addWidget(histograms[1]);
  // layout->addWidget(histograms[2]);
  // QDockWidget *tools = new QDockWidget(tr("tools"), this);
  // tools->setWidget(histograms_widget);
  // addDockWidget(Qt::TopDockWidgetArea, tools);

  setWindowTitle("Negative Scan");
  
  // TODO: autoload an image for debug
  // emit controls->requestImage("/Users/mgharbi/projects/negascan/data/shuglo.CR2");
  raw_processor->load("/Users/mgharbi/projects/negascan/data/shuglo.CR2");
}

MainWindow::~MainWindow() {
  if (!raw_processor) {
    delete raw_processor;
    raw_processor = nullptr;
  }
}
