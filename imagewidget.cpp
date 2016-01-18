#include "imagewidget.h"
#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QString>
#include <QFileDialog>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    _widthEdit->setMinimum(1);
    _heightEdit->setMinimum(1);
    _quanEdit->setMinimum(1);
    _quanEdit->setMaximum(255);
    _threEdit->setMinimum(-1);
    _threEdit->setMaximum(256);
    _threEdit->setValue(-1);
    connect(_showBtn, &QPushButton::clicked, this, &ImageWidget::showImage);
    _imageBox->setAlignment(Qt::AlignCenter);
    //_imageBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _width = _height = _quanNum = _threNum = 0;
}

void ImageWidget::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("打开图片"), "./", tr("jpg File(*.jpg *.jpeg)\n"
                                                                       "png File(*.png)\n"
                                                                       "bmp File(*.bmp)"));
    openImage(filename);
}

void ImageWidget::openImage(QString filename)
{
    if(!filename.isEmpty()){
        _imageSrc.load(filename);
        if(!_imageSrc.isNull()){
            _widthEdit->setMaximum(_imageSrc.width());
            _widthEdit->setValue(_imageSrc.width());
            _heightEdit->setMaximum(_imageSrc.height());
            _heightEdit->setValue(_imageSrc.height());
            _quanEdit->setValue(255);
            _threEdit->setValue(-1);
            _image = _imageSrc;
            _width = _imageSrc.width();
            _height = _imageSrc.height();
            _quanNum = 255;
            _threNum = -1;
            showImage();
            emit(imageChanged(_image));
        }
    }
}

void ImageWidget::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"), "./", tr("jpg File(*.jpg *.jpeg)\n"
                                                                               "png File(*.png)\n"
                                                                               "bmp File(*.bmp)"));
    saveImage(filename);
}

void ImageWidget::saveImage(QString filename)
{
    if(!filename.isEmpty()){
        if(!_image.isNull())
            _image.save(filename);
    }
}

QImage& ImageWidget::getModifImage()
{
    return _image;
}

void ImageWidget::returnModifImage()
{
    emit(imageChanged(_image));
    if(!_image.isNull())
        _imageBox->setPixmap(QPixmap::fromImage(_image));
}

void ImageWidget::linearVary(double a, double b)
{
    if(_image.isNull())
        return;
    for(int i=0; i<_image.width(); ++i){
        for(int j=0; j<_image.height(); ++j){
            QRgb rgb = _image.pixel(i, j);
            int gray = qGray(rgb) * a + b;
            gray = gray>255 ? 255 : (gray<0 ? 0 : gray);
            //                r = qRed(rgb) * a + b;
            //                g = qGreen(rgb) * a + b;
            //                b = qBlue(rgb) * a + b;
            _image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(rgb)));
        }
    }
    returnModifImage();
}

void ImageWidget::showImage()
{
    if(_image.isNull())
        return;
    if(_width!=_widthEdit->value() || _height!=_heightEdit->value() || _quanNum!=_quanEdit->value() || _threNum!=_threEdit->value()){
        _image = _image.scaled(_widthEdit->value(), _heightEdit->value());
        double dx = static_cast<double>(_imageSrc.width()) / _widthEdit->value(), dy = static_cast<double>(_imageSrc.height()) / _heightEdit->value();
        for(int i=0; i<_image.width(); ++i){
            for(int j=0; j<_image.height(); ++j){
                _image.setPixel(i, j, _imageSrc.pixel(i*dx, j*dy));
            }
        }
        _image = _image.convertToFormat(QImage::Format_ARGB32);
        if(_quanEdit->value()!=255 || _threEdit->value()!=-1){
            double delta = 256.0 / _quanEdit->value();
            for(int i=0; i<_image.width(); ++i){
                for(int j=0; j<_image.height(); ++j){
                    QRgb rgb = _image.pixel(i, j);
                    int gray = qGray(rgb) / delta;
                    gray = (gray>=_quanEdit->value() ? _quanEdit->value() : gray) * delta;
                    gray = (gray + delta / 2) > 255 ? 255 : (gray + delta / 2);
                    //                r = r / delta * delta;
                    //                g = g / delta * delta;
                    //                b = b / delta * delta;
                    if(_threEdit->value()!=-1){
                        if(qGray(rgb)<_threEdit->value())
                            _image.setPixel(i, j, qRgba(0, 0, 0, qAlpha(rgb)));
                        else
                            _image.setPixel(i, j, qRgba(255, 255, 255, qAlpha(rgb)));
                        continue;
                    }
                    _image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(rgb)));
                }
            }
        }
        _width = _widthEdit->value();
        _height = _heightEdit->value();
        _quanNum = _quanEdit->value();
        _threNum = _threEdit->value();
        emit(imageChanged(_image));
    }
    _imageBox->setPixmap(QPixmap::fromImage(_image));
}

void ImageWidget::resetImage()
{
    if(_imageSrc.isNull())
        return;
    _image = _imageSrc;
    _widthEdit->setValue(_imageSrc.width());
    _heightEdit->setValue(_imageSrc.height());
    _quanEdit->setValue(255);
    _threEdit->setValue(-1);
    showImage();
}

const QImage ImageWidget::getImage() const
{
    return _image;
}

QRgb ImageWidget::getPixel(int x, int y)
{
    if(x<0 || x>=_image.width() ||
            y<0 || y>=_image.height())
        return qRgba(0, 0, 0, 0);
    return _image.pixel(x, y);
}
