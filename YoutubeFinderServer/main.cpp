#include "youtubefinderserver.h"
#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YoutubeFinderServer w;
    w.show();

    Server *tcp = new Server(54000);
    return a.exec();
}
