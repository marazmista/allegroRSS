#include "allegrorss.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    allegroRSS w;
    w.show();

    return a.exec();
}
