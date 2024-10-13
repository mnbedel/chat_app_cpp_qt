#include "servermanager.h"

ServerManager::ServerManager(ushort port, QObject *parent)
    : QObject{parent}
{
    SetupServer(port);
}

void ServerManager::NewClientConnectionReceived()
{
    QTcpSocket* client = server->nextPendingConnection();
    clients << client;

    long long id = clients.length();
    client->setProperty("id", id);

    QObject::connect(client, &QTcpSocket::disconnected, this, &ServerManager::OnClientDisconnected);
    emit NewClientConnected(client);
}

void ServerManager::OnClientDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender()); // sender() returns pointer to the object that sent the signal.
    //int id = client->property("id").toInt();

    clients.removeOne(client);
    emit ClientDisconnected(client);
}

void ServerManager::SetupServer(ushort port)
{
    server = new QTcpServer(this); // parent is ithis pointer

    // When a new connection is established, call NewClientConnectionReceived slot function.
    QObject::connect(server, &QTcpServer::newConnection, this, &ServerManager::NewClientConnectionReceived);
    server->listen(QHostAddress::Any, port); // listen any IP on port
}
