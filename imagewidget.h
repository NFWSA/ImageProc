#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "ui_imagewidget.h"

class ImageWidget : public QWidget, public Ui_ImageWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = 0);
    const QImage getImage() const;
    QRgb getPixel(int x, int y);
    void openImage(QString filename = "");
    void saveImage(QString filename = "");
    QImage& getModifImage();
    void returnModifImage();
    void linearVary(double a, double b);

signals:
    void imageChanged(const QImage &img);

public slots:
    void openFile();
    void saveFile();
    void showImage();
    void resetImage();

private:
    QImage _imageSrc, _image;
    int _width, _height, _quanNum, _threNum;
};

#endif // IMAGEWIDGET_H
