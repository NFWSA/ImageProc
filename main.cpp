#include "grayinfowidget.h"
#include "imagewidget.h"
#include "imageproc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageProc w;
    w.show();

    return a.exec();
}
