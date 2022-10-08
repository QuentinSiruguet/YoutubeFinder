#include "youtubefinder.h"
#include <QApplication>
#include <QLinearGradient>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    YoutubeFinder w;
    qRegisterMetaType<QAbstractSocket::SocketError>();

    w.setPalette(QPalette(QColor(41, 55, 61)));
    w.show();

    return app.exec();
}
