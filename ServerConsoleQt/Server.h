#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QTime>

class Server : public QTcpServer
{
    Q_OBJECT
public:
       Server(); //when Server is turned on, it is already listening for any connection
       QTcpSocket* socket;

public:
       QVector <QTcpSocket*> vectorSockets; //keeping created sockets for work with Clients
       QByteArray data; //this variable will be traveling from Server to Client; from Client to Server
       void sendToClient(QString messageToClient); //foonction for send some data to Client

public slots:
       void incomingConnection(qintptr socketDescription); //handler for new connection (QTcpServer's)
       void slotReadyRead(); //handler for socket ready receving a message from Client
       void slotDisconnect(); //just for disconnect Client's socket


};

#endif // SERVER_H
