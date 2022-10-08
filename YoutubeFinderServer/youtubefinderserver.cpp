#include "youtubefinderserver.h"
#include "./ui_youtubefinderserver.h"

YoutubeFinderServer::YoutubeFinderServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::YoutubeFinderServer)
{
    ui->setupUi(this);
}

YoutubeFinderServer::~YoutubeFinderServer()
{
    delete ui;
}

