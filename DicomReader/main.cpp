#include <QApplication>
#include <QDesktopWidget>

#include "DicomReaderDlg.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  DicomReaderDlg dicomReader;

  dicomReader.resize(dicomReader.sizeHint());
  const int desktopArea = QApplication::desktop()->width() *  QApplication::desktop()->height();
  const int widgetArea = dicomReader.width() * dicomReader.height();
  if (((float)widgetArea / (float)desktopArea) < 0.75f)
    dicomReader.show();
  else
    dicomReader.showMaximized();

  return app.exec();
}
