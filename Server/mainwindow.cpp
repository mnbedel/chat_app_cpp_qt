#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "clientchatwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::NewClientConnected(QTcpSocket *client)
{
    int id = client->property("id").toInt();
    ui->listClients->addItem(QString("New Client added: %1").arg(id));
    ClientChatWidget* chatWidget = new ClientChatWidget(client);
    ui->tabChats->addTab(chatWidget, QString("Client (%1)").arg(id));

    QObject::connect(chatWidget, &ClientChatWidget::ClientNameChanged, this, &MainWindow::SetClientName);
    QObject::connect(chatWidget, &ClientChatWidget::StatusChanged, this, &MainWindow::SetClientStatus);
    QObject::connect(chatWidget, &ClientChatWidget::IsTyping, [this](QString name){
        this->statusBar()->showMessage(name, 600);
    });

    QObject::connect(chatWidget, &ClientChatWidget::TextForOtherClients, server, &ServerManager::onTextForOtherClients);
}

void MainWindow::ClientDisconnected(QTcpSocket *client)
{
    int id = client->property("id").toInt();
    ui->listClients->addItem(QString("Client disconnected: %1").arg(id));
}

void MainWindow::SetClientName(QString previousName, QString name)
{
    QWidget* widget = qobject_cast<QWidget*>(sender());
    int index = ui->tabChats->indexOf(widget);
    ui->tabChats->setTabText(index, name);

    server->NotifyOtherClients(previousName, name);
}

void MainWindow::SetClientStatus(ChatProtocol::Status status)
{
    QWidget* widget = qobject_cast<QWidget*>(sender());
    int index = ui->tabChats->indexOf(widget);
    QString iconname = ":/icons/";

    switch (status) {
    case ChatProtocol::Status::Available:
        iconname.append("available.png");
        break;

    case ChatProtocol::Status::Away:
        iconname.append("away.png");
        break;

    case ChatProtocol::Status::Busy:
        iconname.append("busy.png");
        break;
    default:
        iconname = "";
        break;
    }

    auto icon = QIcon(iconname);
    ui->tabChats->setTabIcon(index, icon);
}

void MainWindow::SetupServer()
{
    server = new ServerManager();
    QObject::connect(server, &ServerManager::NewClientConnected, this, &MainWindow::NewClientConnected);
    QObject::connect(server, &ServerManager::ClientDisconnected, this, &MainWindow::ClientDisconnected);
}

void MainWindow::on_tabChats_tabCloseRequested(int index)
{
    ClientChatWidget* chatWidget = qobject_cast<ClientChatWidget*>(ui->tabChats->widget(index));
    chatWidget->disconnect();
    ui->tabChats->removeTab(index);
}

