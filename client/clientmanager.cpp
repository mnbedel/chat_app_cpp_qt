#include "clientmanager.h"

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent}, _ip{ip}, _port{port}
{
    _socket = new QTcpSocket(this);
    QObject::connect(_socket, &QTcpSocket::connected, this, &ClientManager::Connected);
    QObject::connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::Disconnected);
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::ReadyRead);
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);
}

void ClientManager::SendMessage(QString message)
{
    _socket->write(_protocol.textMessage(message));
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
        emit TextMessageReceived(_protocol.message());
        break;

    case ChatProtocol::MessageType::SetName:
        emit NameChanged(_protocol.name());
        break;

    case ChatProtocol::MessageType::SetStatus:
        emit StatusChanged(_protocol.status());
        break;

    case ChatProtocol::MessageType::IsTyping:
        emit IsTyping();
        break;
    default:
        break;
    }
}
