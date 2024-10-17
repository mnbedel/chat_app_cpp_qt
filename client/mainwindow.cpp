#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);

    SetupClient();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::SetupClient()
{
    _client = new ClientManager();

    QObject::connect(_client, &ClientManager::Connected, [this](){
        ui->centralwidget->setEnabled(true);
        ui->actionConnect->setEnabled(false);
        ui->buttonSend->setEnabled(false);
    });
    QObject::connect(_client, &ClientManager::Disconnected, [this](){
        ui->centralwidget->setEnabled(false);
        ui->actionConnect->setEnabled(true);
    });
    QObject::connect(_client, &ClientManager::TextMessageReceived, this, &MainWindow::DataReceived);
    QObject::connect(_client, &ClientManager::IsTyping, this, &MainWindow::onTyping);
    QObject::connect(_client, &ClientManager::InitReceivingFile, this, &MainWindow::onInitReceivingFile);
    QObject::connect(_client, &ClientManager::RejectReceivingFile, this, &MainWindow::onRejectReceivingFile);
    //QObject::connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::SendIsTyping);
    QObject::connect(_client, &ClientManager::ConnectionACK, this, &MainWindow::onConnectionACK);
    QObject::connect(_client, &ClientManager::NewClientConnectedToServer, this, &MainWindow::onNewClientConnectedToServer);
    QObject::connect(_client, &ClientManager::ClientNameChanged, this, &MainWindow::onClientNameChanged);
    QObject::connect(_client, &ClientManager::ClientDisconnected, this, &MainWindow::onClientDisconnected);
}

void MainWindow::on_actionConnect_triggered()
{
    _client->connectToServer();
}

void MainWindow::DataReceived(QString message)
{
    ChatItemWidget* chatwidget = new ChatItemWidget(this);
    chatwidget->SetMessage(message);

    QListWidgetItem* listitemwidget = new QListWidgetItem();
    listitemwidget->setSizeHint(QSize(0, 65));

    ui->listMessages->addItem(listitemwidget);
    listitemwidget->setBackground(QColor(167, 255, 237));

    ui->listMessages->setItemWidget(listitemwidget, chatwidget);
}


void MainWindow::on_buttonSend_clicked()
{
    QString message = ui->lineMessage->text().trimmed();
    _client->SendMessage(message, ui->comboDestination->currentText());

    ChatItemWidget* chatwidget = new ChatItemWidget(this);
    chatwidget->SetMessage(message, true);

    QListWidgetItem* listitemwidget = new QListWidgetItem();
    listitemwidget->setSizeHint(QSize(0, 65));

    ui->listMessages->addItem(listitemwidget);

    ui->listMessages->setItemWidget(listitemwidget, chatwidget);

    ui->lineMessage->setText("");
    ui->lineMessage->setFocus();
}


void MainWindow::on_lineClientName_editingFinished()
{
    QString name = ui->lineClientName->text().trimmed();
    _client->SendName(name);
}


void MainWindow::on_comboStatus_currentIndexChanged(int index)
{
    ChatProtocol::Status status = (ChatProtocol::Status)index;
    _client->SendStatus(status);
}

void MainWindow::onTyping()
{
    statusBar()->showMessage("Server is typing", 600);
}


void MainWindow::on_buttonSendFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a file", "/home");
    _client->sendInitSendingFile(fileName);
}

void MainWindow::onRejectReceivingFile()
{
    QMessageBox::critical(this, "Sending File", "Operation Rejected");
}

void MainWindow::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
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

void MainWindow::onConnectionACK(QString myName, QStringList clientsName)
{
    ui->comboDestination->clear();
    clientsName.prepend("All");
    clientsName.prepend("Server");

    foreach (QString client, clientsName) {
        ui->comboDestination->addItem(client);
    }

    setWindowTitle(myName);
}

void MainWindow::onNewClientConnectedToServer(QString clientName)
{
    ui->comboDestination->addItem(clientName);
}

void MainWindow::onClientNameChanged(QString previousName, QString clientName)
{
    for (int idx = 0; idx < ui->comboDestination->count(); idx++) {
        if (ui->comboDestination->itemText(idx) == previousName) {
            ui->comboDestination->setItemText(idx, clientName);
        }
    }
}

void MainWindow::onClientDisconnected(QString clientName)
{
    for (int idx = 0; idx < ui->comboDestination->count(); idx++) {
        if (ui->comboDestination->itemText(idx) == clientName) {
            ui->comboDestination->removeItem(idx);
        }
    }
}


void MainWindow::on_lineMessage_textChanged(const QString &arg1)
{
    ui->buttonSend->setEnabled(arg1.trimmed().length() > 0);
    _client->SendIsTyping();
}

