#ifndef SPACEALTER_H
#define SPACEALTER_H

#include <QDialog>
#include "ui_spacealter.h"

class SpaceAlter : public QDialog, public Ui_SpaceAlter
{
    Q_OBJECT

public:
    explicit SpaceAlter(QWidget *parent = 0);
    ~SpaceAlter();

signals:
    void doSpaceAlter(double w, double h, int angle, int x, int y, bool type);

public slots:
    void buttonClicked();

private:
};

#endif // SPACEALTER_H
