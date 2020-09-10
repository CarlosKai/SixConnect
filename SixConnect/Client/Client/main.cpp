#include "chessclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chessClient w;
    w.show();

    return a.exec();
}
