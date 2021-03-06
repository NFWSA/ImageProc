#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include "ui_imageproc.h"
#include "imagewidget.h"
#include "grayinfowidget.h"

class ImageProc : public QMainWindow, public Ui_ImageProc
{
    Q_OBJECT
public:
    explicit ImageProc(QWidget *parent = 0);

signals:

public slots:
    void showBitPlane();

    void oppoImage();
    void highImage();
    void darkImage();

    void equaImage();
    void slicImage();
    void grayExImage();

    void spaceAlter();
    void doSpaceAlter(double w, double h, int angle, int x, int y, bool type);

    void avgSmooth();
    void boxSmooth();
    void gaussSmooth();

    void sobelXSharp();
    void sobelYSharp();
    void laplaceSharp();
    void lapExSharp();

    void convolutionTemplate();
    void doConvolutionTemplate(std::vector<std::vector<int> > mart, int x, int y, double k);

    void bmp2txt();

private:
    ImageWidget *_imgWidget;
    GrayInfoWidget *_grayWidget;
};

#endif // IMAGEPROC_H
