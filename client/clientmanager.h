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
    void SendMessage(QString message, QString receiver);
    void SendName(QString name);
    void SendStatus(ChatProtocol::Status status);

    void SendIsTyping();

    void sendInitSendingFile(QString fileName);
    void sendAcceptFile();
    void sendRejectFile();

signals:
    void Connected();
    void Disconnected();

    void TextMessageReceived(QString message);
    void IsTyping();
    void NameChanged(QString name);
    void StatusChanged(ChatProtocol::Status status);
    void RejectReceivingFile();
    void InitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void ConnectionACK(QString myName, QStringList clientsName);
    void NewClientConnectedToServer(QString clientName);
    void ClientNameChanged(QString previousName, QString clientName);
    void ClientDisconnected(QString clientName);

private slots:
    void ReadyRead();

private:
    QTcpSocket* _socket;
    QHostAddress _ip;
    ushort _port;
    ChatProtocol _protocol;
    QString _tempFileName;

private:
    void SendFile();

};

#endif // CLIENTMANAGER_H
