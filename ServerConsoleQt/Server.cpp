#include "Server.h"

Server::Server()
{
    //the server is listening a signal from any address and work in port number 2323
    if (this->listen(QHostAddress::Any, 2323))
        qDebug().noquote() << QTime::currentTime().toString()
                 << " Server is successfully turned on and listens requests for connection...";
    else
        qDebug().noquote() << QTime::currentTime().toString() << " Error: Server is not turned on";

    /*about qDebug():
    .noquote() - disables automatic insertion of quotation characters around
    .nospace() - Disables automatic insertion of spaces*/
}

void Server::incomingConnection(qintptr socketDescription)
{
    socket = new QTcpSocket; //when connection, we create a new socket to work with new Client
    socket->setSocketDescriptor(socketDescription); //assign a unique number of our connection

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect);

    vectorSockets.push_back(socket);
    qDebug().noquote().nospace() << QTime::currentTime().toString() << "  Client connected with Descriptor #"
                                 << socketDescription;
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender(); //remembering the socket that sends us a message from Client
    QDataStream in(socket); //varibale for work with data that are in socket. Convert message(QByteArray) to QString
    in.setVersion(QDataStream::Qt_6_2); //specify the version of Qt
    if (in.status() == QDataStream::Ok)
    {
        qDebug().noquote().nospace() << QTime::currentTime().toString()
                 << "  Reading a data from Client(#" << socket->socketDescriptor() << ')';
        QString message;
        in >> message;
        qDebug().noquote().nospace() << QTime::currentTime().toString() << "  Recived the message from Client(#"
                 << socket->socketDescriptor() << "): "<< message;

        sendToClient(message);
    }
    else
        qDebug().noquote().nospace() << "  Error: Con not read a data from Client(#"
                                     << socket->socketDescriptor() << ')';
}

void Server::slotDisconnect()
{
    qDebug().noquote().nospace() << QTime::currentTime().toString()
                                 << "  Someone disconnection"; //The descriptors remained:
//    if (vectorSockets.size() < 2)
//        qDebug().noquote() << "  Empty";

    for (int i = 0; i < vectorSockets.size(); i++)
    {
        if (vectorSockets[i]->socketDescriptor() == (qintptr)-1)
            continue;
        qDebug().noquote() << vectorSockets[i]->socketDescriptor() << "; ";
    }


    socket->deleteLater(); //if the first opportunity, application will delete our socket
}

void Server::sendToClient(QString messageToClient)
{
    data.clear(); //even in a empty variable there is garbage, so we need to clear its
    QDataStream out(&data, QIODevice::WriteOnly); //variable for convert our message to QByteArray and send Client
    out.setVersion(QDataStream::Qt_6_2);
    out  << QTime::currentTime() << messageToClient; //write messageToClient to QByteArray via object out
    for (int i = 0; i < vectorSockets.size(); i++)
        vectorSockets[i]->write(data); //write our QByteArray in sockets

    qDebug().noquote() << QTime::currentTime().toString() << " The message sent to the clients: ";
    for (int i = 0; i < vectorSockets.size(); i++)
        qDebug().noquote() << vectorSockets[i]->socketDescriptor() << "; ";
}

