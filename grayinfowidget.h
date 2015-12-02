#ifndef GRAYINFOWIDGET_H
#define GRAYINFOWIDGET_H

#include "ui_grayinfowidget.h"

class GrayInfoWidget : public QWidget, public Ui_GrayInfoWidget
{
    Q_OBJECT
public:
    explicit GrayInfoWidget(QWidget *parent = 0);
    ~GrayInfoWidget();
    void setGray(const int gray, const int num, const int max);
    void showInfo();
    int getGrayCnt(const int gray) const;
    int getGrayAvg() const;
    int getGrayMid() const;
    int getGraySDelta() const;
    int getPixelCnt() const;

signals:

public slots:
    void setImage(const QImage &img);

private:
    QImage *_grayInfo;
    int *_grayCnt, _grayAvg, _grayMid, _cntMax, _graySDelta, _cnt;
};

#endif // GRAYINFOWIDGET_H
