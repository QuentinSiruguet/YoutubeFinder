#ifndef SERVER_H
#define SERVER_H


#include <QObject>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>


#include <QByteArray>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUrl>
#include <QRandomGenerator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


class Server: public QObject
{
    Q_OBJECT
protected:
    void incomingConnection();

private:
    bool running = true;
    int port;
    std::map<QTcpSocket*, std::string> users;
    std::map<std::string, int> users_credits;
    std::map<QTcpSocket*, std::string> work;

    QTcpServer *server;
    std::vector<QTcpSocket*> clients;

    const std::vector<char> base64 = { '0','1','2','3','4','5','6','7','8','9'
        ,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
        ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-','_' };

    std::string generate9ID();

public:
    Server(int port,QObject *parent = 0);
    //~Server();
public slots:
    void disconnected();
    void readyRead();
};


#endif // SERVER_H
