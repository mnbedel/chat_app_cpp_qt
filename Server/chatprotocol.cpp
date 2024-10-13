#include "chatprotocol.h"

#include <QDataStream>
#include <QIODevice>

ChatProtocol::ChatProtocol()
{

}

QByteArray ChatProtocol::textMessage(QString message)
{
    return getData(MessageType::Text, message);
}

QByteArray ChatProtocol::isTypingMessage()
{
    return getData(MessageType::IsTyping, "");
}

QByteArray ChatProtocol::setNameMessage(QString name)
{
    return getData(MessageType::SetName, name);
}

QByteArray ChatProtocol::setStatusMessage(Status status)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << MessageType::SetStatus << status;

    return ba;
}

void ChatProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _type;

    switch(_type) {
    case MessageType::Text:
        in >> _message;
        break;

    case MessageType::SetName:
        in >> _name;
        break;

    case MessageType::SetStatus:
        in >> _status;
        break;

    default:
        break;
    }
}

QByteArray ChatProtocol::getData(MessageType type, QString data)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << data;

    return ba;
}

const QString &ChatProtocol::message() const
{
    return _message;
}

ChatProtocol::MessageType ChatProtocol::type() const
{
    return _type;
}

const QString &ChatProtocol::name() const
{
    return _name;
}

ChatProtocol::Status ChatProtocol::status() const
{
    return _status;
}
