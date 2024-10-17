#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientmanager.h"
#include "chatitemwidget.h"

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
    void on_actionConnect_triggered();
    void DataReceived(QString message);

    void on_buttonSend_clicked();

    void on_lineClientName_editingFinished();

    void on_comboStatus_currentIndexChanged(int index);
    void onTyping();

    void on_buttonSendFile_clicked();
    void onRejectReceivingFile();
    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);

    void onConnectionACK(QString myName, QStringList clientsName);
    void onNewClientConnectedToServer(QString clientName);
    void onClientNameChanged(QString previousName, QString clientName);
    void onClientDisconnected(QString clientName);

    void on_lineMessage_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    ClientManager* _client;
    void SetupClient();
};
#endif // MAINWINDOW_H
