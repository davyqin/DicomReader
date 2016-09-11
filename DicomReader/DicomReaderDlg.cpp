#include "DicomReaderDlg.h"

#include <QFileDialog>

DicomReaderDlg::DicomReaderDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ui.zoomValue->setText(QString::number(ui.zoomSlider->value()));
    ui.windowValue->setText(QString::number(ui.windowSlider->value()));
    ui.levelValue->setText(QString::number(ui.levelSlider->value()));

    connect(ui.browseButton, SIGNAL(clicked()), SLOT(onBrowseFolder()));
    connect(ui.loadButton, SIGNAL(clicked()), SLOT(onLoadImage()));
    connect(ui.zoomSlider, SIGNAL(valueChanged(int)), SLOT(setZoomFactor(const int)));
    connect(ui.windowSlider, SIGNAL(valueChanged(int)), SLOT(setWindowLevel()));
    connect(ui.levelSlider, SIGNAL(valueChanged(int)), SLOT(setWindowLevel()));
}

DicomReaderDlg::~DicomReaderDlg()
{

}

void DicomReaderDlg::onBrowseFolder() {
  QFileDialog browser(this, tr("Select Image Stack Directory"));
  browser.setFileMode(QFileDialog::ExistingFile);
  browser.setAcceptMode(QFileDialog::AcceptOpen);
  if (browser.exec() == QFileDialog::Accepted) {
    fileName = browser.selectedFiles().at(0);
    ui.fileEdit->setText(fileName);
  }
}

void DicomReaderDlg::onLoadImage() {
  if (!fileName.isEmpty()) {
    ui.glWidget->loadFile(fileName);
  }
}


void DicomReaderDlg::setZoomFactor(const int value)
{
  ui.zoomValue->setText(QString::number(value));
  ui.glWidget->setZoomFactor(value);
}

void DicomReaderDlg::setWindowLevel() {
  ui.windowValue->setText(QString::number(ui.windowSlider->value()));
  ui.levelValue->setText(QString::number(ui.levelSlider->value()));
  ui.glWidget->setWindowLevel(ui.windowSlider->value(), ui.levelSlider->value());
}