#ifndef YOUTUBEFINDER_H
#define YOUTUBEFINDER_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

#include <QPainter>

#include <QTcpSocket>
#include <QUrl>
#include <QRandomGenerator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <iostream>
#include <string>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class YoutubeFinder; }
QT_END_NAMESPACE

class YoutubeFinder : public QMainWindow
{
    Q_OBJECT

public:
    YoutubeFinder(QWidget *parent = nullptr);
    ~YoutubeFinder();
    virtual void paintEvent(QPaintEvent *event);

private slots:

    void serverConnect(QAbstractSocket::SocketError error);
    void readyRead();
    void replyFinished(QNetworkReply *reply);
    void enterClicked();
    void name_textChange();
    void togglePerformance();

private:
    Ui::YoutubeFinder *ui;

    //QPainter *p;

    QTextEdit *name_text;
    QLabel *no_server;
    QLabel *url_youtube;
    QPushButton *enter_button;
    QRadioButton *mode_performance;

    QTcpSocket* socket;
    QNetworkAccessManager *manager;

    bool connecting = false;
    bool online = false;
    bool performance = false;
    int count = 4095;

    void setStyle();
    void setConnect();

    void branch(QPainter*,float);

    void parseReceived(std::string r);
    void find();
};

const std::vector<char> base64 = { '0','1','2','3','4','5','6','7','8','9'
                                  ,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
                                  ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-','_' };
#endif // YOUTUBEFINDER_H
