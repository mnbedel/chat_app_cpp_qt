#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QByteArray>
#include <QString>



class ChatProtocol
{
public:
    enum class MessageType{
        Text,
        File,
        IsTyping,
        SetName,
        SetStatus
    };

    enum class Status{
        None,
        Available,
        Away,
        Busy
    };

    ChatProtocol();

    QByteArray textMessage(QString message);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);

    void loadData(QByteArray data);

    Status status() const;

    const QString &name() const;

    MessageType type() const;

    const QString &message() const;

private:
    QByteArray getData(MessageType type, QString data);

    MessageType _type;
    QString _message;
    QString _name;
    Status _status;
};

#endif // CHATPROTOCOL_H
