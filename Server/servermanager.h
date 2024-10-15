#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "chatprotocol.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(ushort port = 1972, QObject *parent = nullptr);
    void NotifyOtherClients(QString previousName, QString name);

private slots:
    void NewClientConnectionReceived();
    void OnClientDisconnected();

public slots:
    void onTextForOtherClients(QString message, QString receiver, QString sender);

signals:
    void NewClientConnected(QTcpSocket* client);
    void ClientDisconnected(QTcpSocket* client);

private:
    QTcpServer* server;
    QMap<QString, QTcpSocket*> clients;
    ChatProtocol protocol;

private:
    void SetupServer(ushort port);

};

#endif // SERVERMANAGER_H
