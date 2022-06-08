#include "Server.h"

Server::Server()
{
    //the server is listening a signal from any address and work in port number 2323
    if (this->listen(QHostAddress::Any, 2323))
        qDebug() << "Server is successfully turned on and listens requests for connection";
    else
        qDebug() << "Error: Server is not turned on";
}

void Server::incomingConnection(qintptr socketDescription)
{
    socket = new QTcpSocket; //when connection, we create a new socket to work with new Client
    socket->setSocketDescriptor(socketDescription); //assign a unique number of our connection

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect);

    vectorSockets.push_back(socket);
    qDebug() << "Client connected with Descriptor# " << socketDescription;

    //sendToClient("A message from Server to Client"); //send information about successful connection to Server

}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender(); //remembering the socket that sends us a message from Client
    QDataStream in(socket); //varibale for work with data that are in socket. Convert message(QByteArray) to QString
    in.setVersion(QDataStream::Qt_6_2); //specify the version of Qt
    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "Reading a data from Client...";
        QString message;
        in >> message;
        qDebug() << "Recived the message from Client: " << message;
        sendToClient(message);
    }
    else
        qDebug() << "Error: Con not read a data from Client";
}

void Server::slotDisconnect()
{
    //qDebug() << "Disconnection. Descriptor# " << socket->socketDescriptor();
    socket->deleteLater(); //if the first opportunity, application will delete our socket
}

void Server::sendToClient(QString messageToClient)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly); //variable for convert our message to QByteArray and send Client
    out.setVersion(QDataStream::Qt_6_2);
    out << messageToClient; //write messageToClient to QByteArray via object out
    //socket->write(data); //write our QByteArray in socket
    for (int i = 0; i < vectorSockets.size(); i++)
        vectorSockets[i]->write(data);
}

