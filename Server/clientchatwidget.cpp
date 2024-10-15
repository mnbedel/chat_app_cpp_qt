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
    QObject::connect(_client, &ClientManager::NameChanged, this, &ClientChatWidget::onClientNameChanged);
    QObject::connect(_client, &ClientManager::StatusChanged, this, &ClientChatWidget::StatusChanged);
    QObject::connect(_client, &ClientManager::FileSaved, this, &ClientChatWidget::onFileSaved);
    QObject::connect(_client, &ClientManager::InitReceivingFile, this, &ClientChatWidget::onInitReceivingFile);
    QObject::connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::SendIsTyping);

    _dir.mkdir(_client->Name());
    _dir.setPath("./" + _client->Name());
}

void ClientChatWidget::disconnect()
{
    _client->disconnectFromHost();
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

void ClientChatWidget::TextMessageReceived(QString message, QString receiver)
{
    if (receiver == "Server" || receiver == "All") {
        ui->listMessages->addItem(message);
    }

    if (receiver != "Server") {
        emit TextForOtherClients(message, receiver, _client->Name());
    }
}

void ClientChatWidget::onTyping()
{
    emit IsTyping(QString("%1 is typing").arg(_client->Name()));
}

void ClientChatWidget::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    QString message = QString("Client (%1) wants to send a file. Do you want to accept it?\nFile Name:%2\nFile Size: %3 bytes")
            .arg(clientName, fileName)
            .arg(fileSize);

    QMessageBox::StandardButton result = QMessageBox::question(this, "Receiving File", message);

    if (result == QMessageBox::Yes) {
        _client->sendAcceptFile();
    }
    else {
        _client->sendRejectFile();
    }
}

void ClientChatWidget::onFileSaved(QString path)
{
    QString message = QString("File saved here:\n%1").arg(path);
    QMessageBox::information(this, "File saved", message);
}

void ClientChatWidget::on_labelOpenFolder_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_client->Name()));
}

void ClientChatWidget::onClientNameChanged(QString previousName, QString name)
{
    QFile::rename(_dir.canonicalPath(), name);
    emit ClientNameChanged(previousName, name);
}

