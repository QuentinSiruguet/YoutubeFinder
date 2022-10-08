#include "youtubefinder.h"
#include "./ui_youtubefinder.h"


YoutubeFinder::YoutubeFinder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::YoutubeFinder)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    socket = new QTcpSocket(this);

    setStyle();
    setConnect();
}

YoutubeFinder::~YoutubeFinder()
{
    delete ui;
}

void YoutubeFinder::paintEvent(QPaintEvent *event)
{

    QPainter p(this);
    if(!performance){
        p.setPen(QColor(0,(count/16),255-(count/16)));
        p.translate(this->size().width()/2, this->size().height());
        branch(&p, count/32);
        p.end();
    }
}

void YoutubeFinder::branch(QPainter *p, float len)
{
    p->drawLine(0, 0, 0, -len);
    p->translate(0, -len);

    if(len>10 && p->isActive())
    {
        for(int i = 1; i <= 2; i++)
        {
            p->save();
            p->rotate((10+count/75)/i);
            branch(p,len*0.55);
            p->restore();

            p->save();
            p->rotate(-(10+count/75)/i);
            branch(p,len*0.55);
            p->restore();
        }
    }
}

void YoutubeFinder::setStyle()
{
    no_server = ui->NOSERVER;
    name_text = ui->NAMETEXT;
    enter_button = ui->ENTERBUTTON;
    url_youtube = ui->URLYOUTUBE;
    mode_performance = ui->PERFMODE;

    QPalette p = name_text->palette();
    p.setColor(QPalette::Base, QColor(81, 95, 101));
    name_text->setPalette(p);
    name_text->setText("John DOE");
    name_text->setAlignment(Qt::AlignCenter);
}

void YoutubeFinder::setConnect()
{
    connect(name_text, &QTextEdit::textChanged,this, &YoutubeFinder::name_textChange);
    connect(enter_button, &QPushButton::clicked, this, &YoutubeFinder::enterClicked);
    connect(mode_performance, &QRadioButton::toggled, this, &YoutubeFinder::togglePerformance);

    connect(manager, &QNetworkAccessManager::finished,this, &YoutubeFinder::replyFinished);
    connect(socket, &QTcpSocket::errorOccurred, this, &YoutubeFinder::serverConnect);
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
}

void YoutubeFinder::togglePerformance()
{
    performance = !performance;
    this->update();
}
void YoutubeFinder::enterClicked()
{

    if(!connecting){
        socket->connectToHost("127.0.0.1", 54000);
        connecting = true;
    }
    if(online)
    {
        socket->close();
        delete manager;
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished,this, &YoutubeFinder::replyFinished);


        connecting = false;
        online = false;

        ui->NAMEONLINE->setText("");
        ui->CREDITS->setText("");

        count = 4095;
        this->update();
        no_server->setText("");
        enter_button->setText("ENTER");
        name_text->show();
        url_youtube->hide();
    }
}

void YoutubeFinder::name_textChange()
{
    if(name_text->toPlainText().size() > 20 || name_text->toPlainText().at(name_text->toPlainText().size()-1) =='\n')
    {
        QChar fin = name_text->toPlainText().at(name_text->toPlainText().size()-1);
        std::string name = name_text->toPlainText().toStdString().substr(0, name_text->toPlainText().size()-1);
        name_text->setText(QString::fromStdString(name));
        name_text->moveCursor(QTextCursor::End);
        name_text->setAlignment(Qt::AlignCenter);
        if(fin == '\n')
            enterClicked();
    }
}

void YoutubeFinder::serverConnect(QAbstractSocket::SocketError error)
{
    if(!socket->waitForConnected(1000))
    {
        no_server->setText("Can't connect to the server\n\nTrying to reconnect...");
        socket->connectToHost("127.0.0.1", 54000);
    }
}

void YoutubeFinder::readyRead()
{
    no_server->setText("");
    enter_button->setText("QUIT");
    online = true;
    name_text->hide();
    url_youtube->show();
    ui->NAMEONLINE->setText(name_text->toPlainText());

    QByteArray myByte = socket->readAll();
    qDebug() << myByte;
    if(myByte.contains("#ASKCONNECTIONID"))
    {
        QString commandid = "\\connect "+name_text->toPlainText();
        socket->write(commandid.toStdString().c_str());
    }

    else if(myByte.contains("#CONFIRMEDCONNECTION") || myByte.contains("#CONFIRMEDDONE"))
    {

        QString credits = myByte.toStdString().substr(myByte.toStdString().find(' ')+1, myByte.toStdString().size()).c_str();
        ui->CREDITS->setText(credits + " CREDITS");
        QString commandgen = "\\gen9id";
        socket->write(commandgen.toStdString().c_str());
    }
    else if(myByte.contains("#9ID"))
    {
        std::string gen9id = myByte.toStdString().substr(4, myByte.size());
        std::string url = "https://www.youtube.com/watch?v="+gen9id;
        url_youtube->setText(url.c_str());
        parseReceived(gen9id);
    }
    else if(myByte.contains("#SERVEROUT"))
    {
        enterClicked();
    }
}

void YoutubeFinder::parseReceived(std::string r)
{
    count = 0;
    for(int i = 0; i < 64; i++)
    { 
        for(int y = 0; y < 64; y++)
        {
            std::stringstream url;
            url << "https://www.youtube.com/watch?v="<<r.substr(0,r.size()-1)<<base64[i]<<base64[y];
            QUrl u(url.str().c_str());
            manager->get(QNetworkRequest(u));
        }
    }
}


void YoutubeFinder::replyFinished(QNetworkReply *reply) {
    QString answer = reply->readAll();
    if(!performance)
        url_youtube->setText(reply->url().toString());

    if(answer.contains("\"status\":\"ERROR\""));
    else
    {
        socket->write(reply->url().toString().toStdString().c_str());
        qDebug() << "FIND ONE at " << reply->url();
    }
    reply->deleteLater();
    count++;

    if(!performance && count < 4096)
        this->update();

    else if(count == 4096)
        socket->write("\\done");
}
