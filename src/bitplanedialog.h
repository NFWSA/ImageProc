#ifndef BITPLANEDIALOG_H
#define BITPLANEDIALOG_H

#include "ui_bitplanedialog.h"

class BitPlaneDialog : public QDialog, public Ui_BitPlaneDialog
{
    Q_OBJECT
public:
    explicit BitPlaneDialog(QWidget *parent = 0);

signals:

public slots:
    void showPlane(const QImage &img);
    void okPush();

};

#endif // BITPLANEDIALOG_H
