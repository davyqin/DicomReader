#include "DicomReaderDlg.h"
#include "glwidget.h"

#include <QFileDialog>

DicomReaderDlg::DicomReaderDlg(QWidget *parent)
  : QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
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
    connect(ui.pixelcurveCheckBox, SIGNAL(clicked(bool)), ui.glWidget, SLOT(showPixelCurve(bool)));
    connect(ui.pixelTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(pixelTypeChanged(const int)));
}

DicomReaderDlg::~DicomReaderDlg() {}

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
    ui.zoomSlider->setValue(10);
  }
}

void DicomReaderDlg::setZoomFactor(const int value) {
  ui.zoomValue->setText(QString::number(value));
  ui.glWidget->setZoomFactor(value);
}

void DicomReaderDlg::setWindowLevel() {
  ui.windowValue->setText(QString::number(ui.windowSlider->value()));
  ui.levelValue->setText(QString::number(ui.levelSlider->value()));
  ui.glWidget->setWindowLevel(ui.windowSlider->value(), ui.levelSlider->value());
}

void DicomReaderDlg::pixelTypeChanged(const int type) {
  if (type == 0) {
    ui.windowSlider->setMaximum(255);
    ui.levelSlider->setMaximum(255);
    ui.windowSlider->setValue(255);
    ui.levelSlider->setValue(0);
    ui.glWidget->setPixelType(GLWidget::BytePixel);
  }
  else {
    ui.windowSlider->setMaximum(65535);
    ui.levelSlider->setMaximum(65535);
    ui.windowSlider->setValue(65535);
    ui.levelSlider->setValue(0);
    ui.glWidget->setPixelType(GLWidget::ShortPixel);
  }
}
