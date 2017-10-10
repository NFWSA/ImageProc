#ifndef CONVOLUTIONDIALOG_H
#define CONVOLUTIONDIALOG_H

#include "ui_convolutiondialog.h"
//#include <vector>

class ConvolutionDialog : public QDialog, public Ui_ConvolutionDialog
{
    Q_OBJECT
public:
    explicit ConvolutionDialog(QWidget *parent = 0);

signals:
    void convolutionMsg(std::vector<std::vector<int>> mart, int x, int y, double k);

public slots:
    void sizeChange(int ignore);
    void cellClick(int row, int column);
    void cellChange(const int weight);
    void centerChange(int ignore);
    void emitConvolution();

private:
    int _weightX, _weightY;
    int _centerX, _centerY;
};

#endif // CONVOLUTIONDIALOG_H
