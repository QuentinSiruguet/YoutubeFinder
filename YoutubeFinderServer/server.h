#ifndef SERVER_H
#define SERVER_H

//QT
#include <QObject>
#include <QDateTime>
#include <QRandomGenerator>

//STD
#include <iostream>
#include <string>
#include <sstream>
#include <map>

//QT Network
#include <QByteArray>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

//QT SQL
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>


class Server: public QObject
{
    Q_OBJECT
protected:
    void incomingConnection();

private:
    bool running = true;
    int port;
    std::map<QTcpSocket*, std::string> users;
    std::map<QTcpSocket*, int> users_id;
    std::map<std::string, int> users_credits;
    std::map<QTcpSocket*, std::string> work;

    QTcpServer *server;

    const std::vector<char> base64 = { '0','1','2','3','4','5','6','7','8','9'
        ,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
        ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-','_' };

    std::string generate9ID();
    void updateCredits(std::string);

public:
    Server(int port,QObject *parent = 0);
    //~Server();
public slots:
    void disconnected();
    void readyRead();
};


#endif // SERVER_H
