#include "clientmanager.h"

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent}, _ip{ip}, _port{port}
{
    _socket = new QTcpSocket(this);
    SetupClient();
}

ClientManager::ClientManager(QTcpSocket *client, QObject *parent)
    : QObject{parent}, _socket{client}
{
    SetupClient();
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);
}

void ClientManager::disconnectFromHost()
{
    _socket->disconnectFromHost();
}

void ClientManager::SendMessage(QString message)
{
    _socket->write(_protocol.textMessage(message, Name()));
}

void ClientManager::SendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));
}

void ClientManager::SendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));
}

void ClientManager::SendIsTyping()
{
    _socket->write(_protocol.isTypingMessage());
}

void ClientManager::ReadyRead()
{
    QByteArray data = _socket->readAll();
    _protocol.loadData(data);

    switch(_protocol.type()) {
    case ChatProtocol::MessageType::Text:
    {
        emit TextMessageReceived(_protocol.message(), _protocol.receiver());
        break;
    }

    case ChatProtocol::MessageType::SetName:
    {
        QString previousName = _socket->property("clientName").toString();
        _socket->setProperty("clientName", Name());
        emit NameChanged(previousName, Name());
        break;
    }

    case ChatProtocol::MessageType::SetStatus:
    {
        emit StatusChanged(_protocol.status());
        break;
    }

    case ChatProtocol::MessageType::IsTyping:
    {
        emit IsTyping();
        break;
    }

    case ChatProtocol::MessageType::InitSendingFile:
    {
        emit InitReceivingFile(_protocol.name(), _protocol.fileName(), _protocol.fileSize());
        break;
    }

    case ChatProtocol::MessageType::AcceptSendingFile:
    {
        emit SendFile();
        break;
    }

    case ChatProtocol::MessageType::RejectSendingFile:
    {
        emit RejectReceivingFile();
        break;
    }

    case ChatProtocol::MessageType::SendFile:
    {
        emit SaveFile();
        break;
    }

    default:
        break;
    }
}

void ClientManager::SendFile()
{
    _socket->write(_protocol.setFileMessage(_tempFileName));
}

void ClientManager::SaveFile()
{
    QDir dir;
    dir.mkdir(Name());
    QString path = QString("%1/%2/%3_%4")
            .arg(dir.canonicalPath(), Name(), QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), _protocol.fileName());
    QFile file(path);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(_protocol.fileData());
        file.close();
        emit FileSaved(path);
    }
}

void ClientManager::SetupClient() {
    QObject::connect(_socket, &QTcpSocket::connected, this, &ClientManager::Connected);
    QObject::connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::Disconnected);
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::ReadyRead);
}

QString ClientManager::Name() const
{
    int id = _socket->property("id").toInt();
    auto name = _protocol.name().length() > 0 ? _protocol.name() : QString("Client (%1").arg(id);

    return name;
}

void ClientManager::sendInitSendingFile(QString fileName)
{
    _tempFileName = fileName;
    _socket->write(_protocol.setInitSendingFileMessage(fileName));
}

void ClientManager::sendAcceptFile()
{
    _socket->write(_protocol.setAcceptFileMessage());
}

void ClientManager::sendRejectFile()
{
    _socket->write(_protocol.setRejectFileMessage());
}
