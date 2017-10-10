#include "imageproc.h"
#include "imagewidget.h"
#include "grayinfowidget.h"
#include "bitplanedialog.h"
#include "spacealter.h"
#include "convolutiondialog.h"
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <fstream>
#include <cmath>

const double PI = 3.141592653;

ImageProc::ImageProc(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    _imgWidget = new ImageWidget;
    _grayWidget = new GrayInfoWidget;
    auto *layout = new QHBoxLayout;
    layout->addWidget(_imgWidget);
    layout->addWidget(_grayWidget);
    auto *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    connect(_imgWidget, &ImageWidget::imageChanged, _grayWidget, &GrayInfoWidget::setImage);

    connect(action_Open, &QAction::triggered, _imgWidget, &ImageWidget::openFile);
    connect(action_Save, &QAction::triggered, _imgWidget, &ImageWidget::saveFile);
    connect(action_Reset, &QAction::triggered, _imgWidget, &ImageWidget::resetImage);
    connect(action_Quit, &QAction::triggered, this, &QMainWindow::close);

    connect(action_Plane, &QAction::triggered, this, &ImageProc::showBitPlane);

    connect(action_Opposition, &QAction::triggered, this, &ImageProc::oppoImage);
    connect(action_Highlight, &QAction::triggered, this, &ImageProc::highImage);
    connect(action_Darken, &QAction::triggered, this, &ImageProc::darkImage);

    connect(action_Equali, &QAction::triggered, this, &ImageProc::equaImage);
    connect(action_Slicing, &QAction::triggered, this, &ImageProc::slicImage);
    connect(action_GrayEx, &QAction::triggered, this, &ImageProc::grayExImage);

    connect(action_SpaceAlter, &QAction::triggered, this, &ImageProc::spaceAlter);

    connect(action_avgSmooth, &QAction::triggered, this, &ImageProc::avgSmooth);
    connect(action_boxSmooth, &QAction::triggered, this, &ImageProc::boxSmooth);
    connect(action_gaussSmooth, &QAction::triggered, this, &ImageProc::gaussSmooth);

    connect(action_SobelXSharp, &QAction::triggered, this, &ImageProc::sobelXSharp);
    connect(action_SobelYSharp, &QAction::triggered, this, &ImageProc::sobelYSharp);
    connect(action_LaplaceSharp, &QAction::triggered, this, &ImageProc::laplaceSharp);
    connect(action_LapExSharp, &QAction::triggered, this, &ImageProc::lapExSharp);

    connect(action_Template, &QAction::triggered, this, &ImageProc::convolutionTemplate);

    connect(action_BMP2TXT, &QAction::triggered, this, &ImageProc::bmp2txt);
}

void ImageProc::showBitPlane()
{
    BitPlaneDialog *dialog = new BitPlaneDialog(this);
    dialog->showPlane(_imgWidget->getImage());
    dialog->exec();
}

void ImageProc::oppoImage()
{
    _imgWidget->linearVary(-1, 255);
}

void ImageProc::highImage()
{
    _imgWidget->linearVary(1, 10);
}

void ImageProc::darkImage()
{
    _imgWidget->linearVary(1, -10);
}

void ImageProc::equaImage()
{
    QImage &img = _imgWidget->getModifImage();
    if(img.isNull())
        return;
    int gray[256];
    gray[0] = _grayWidget->getGrayCnt(0);
    for(int i=1; i<256; ++i){
        gray[i] = _grayWidget->getGrayCnt(i);
        gray[i] += gray[i-1];
    }
    for(int i=0; i<256; ++i)
        gray[i] = gray[i] * 255 / _grayWidget->getPixelCnt();
    for(int i=0; i<img.width(); ++i)
        for(int j=0; j<img.height(); ++j){
            int graySrc = qGray(img.pixel(i, j)), alpha = qAlpha(img.pixel(i, j));
            img.setPixel(i, j, qRgba(gray[graySrc], gray[graySrc], gray[graySrc], alpha));
        }
    _imgWidget->returnModifImage();
}

void ImageProc::slicImage()
{
    QImage &img = _imgWidget->getModifImage();
    if(img.isNull())
        return;
    int g1 = QInputDialog::getInt(this, tr("输入g1"), tr("请输入g1值："), 0, 0, 255),
            g2 = QInputDialog::getInt(this, tr("输入g2"), tr("请输入g2值："), g1+1, g1+1, 256);
    for(int i=0; i<img.width(); ++i)
        for(int j=0; j<img.height(); ++j){
            int gray = qGray(img.pixel(i, j)), alpha = qAlpha(img.pixel(i, j));
            if(gray<g1 || gray>=g2)
                continue;
            img.setPixel(i, j, qRgba(255, 255, 255, alpha));
        }
    _imgWidget->returnModifImage();
}

void ImageProc::grayExImage()
{
    QImage &img = _imgWidget->getModifImage();
    if(img.isNull())
        return;
    int g1 = QInputDialog::getInt(this, tr("输入g1"), tr("请输入g1值："), 0, 0, 255),
            g2 = QInputDialog::getInt(this, tr("输入g2"), tr("请输入g2值："), g1+1, g1+1, 256);
    double k = QInputDialog::getDouble(this, tr("输入k值"), tr("请输入k值："), 0, -10, 10, 1),
            b = QInputDialog::getDouble(this, tr("输入b值"), tr("请输入b值："), 0, -10, 10, 1);
    for(int i=0; i<img.width(); ++i)
        for(int j=0; j<img.height(); ++j){
            int gray = qGray(img.pixel(i, j)), alpha = qAlpha(img.pixel(i, j));
            if(gray<g1 || gray>=g2)
                continue;
            gray = gray * k + b;
            gray = gray>255 ? 255 : (gray<0 ? 0 : gray);
            img.setPixel(i, j, qRgba(gray, gray, gray, alpha));
        }
    _imgWidget->returnModifImage();
}

void ImageProc::spaceAlter()
{
    auto *dialog = new SpaceAlter;
    connect(dialog, &SpaceAlter::doSpaceAlter, this, &ImageProc::doSpaceAlter);
    dialog->exec();
}

void ImageProc::doSpaceAlter(double w, double h, int angle, int x, int y, bool type)
{
    //type 最近邻true
    QImage &img = _imgWidget->getModifImage();
    if(img.isNull())
        return;
    if(w!=1.0 || h!=1.0){
        QImage newimg(img.width()*w, img.height()*h, QImage::Format_ARGB32);
        for(int i=0; i<newimg.width(); ++i){
            for(int j=0; j<newimg.height(); ++j){
                double di = i/w, dj = j/h;
                int dx = di, dy = dj;
                di = di - dx;
                dj = dj - dy;
                if(type){
                    dx = di<0.5 ? dx : dx+1;
                    dy = dj<0.5 ? dy : dy+1;
                    newimg.setPixel(i, j, _imgWidget->getPixel(dx, dy));
                }
                else{
                    int d00 = qGray(_imgWidget->getPixel(dx, dy)),
                            d01 = qGray(_imgWidget->getPixel(dx, dy + 1)),
                            d10 = qGray(_imgWidget->getPixel(dx + 1, dy)),
                            d11 = qGray(_imgWidget->getPixel(dx + 1, dy + 1)),
                            a00 = qAlpha(_imgWidget->getPixel(dx, dy)),
                            a01 = qAlpha(_imgWidget->getPixel(dx, dy + 1)),
                            a10 = qAlpha(_imgWidget->getPixel(dx + 1, dy)),
                            a11 = qAlpha(_imgWidget->getPixel(dx + 1, dy + 1));
                    int gray = (d10-d00)*di + (d01-d00)*dj + (d11+d00-d10-d01)*di*dj + d00, alpha = (a00+a01+a10+a11)/4;
                    gray = gray>255 ? 255 : (gray<0 ? 0 : gray);
                    alpha = alpha>255 ? 255 : (alpha<0 ? 0 : alpha);
                    newimg.setPixel(i, j, qRgba(gray, gray, gray, alpha));
                }
            }
        }
        img = newimg;
    }
    if(angle!=0){
        double dsx = (img.width() - 1) / 2.0, dsy = (img.height() - 1) / 2.0;
        double dl1 = std::sqrt(dsx * dsx + dsy * dsy), dl2 = dl1;
        double dr1 = std::asin(dsy / dl1)<0 ? -std::acos(-dsx / dl1) : std::acos(-dsx / dl1),
                dr2 = std::asin(dsy / dl2)<0 ? -std::acos(dsx / dl2) : std::acos(dsx / dl2);
        double dra = angle / 180.0 * PI;
        dr1 -= dra;
        dr2 -= dra;
        double dx1 = std::abs(dl1 * std::cos(dr1)), dy1 = std::abs(dl1 * std::sin(dr1)),
                dx2 = std::abs(dl2 * std::cos(dr2)), dy2 = std::abs(dl2 * std::sin(dr2));
        int dw = std::ceil(2 * (dx1<dx2 ? dx2 : dx1)), dh = std::ceil(2 * (dy1<dy2 ? dy2 : dy1));
        QImage newimg(dw, dh, QImage::Format_ARGB32);
        double ddx = (dw - 1) / 2.0, ddy = (dh - 1) / 2.0;
        for(int i=0; i<newimg.width(); ++i){
            for(int j=0; j<newimg.height(); ++j){
                double dl = std::sqrt((i - ddx) * (i - ddx) + (j - ddy) * (j - ddy)),
                        dr = std::asin((j - ddy) / dl)<0 ? -std::acos((i - ddx) / dl) : std::acos((i - ddx) / dl);
                dr -= dra;
                double di = dsx + dl * std::cos(dr), dj = dsy + dl * std::sin(dr);
                int dx = di, dy = dj;
                di = di - dx;
                dj = dj - dy;
                if(type){
                    dx = di<0.5 ? dx : dx+1;
                    dy = dj<0.5 ? dy : dy+1;
                    newimg.setPixel(i, j, _imgWidget->getPixel(dx, dy));
                }
                else{
                    int d00 = qGray(_imgWidget->getPixel(dx, dy)),
                            d01 = qGray(_imgWidget->getPixel(dx, dy + 1)),
                            d10 = qGray(_imgWidget->getPixel(dx + 1, dy)),
                            d11 = qGray(_imgWidget->getPixel(dx + 1, dy + 1)),
                            a00 = qAlpha(_imgWidget->getPixel(dx, dy)),
                            a01 = qAlpha(_imgWidget->getPixel(dx, dy + 1)),
                            a10 = qAlpha(_imgWidget->getPixel(dx + 1, dy)),
                            a11 = qAlpha(_imgWidget->getPixel(dx + 1, dy + 1));
                    int gray = (d10-d00)*di + (d01-d00)*dj + (d11+d00-d10-d01)*di*dj + d00, alpha = (a00+a01+a10+a11)/4;
                    gray = gray>255 ? 255 : (gray<0 ? 0 : gray);
                    alpha = alpha>255 ? 255 : (alpha<0 ? 0 : alpha);
                    newimg.setPixel(i, j, qRgba(gray, gray, gray, alpha));
                }
            }
        }
        img = newimg;
    }
    if(x!=0 || y!=0){
        QImage newimg(img.width(), img.height(), QImage::Format_ARGB32);
        for(int i=0; i<newimg.width(); ++i){
            for(int j=0; j<newimg.height(); ++j){
                newimg.setPixel(i, j, _imgWidget->getPixel(i-x, j-y));
            }
        }
        img = newimg;
    }
    _imgWidget->returnModifImage();
}

void ImageProc::avgSmooth()
{
    std::vector<std::vector<int>> vec{
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    doConvolutionTemplate(vec, 1, 1, 1.0/9.0);
}

void ImageProc::boxSmooth()
{
    std::vector<std::vector<int>> vec{{1, 1, 1}};
    doConvolutionTemplate(vec, 1, 1, 0.3333);
}

void ImageProc::gaussSmooth()
{
    std::vector<std::vector<int>> vec{{1, 2, 1}};
    doConvolutionTemplate(vec, 1, 1, 0.25);
}

void ImageProc::sobelXSharp()
{
    std::vector<std::vector<int>> vec{
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}
    };
    doConvolutionTemplate(vec, 1, 1, 1);
}

void ImageProc::sobelYSharp()
{
    std::vector<std::vector<int>> vec{
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    doConvolutionTemplate(vec, 1, 1, 1);
}

void ImageProc::laplaceSharp()
{
    std::vector<std::vector<int>> vec{
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    };
    doConvolutionTemplate(vec, 1, 1, 1);
}

void ImageProc::lapExSharp()
{
    std::vector<std::vector<int>> vec{
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };
    doConvolutionTemplate(vec, 1, 1, 1);
}

void ImageProc::convolutionTemplate()
{
    auto *dialog = new ConvolutionDialog;
    connect(dialog, &ConvolutionDialog::convolutionMsg, this, &ImageProc::doConvolutionTemplate);
    dialog->exec();
}

void ImageProc::doConvolutionTemplate(std::vector<std::vector<int> > mart, int x, int y, double k)
{
    QImage &img = _imgWidget->getModifImage();
    if(img.isNull())
        return;
    QImage newimg(img.width(), img.height(), QImage::Format_ARGB32);
    for(int i=0; i<newimg.width(); ++i){
        for(int j=0; j<newimg.height(); ++j){
            int sum = 0;
            for(auto dx=0UL; dx<mart.size(); ++dx){
                for(auto dy=0UL; dy<mart[dx].size(); ++dy){
                    sum += qGray(_imgWidget->getPixel(i+dx-x, j+dy-y)) * mart[dx][dy];
                }
            }
            sum *= k;
            sum = sum>255 ? 255 : (sum<0 ? 0 : sum);
            newimg.setPixel(i, j, qRgb(sum, sum, sum));
        }
    }
    img = newimg;
    _imgWidget->returnModifImage();
}

const QString grayStr("MXKGBA@$#*21/;. ");
//本字符串测试随手所写，未进行细致构造
void ImageProc::bmp2txt()
{
    QImage img = _imgWidget->getImage();
    if(img.isNull())
        return;
    QString filename = QFileDialog::getSaveFileName(this, tr("保存txt文件"), tr("."), tr("txt File(*.txt)"));
    std::ofstream file(filename.toStdString().c_str());
    if(!file.is_open())
        return;
    QString str;
    for(int i=0; i<img.height(); ++i){
        for(int j=0; j<img.width(); ++j){
            str += grayStr.at(qGray(img.pixel(j, i)) / (256 / grayStr.size()));
        }
        str += '\n';
    }
    file << str.toStdString();
    file.close();
}
