#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include "clientmanager.h"

#include <QWidget>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTcpSocket>

namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatWidget(QTcpSocket* _client, QWidget *parent = nullptr);
    ~ClientChatWidget();

private slots:
    void on_buttonSend_clicked();
    void ClientDisconnected();
    void TextMessageReceived(QString message);
    void onTyping();

    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void onFileSaved(QString path);
    void on_labelOpenFolder_linkActivated(const QString &link);

    void onClientNameChanged(QString name);

signals:
    void ClientNameChanged(QString name);
    void IsTyping(QString message);
    void StatusChanged(ChatProtocol::Status status);

private:
    Ui::ClientChatWidget *ui;
    ClientManager* _client;
    QDir _dir;
};

#endif // CLIENTCHATWIDGET_H
