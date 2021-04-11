#include "cutting_polygon.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cutting_polygon w;
    w.show();
    return a.exec();
}
