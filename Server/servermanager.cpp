#include "servermanager.h"

ServerManager::ServerManager(ushort port, QObject *parent)
    : QObject{parent}
{
    SetupServer(port);
}

void ServerManager::NotifyOtherClients(QString previousName, QString name)
{
    QByteArray message = protocol.setClientNameMessage(previousName, name);

    foreach (QTcpSocket* cl, clients) {
        auto clientName = cl->property("clientName").toString();
        if (clientName != name) {
            cl->write(message);
        }
    }
}

void ServerManager::NewClientConnectionReceived()
{
    QTcpSocket* client = server->nextPendingConnection();


    long long id = clients.count();
    QString clientName = QString("Client (%1)").arg(id);
    client->setProperty("id", id);
    client->setProperty("clientName", clientName);

    QObject::connect(client, &QTcpSocket::disconnected, this, &ServerManager::OnClientDisconnected);
    emit NewClientConnected(client);

    if (id > 1) {
        QByteArray message = protocol.setconnectionACKMessage(clientName, clients.keys());
        client->write(message);

        QByteArray newClientMessage = protocol.setNewClientMessage(clientName);

        foreach (QTcpSocket* cl, clients) {
            cl->write(newClientMessage);
        }
    }

    clients[clientName] = client;
}

void ServerManager::OnClientDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender()); // sender() returns pointer to the object that sent the signal.

    QString clientName = client->property("clientName").toString();
    clients.remove(clientName);
    auto message = protocol.setClientDisconnectedMessage(clientName);

    foreach(QTcpSocket* cl, clients) {
        cl->write(message);
    }

    emit ClientDisconnected(client);
}

void ServerManager::onTextForOtherClients(QString message, QString receiver, QString sender)
{
    QByteArray mesg = protocol.textMessage(message, receiver);
    if (receiver == "All") {
        foreach (QTcpSocket* cl, clients) {
            auto clientName = cl->property("clientName").toString();
            if (clientName != sender) {
                cl->write(mesg);
            }
        }
    }
    else {
        foreach (QTcpSocket* cl, clients) {
            auto clientName = cl->property("clientName").toString();
            if (clientName == receiver) {
                cl->write(mesg);
                return;
            }
        }
    }
}

void ServerManager::SetupServer(ushort port)
{
    server = new QTcpServer(this); // parent is ithis pointer

    // When a new connection is established, call NewClientConnectionReceived slot function.
    QObject::connect(server, &QTcpServer::newConnection, this, &ServerManager::NewClientConnectionReceived);
    server->listen(QHostAddress::Any, port); // listen any IP on port
}
