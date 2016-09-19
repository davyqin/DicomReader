#ifndef DICOMREADERDLG_H
#define DICOMREADERDLG_H

#include <QDialog>
#include "ui_dicomreader.h"

class DicomReaderDlg : public QDialog
{
    Q_OBJECT

public:
    DicomReaderDlg(QWidget *parent = 0);
    ~DicomReaderDlg();

private slots:
    void onBrowseFolder();
    void onLoadImage();
    void setZoomFactor(const int value);
    void setWindowLevel();
    void pixelTypeChanged(const int type);

private:
    Ui::DicomReaderDlg ui;
    QString fileName;
};

#endif // DICOMREADERDLG_H
