#include "server.h"

#define SHOW_USER "USER #" << socket << "[" << users[socket].c_str() << "]: "


Server::Server(int port,QObject *parent) : port(port), QObject(parent)
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("IPDATABASE");
    db.setPort(5432);
    db.setUserName("USERNAME DATABASE");
    db.setPassword("PASSWORD DATABASE");
    db.setDatabaseName("DATABASE NAME");

    if(db.open())
        qDebug() << "Vous êtes maintenant connecté à " << db.hostName();

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
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("#ASKCONNECTIONID");
    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
}

void Server::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    socket->deleteLater();
    users.erase(socket);
}

void Server::readyRead() //UN CLIENT ENVOIE DES DONNEES
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    QByteArray myByte = socket->readAll();

    if (myByte.toStdString().substr(0, 8) == "\\connect")
    {
        users[socket] = myByte.toStdString().substr(9, myByte.toStdString().size());

        //ON CREE LE COMPTE SI IL N'EXISTE PAS
        QSqlQuery query;
        std::stringstream q;
        q << "SELECT COUNT(user_name) FROM Users WHERE user_name='" << users[socket] << "'";
        query.exec(q.str().c_str());
        query.first();
        if(query.value(0) == 0) //SI IL N'Y A PAS LE MEME USERNAME
        {
            QSqlQuery create_user;
            QDateTime date = date.currentDateTime();
            std::stringstream k;
            k << "INSERT INTO Users(user_name, user_credits, user_trophies, created_on) VALUES ('" << users[socket] << "', 0, 0,'" << date.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "')";
            create_user.exec(k.str().c_str());
        }

        //ON STOCK SON ID
        q.str("");
        query.clear();
        q << "SELECT user_id FROM Users WHERE user_name='" << users[socket] << "'";
        query.exec(q.str().c_str());
        query.first();
        users_id[socket] = query.value(0).toInt();
        std::stringstream message;
        message << "#CONFIRMEDCONNECTION";
        socket->write(message.str().c_str());

        //ON UPDATE LES CREDITS DE TOUT LES CLIENTS
        updateCredits(users[socket]);

    }
    else if (myByte.toStdString().substr(0, 7) == "\\gen9id") //ON GENERE ET ENVOIE LE 9ID
    {
        std::string message = "#9ID";
        std::string id = generate9ID();
        message += id;
        socket->write(message.c_str());
        work[socket] = id;
    }
    else if (myByte.toStdString().substr(0, 5) == "\\done") //LE CLIENT A TERMINÉ
    {
        //ON INCREMENTE LES CREDITS
        QSqlQuery query;
        std::stringstream q;
        q << "UPDATE Users SET user_credits = user_credits+1 WHERE user_name='" << users[socket] << "'";
        query.exec(q.str().c_str());

        //ON AJOUTE DANS WORKS
        q.str("");
        QDateTime date = date.currentDateTime();
        q << "INSERT INTO Works(nine_id, user_id, created_on) VALUES ('" << work[socket] << "','"<<users_id[socket]<<"','" << date.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "')";
        query.exec(q.str().c_str());

        //ON ENVOIE AU CLIENT
        std::stringstream message;
        message << "#CONFIRMEDDONE";
        socket->write(message.str().c_str());

        //ON UPDATE LES CREDITS DE TOUT LES CLIENTS
        updateCredits(users[socket]);

        work.erase(socket);
    }
    else if(myByte.toStdString().substr(0,6) == "\\found") //LE CLIENT A TROUVÉ UNE VIDEO
    {
        std::string full_id = myByte.toStdString().substr(7,myByte.toStdString().size());
        QDateTime date = date.currentDateTime();

        //ON AJOUTE A TABLE VIDEOS
        QSqlQuery query;
        std::stringstream q;
        q << "INSERT INTO Videos(video_full_id,nine_id,user_id,found_on) VALUES ('" << full_id << "','"<<work[socket] << "'," << users_id[socket] <<",'" << date.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "')";
        query.exec(q.str().c_str());

        //ON UPDATE USER TROPHIES
        q.str("");
        query.clear();
        q << "UPDATE Users SET user_trophies = user_trophies+1 WHERE user_name='" << users[socket] << "'";
        query.exec(q.str().c_str());

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
        toreturn += base64[QRandomGenerator::global()->bounded(64)];
    return toreturn;
}

void Server::updateCredits(std::string user)
{
    //ON RECUPERE LE NOMBRE DE CREDITS
    std::stringstream q;
    QSqlQuery query;
    q.str("");
    query.clear();
    q << "SELECT user_credits FROM Users WHERE user_name='" << user << "'";
    query.exec(q.str().c_str());
    query.first();
    users_credits[user] = query.value(0).toInt();

    //ON MET A JOUR TOUT LES CLIENTS CONNECTÉS SOUS LE MÊME USERNAME

    for(auto &u : users)
    {
        if(u.second == user)
        {
            q.str("");
            q << "#UPDATECREDITS " << users_credits[user];
            u.first->write(q.str().c_str());
        }
    }

}
