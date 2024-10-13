#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionConnect_triggered()
{
    _client = new ClientManager();

    QObject::connect(_client, &ClientManager::Connected, [this](){
        ui->centralwidget->setEnabled(true);
    });
    QObject::connect(_client, &ClientManager::Disconnected, [this](){
        ui->centralwidget->setEnabled(false);
    });
    QObject::connect(_client, &ClientManager::TextMessageReceived, this, &MainWindow::DataReceived);
    QObject::connect(_client, &ClientManager::IsTyping, this, &MainWindow::onTyping);
    QObject::connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::SendIsTyping);
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
    _client->SendMessage(message);

    ChatItemWidget* chatwidget = new ChatItemWidget(this);
    chatwidget->SetMessage(message, true);

    QListWidgetItem* listitemwidget = new QListWidgetItem();
    listitemwidget->setSizeHint(QSize(0, 65));

    ui->listMessages->addItem(listitemwidget);

    ui->listMessages->setItemWidget(listitemwidget, chatwidget);

    ui->lineMessage->setText("");
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

