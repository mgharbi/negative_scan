#include <QMainWindow>
#include <QDockWidget>
#include "PreviewWidget.h"
#include "HistogramWidget.h"

class MainWindow : public QMainWindow
{
public:
  MainWindow();
  virtual ~MainWindow ();

private:
  PreviewWidget *preview;
  HistogramWidget *histogram;
  QDockWidget *tools;
};
