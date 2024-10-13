#include "clientchatwidget.h"
#include "ui_clientchatwidget.h"

ClientChatWidget::ClientChatWidget(QTcpSocket* client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    //QObject::connect(_client, &QTcpSocket::readyRead, this, &ClientChatWidget::DataReceived);
    QObject::connect(_client, &ClientManager::Disconnected, this, &ClientChatWidget::ClientDisconnected);
    QObject::connect(_client, &ClientManager::TextMessageReceived, this, &ClientChatWidget::TextMessageReceived);
    QObject::connect(_client, &ClientManager::IsTyping, this, &ClientChatWidget::onTyping);
    QObject::connect(_client, &ClientManager::NameChanged, this, &ClientChatWidget::ClientNameChanged);
    QObject::connect(_client, &ClientManager::StatusChanged, this, &ClientChatWidget::StatusChanged);
    QObject::connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::SendIsTyping);
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

void ClientChatWidget::on_buttonSend_clicked()
{
    auto message = ui->lineMessage->text().trimmed();
    _client->SendMessage(message);
    ui->lineMessage->setText("");
    ui->listMessages->addItem(message);
}

void ClientChatWidget::ClientDisconnected()
{
    ui->widgetSend->setEnabled(false);
}

void ClientChatWidget::TextMessageReceived(QString message)
{
    ui->listMessages->addItem(message);
}

void ClientChatWidget::onTyping()
{
    emit IsTyping(QString("%1 is typing").arg(_client->Name()));
}

