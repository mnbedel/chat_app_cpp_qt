#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "chatprotocol.h"

#include <QObject>
#include <QTcpSocket>
#include <QDir>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 1972, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket* client, QObject *parent = nullptr);

    void connectToServer();
    void disconnectFromHost();
    void SendMessage(QString message);
    void SendName(QString name);
    void SendStatus(ChatProtocol::Status status);

    void SendIsTyping();

    void SetupClient();
    QString Name() const;

    void sendInitSendingFile(QString fileName);
    void sendAcceptFile();
    void sendRejectFile();

signals:
    void Connected();
    void Disconnected();
    //void DataReceived(QByteArray data);
    void TextMessageReceived(const QString message, QString receiver);
    void IsTyping();
    void NameChanged(QString previousName, QString name);
    void StatusChanged(ChatProtocol::Status status);
    void RejectReceivingFile();
    void InitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void FileSaved(QString path);

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
    void SaveFile();

};

#endif // CLIENTMANAGER_H
