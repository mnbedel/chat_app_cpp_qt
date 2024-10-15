#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QByteArray>
#include <QString>



class ChatProtocol
{
public:
    enum class MessageType{
        Text,
        IsTyping,
        SetName,
        SetStatus,
        InitSendingFile,
        AcceptSendingFile,
        RejectSendingFile,
        SendFile,
        ClientName,
        ConnectionACK,
        NewClient,
        ClientDisconnected
    };

    enum class Status{
        None,
        Available,
        Away,
        Busy
    };

    ChatProtocol();

    QByteArray textMessage(QString message, QString receiver);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);
    QByteArray setInitSendingFileMessage(QString fileName);
    QByteArray setAcceptFileMessage();
    QByteArray setRejectFileMessage();
    QByteArray setFileMessage(QString fileName);

    QByteArray setClientNameMessage(QString previousName, QString currName);
    QByteArray setconnectionACKMessage(QString clientName, QStringList otherClients);
    QByteArray setNewClientMessage(QString clientName);
    QByteArray setClientDisconnectedMessage(QString clientName);

    void loadData(QByteArray data);

    Status status() const;

    const QString &name() const;

    MessageType type() const;

    const QString &message() const;

    const QString &fileName() const;

    qint64 fileSize() const;

    const QByteArray &fileData() const;

    const QString &receiver() const;

private:
    QByteArray getData(MessageType type, QString data);

    MessageType _type;
    QString _message;
    QString _name;
    Status _status;
    QString _fileName;
    qint64 _fileSize;
    QByteArray _fileData;
    QString _receiver;
};

#endif // CHATPROTOCOL_H
