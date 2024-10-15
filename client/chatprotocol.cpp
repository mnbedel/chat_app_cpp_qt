#include "chatprotocol.h"

#include <QDataStream>
#include <QFileInfo>
#include <QIODevice>

ChatProtocol::ChatProtocol()
{

}

QByteArray ChatProtocol::textMessage(QString message, QString receiver)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << MessageType::Text << receiver << message;

    return ba;
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

QByteArray ChatProtocol::setInitSendingFileMessage(QString fileName)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    QFileInfo info(fileName);
    out << MessageType::InitSendingFile << info.fileName() << info.size();

    return ba;
}

QByteArray ChatProtocol::setAcceptFileMessage()
{
    return getData(MessageType::AcceptSendingFile, "");
}

QByteArray ChatProtocol::setRejectFileMessage()
{
    return getData(MessageType::RejectSendingFile, "");
}

QByteArray ChatProtocol::setFileMessage(QString fileName)
{
    QByteArray ba;
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        QFileInfo info(fileName);
        out << MessageType::SendFile << info.fileName() << info.size() << file.readAll();
        file.close();
    }

    return ba;
}

void ChatProtocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _type;

    switch(_type) {
    case MessageType::Text:
        in >> _receiver >> _message;
        break;

    case MessageType::SetName:
        in >> _name;
        break;

    case MessageType::SetStatus:
        in >> _status;
        break;

    case MessageType::InitSendingFile:
        in >> _fileName >> _fileSize;
        break;

    case MessageType::SendFile:
        in >> _fileName >> _fileSize >> _fileData;
        break;

    case MessageType::ClientName:
        in >> _fileName >> _previousName >> _clientName;
        break;

    case MessageType::ConnectionACK:
        in >> _myName >> _clientsName;
        break;

    case MessageType::NewClient:
    case MessageType::ClientDisconnected:
        in >> _fileName >> _clientName;
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

const QString &ChatProtocol::myName() const
{
    return _myName;
}

const QStringList &ChatProtocol::clientsName() const
{
    return _clientsName;
}

const QString &ChatProtocol::previousName() const
{
    return _previousName;
}

const QString &ChatProtocol::clientName() const
{
    return _clientName;
}

QString ChatProtocol::receiver() const
{
    return _receiver;
}
const QByteArray &ChatProtocol::fileData() const
{
    return _fileData;
}

qint64 ChatProtocol::fileSize() const
{
    return _fileSize;
}

const QString &ChatProtocol::fileName() const
{
    return _fileName;
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
