#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "servermanager.h"
#include "chatprotocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void NewClientConnected(QTcpSocket* client);
    void ClientDisconnected(QTcpSocket* client);
    void SetClientName(QString previousName, QString name);
    void SetClientStatus(ChatProtocol::Status status);

    void on_tabChats_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    ServerManager* server;

    // functions
    void SetupServer();
};
#endif // MAINWINDOW_H
