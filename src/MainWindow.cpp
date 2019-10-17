#include <QColor>
#include <QDesktopWidget>
#include <QHBoxLayout>

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

  QObject::connect(
      controls, 
      &ControlsWidget::updateControlData,
      preview,
      &PreviewWidget::controlDataChanged);

  QObject::connect(
      raw_processor, 
      &RawProcessor::updateImage,
      preview,
      &PreviewWidget::imageChanged);

  QObject::connect(
      raw_processor, 
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

  QObject::connect(
      preview,
      &PreviewWidget::pickWhitePoint,
      raw_processor, 
      &RawProcessor::whitePointPicked);

  QWidget *histograms_widget = new QWidget();
  QHBoxLayout *layout = new QHBoxLayout(histograms_widget);
  histograms[0] = new HistogramWidget(0, QColor(255, 60, 60), this);
  histograms[1] = new HistogramWidget(1, QColor(60, 255, 60), this);
  histograms[2] = new HistogramWidget(2, QColor(60, 60, 255), this);
  layout->addWidget(histograms[0]);
  layout->addWidget(histograms[1]);
  layout->addWidget(histograms[2]);
  QDockWidget *tools = new QDockWidget(tr("tools"), this);
  tools->setWidget(histograms_widget);
  addDockWidget(Qt::TopDockWidgetArea, tools);

  QObject::connect(
      raw_processor,
      &RawProcessor::setWhitePoint,
      controls, 
      &ControlsWidget::whitePointChanged);

  for (int i = 0; i < 3; ++i) {
    QObject::connect(
        raw_processor,
        &RawProcessor::updateHistogram,
        histograms[i], 
        &HistogramWidget::setData);
    QObject::connect(
        raw_processor,
        &RawProcessor::setWhitePoint,
        histograms[i], 
        &HistogramWidget::setWhitePoint);
    QObject::connect(
        histograms[i], 
        &HistogramWidget::whitePointChanged,
        controls,
        &ControlsWidget::whitePointChanged);
  }

  setWindowTitle("Negative Scan");
  
}

void MainWindow::load_image(QString path) {
  // TODO: autoload an image for debug
  emit controls->requestImage(path);
  // raw_processor->load(path);
}

MainWindow::~MainWindow() {
  if (!raw_processor) {
    delete raw_processor;
    raw_processor = nullptr;
  }
}
