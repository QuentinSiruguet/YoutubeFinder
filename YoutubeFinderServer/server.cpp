#include "server.h"

#define SHOW_USER "USER #" << socket << "[" << users[socket].c_str() << "]: "


Server::Server(int port,QObject *parent) : port(port), QObject(parent)
{
   server = new QTcpServer;
    server->listen(QHostAddress::Any, port);

    if (!server->isListening())
    {
        qDebug() << "Can't create a socket! Quitting";
        return;
    }
    connect(server, &QTcpServer::newConnection, this, &Server::incomingConnection);

}

void Server::incomingConnection(){

    qDebug() << " Connecting...";
    QTcpSocket *socket = server->nextPendingConnection();
    clients.push_back(socket);
    socket->write("#ASKCONNECTIONID");
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Server::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    qDebug() << SHOW_USER << "DISCONNECTED" << Qt::endl;
    users.erase(socket);

}
void Server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray myByte = socket->readAll();
    if (myByte.toStdString().substr(0, 8) == "\\connect")
    {
        users[socket] = myByte.toStdString().substr(9, myByte.toStdString().size());
        qDebug() <<  SHOW_USER <<"CONNECTED" << Qt::endl;

        std::stringstream message;
        message << "#CONFIRMEDCONNECTION " << users_credits[users[socket]];
        socket->write(message.str().c_str());
    }
    else if (myByte.toStdString().substr(0, 7) == "\\gen9id")
    {
        std::string message = "#9ID";
        std::string id = generate9ID();
        message += id;
        socket->write(message.c_str());
        work[socket] = id;
        qDebug() << SHOW_USER << "START WORK ON ID : " << work[socket].c_str() << Qt::endl;
    }
    else if (myByte.toStdString().substr(0, 5) == "\\done")
    {
        qDebug() << SHOW_USER << "DONE ID : "<< work[socket].c_str() << Qt::endl;
        work.erase(socket);
        users_credits[users[socket]]++;
        std::stringstream message;
        message << "#CONFIRMEDDONE " << users_credits[users[socket]];
        socket->write(message.str().c_str());
    }
    else
    {
       qDebug() << SHOW_USER << myByte << Qt::endl;
    }
}

std::string Server::generate9ID()
{
    std::string toreturn;
    for (int i = 0; i < 9; i++)
        toreturn += base64[std::rand() / ((RAND_MAX + 1u) / 64)];
    return toreturn;
}
