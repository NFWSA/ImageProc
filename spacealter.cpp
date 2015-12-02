#include "spacealter.h"

SpaceAlter::SpaceAlter(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(this, &SpaceAlter::accepted, this, &SpaceAlter::buttonClicked);
}

SpaceAlter::~SpaceAlter()
{

}

void SpaceAlter::buttonClicked()
{
    emit doSpaceAlter(_widthEdit->value(), _heightEdit->value(), _angleEdit->value(),
                      _deltaXEdit->value(), _deltaYEdit->value(), _nearType->isChecked());
}
