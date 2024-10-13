#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "chatprotocol.h"

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 1972, QObject *parent = nullptr);
    void connectToServer();
    void SendMessage(QString message);
    void SendName(QString name);
    void SendStatus(ChatProtocol::Status status);

    void SendIsTyping();

signals:
    void Connected();
    void Disconnected();

    void TextMessageReceived(QString message);
    void IsTyping();
    void NameChanged(QString name);
    void StatusChanged(ChatProtocol::Status status);

private slots:
    void ReadyRead();

private:
    QTcpSocket* _socket;
    QHostAddress _ip;
    ushort _port;
    ChatProtocol _protocol;

};

#endif // CLIENTMANAGER_H
