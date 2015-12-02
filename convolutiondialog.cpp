#include "convolutiondialog.h"
#include <QDialog>
#include <QSpinBox>

ConvolutionDialog::ConvolutionDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    _weightX = _weightY = 0;
    _centerX = _xEdit->value();
    _centerY = _yEdit->value();
    _xEdit->setMaximum(_widthEdit->value());
    _yEdit->setMaximum(_heightEdit->value());

    connect(_widthEdit, SIGNAL(valueChanged(int)), this, SLOT(sizeChange(int)));
    connect(_heightEdit, SIGNAL(valueChanged(int)), this, SLOT(sizeChange(int)));
    connect(_table, &QTableWidget::cellClicked, this, &ConvolutionDialog::cellClick);
    connect(_weightEdit, SIGNAL(valueChanged(int)), this, SLOT(cellChange(int)));
    connect(_xEdit, SIGNAL(valueChanged(int)), this, SLOT(centerChange(int)));
    connect(_yEdit, SIGNAL(valueChanged(int)), this, SLOT(centerChange(int)));

    connect(this, &ConvolutionDialog::accepted, this, &ConvolutionDialog::emitConvolution);

    for(int i=0; i<_widthEdit->value(); ++i)
        for(int j=0; j<_heightEdit->value(); ++j){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(tr("0"));
            _table->setItem(j, i, item);
        }
    centerChange(0);
}

void ConvolutionDialog::sizeChange(int ignore)
{
    ignore = ignore;  //ignore
    int w = _widthEdit->value(), h = _heightEdit->value();
    _table->setColumnCount(w);
    _table->setRowCount(h);
    for(int i=0; i<w; ++i)
        for(int j=0; j<h; ++j){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            if(_table->item(j, i)!=nullptr)
                item->setText(_table->item(j, i)->text());
            else
                item->setText(tr("0"));
            _table->setItem(j, i, item);
        }
    int x = _centerX, y = _centerY;
    x = x>w ? w : x;
    y = y>h ? h : y;
    _xEdit->setValue(x);
    _xEdit->setMaximum(w);
    _yEdit->setValue(y);
    _yEdit->setMaximum(h);
    centerChange(0);
}

void ConvolutionDialog::cellClick(int row, int column)
{
    _weightX = column;
    _weightY = row;
    _weightEdit->setValue((_table->item(row, column)->text()).toInt());
}

void ConvolutionDialog::cellChange(const int weight)
{
    _table->item(_weightY, _weightX)->setText(QString("%1").arg(weight));
}

void ConvolutionDialog::centerChange(int ignore)
{
    ignore = ignore; //ignore
    if(_centerX<=_widthEdit->value() && _centerY<=_heightEdit->value())
        _table->item(_centerY - 1, _centerX - 1)->setBackgroundColor(QColor(255, 255, 255));
    _centerX = _xEdit->value();
    _centerY = _yEdit->value();
    if(_centerX<=_widthEdit->value() && _centerY<=_heightEdit->value())
        _table->item(_centerY - 1, _centerX - 1)->setBackgroundColor(QColor(128, 128, 128));
}

void ConvolutionDialog::emitConvolution()
{
    int w = _widthEdit->value(), h = _heightEdit->value();
    std::vector<std::vector<int>> vec;
    for(int i=0; i<w; ++i){
        std::vector<int> ivec;
        for(int j=0; j<h; ++j){
            ivec.push_back((_table->item(j, i)->text()).toInt());
        }
        vec.push_back(ivec);
    }
    emit convolutionMsg(vec, _centerX - 1, _centerY - 1, _kEdit->value());
}

