#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(ushort port = 1972, QObject *parent = nullptr);

private slots:
    void NewClientConnectionReceived();
    void OnClientDisconnected();

signals:
    void NewClientConnected(QTcpSocket* client);
    void ClientDisconnected(QTcpSocket* client);

private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;

private:
    void SetupServer(ushort port);

};

#endif // SERVERMANAGER_H
