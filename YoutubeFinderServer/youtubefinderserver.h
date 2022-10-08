#ifndef YOUTUBEFINDERSERVER_H
#define YOUTUBEFINDERSERVER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class YoutubeFinderServer; }
QT_END_NAMESPACE

class YoutubeFinderServer : public QMainWindow
{
    Q_OBJECT

public:
    YoutubeFinderServer(QWidget *parent = nullptr);
    ~YoutubeFinderServer();

private:
    Ui::YoutubeFinderServer *ui;
};
#endif // YOUTUBEFINDERSERVER_H
