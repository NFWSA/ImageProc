#include "grayinfowidget.h"
#include <cmath>
#include <QDebug>

const int grayHeight = 500;

GrayInfoWidget::GrayInfoWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    _grayShowLab->setAlignment(Qt::AlignCenter);
    _grayShowLab->setScaledContents(true);
    _grayGraLab->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _grayGraLab->setScaledContents(true);
    QImage grayBar(256, 10, QImage::Format_ARGB32);
    _grayInfo = new QImage(256, grayHeight, QImage::Format_ARGB32);
    _grayCnt = new int[256];
    for(int j=0; j<10; ++j)
        for(int i=0; i<256; ++i)
            grayBar.setPixel(i, j, qRgb(i, i, i));
    _grayShowLab->setPixmap(QPixmap::fromImage(grayBar));
    _grayGraLab->setPixmap(QPixmap::fromImage(*_grayInfo));
}

GrayInfoWidget::~GrayInfoWidget()
{
    delete[] _grayCnt;
    delete _grayInfo;
}

void GrayInfoWidget::setGray(const int gray, const int num, const int max)
{
    int i = 0, cnt = grayHeight - num * grayHeight / max;
    for(; i<cnt; ++i)
        _grayInfo->setPixel(gray, i, qRgba(0, 0, 0, 0));
    for(; i<grayHeight; ++i)
        _grayInfo->setPixel(gray, i, qRgb(0, 0, 0));
}

void GrayInfoWidget::setImage(const QImage &img)
{
    _cnt = img.width() * img.height();
    _grayAvg = _cntMax = 0;
    for(int i=0; i<256; ++i)
        _grayCnt[i] = 0;
    for(int i=0; i<img.width(); ++i)
        for(int j=0; j<img.height(); ++j){
            ++_grayCnt[qGray(img.pixel(i, j))];
        }
    int midCnt = _cnt / 2;
    _grayMid = -1;
    bool cnt2Flag = _cnt%2==0 ? true : false, flag = true;
    for(int i=0; i<256; ++i){
        _grayAvg += _grayCnt[i] * i;
        _cntMax = _grayCnt[i]>_cntMax ? _grayCnt[i] : _cntMax;
        midCnt -= _grayCnt[i];
        if(flag && midCnt<0){
            if(cnt2Flag){
                if(_grayMid!=-1){
                    _grayMid += i;
                    _grayMid /= 2;
                }
                else
                    _grayMid = i;
            }
            else{
                _grayMid = i;
            }
            flag = false;
        }
        if(flag && midCnt==0){
            if(cnt2Flag){
                _grayMid = i;
            }
        }
    }
    _grayAvg /= _cnt;
    double grayDelta = 0;
    for(int i=0; i<256; ++i){
        grayDelta += (static_cast<double>(i)-_grayAvg) * (static_cast<double>(i)-_grayAvg) * _grayCnt[i] / _cnt;
        setGray(i, _grayCnt[i], _cntMax);
    }
    _graySDelta = std::sqrt(grayDelta);
    showInfo();
}

void GrayInfoWidget::showInfo()
{
    _pixelCntLab->setText(QString("%1").arg(_cnt));
    _grayLab->setText(QString("%1").arg(_grayAvg));
    _midLab->setText(QString("%1").arg(_grayMid));
    _stdLab->setText(QString("%1").arg(_graySDelta));
    _grayGraLab->setPixmap(QPixmap::fromImage(*_grayInfo));
}

int GrayInfoWidget::getGrayCnt(const int gray) const
{
    if(gray<0 || gray>255)
        return 0;
    return _grayCnt[gray];
}

int GrayInfoWidget::getGrayAvg() const
{
    return _grayAvg;
}

int GrayInfoWidget::getGrayMid() const
{
    return _grayMid;
}

int GrayInfoWidget::getGraySDelta() const
{
    return _graySDelta;
}

int GrayInfoWidget::getPixelCnt() const
{
    return _cnt;
}
