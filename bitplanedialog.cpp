#include "bitplanedialog.h"
#include <QImage>
#include <QColor>
#include <QDebug>

BitPlaneDialog::BitPlaneDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    _img1->setAlignment(Qt::AlignCenter);
    _img1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img2->setAlignment(Qt::AlignCenter);
    _img2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img3->setAlignment(Qt::AlignCenter);
    _img3->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img4->setAlignment(Qt::AlignCenter);
    _img4->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img5->setAlignment(Qt::AlignCenter);
    _img5->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img6->setAlignment(Qt::AlignCenter);
    _img6->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img7->setAlignment(Qt::AlignCenter);
    _img7->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _img8->setAlignment(Qt::AlignCenter);
    _img8->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    _img1->setScaledContents(true);
    _img2->setScaledContents(true);
    _img3->setScaledContents(true);
    _img4->setScaledContents(true);
    _img5->setScaledContents(true);
    _img6->setScaledContents(true);
    _img7->setScaledContents(true);
    _img8->setScaledContents(true);
    connect(_okBtn, &QPushButton::clicked, this, &BitPlaneDialog::okPush);
}

void BitPlaneDialog::showPlane(const QImage &img)
{
    QImage dst[8];
    for(int bit=0; bit<8; ++bit){
        dst[bit] = img;
        for(int i=0; i<img.width(); ++i){
            for(int j=0; j<img.height(); ++j){
                QRgb rgb = img.pixel(i, j);
                if(qGray(rgb)&(1U<<bit)){
                    dst[bit].setPixel(i, j, qRgba(255, 255, 255, qAlpha(rgb)));
                }
                else{
                    dst[bit].setPixel(i, j, qRgba(0, 0, 0, qAlpha(rgb)));
                }
            }
        }
    }
    _img1->setPixmap(QPixmap::fromImage(dst[0]));
    _img2->setPixmap(QPixmap::fromImage(dst[1]));
    _img3->setPixmap(QPixmap::fromImage(dst[2]));
    _img4->setPixmap(QPixmap::fromImage(dst[3]));
    _img5->setPixmap(QPixmap::fromImage(dst[4]));
    _img6->setPixmap(QPixmap::fromImage(dst[5]));
    _img7->setPixmap(QPixmap::fromImage(dst[6]));
    _img8->setPixmap(QPixmap::fromImage(dst[7]));
}

void BitPlaneDialog::okPush()
{
    this->close();
}
