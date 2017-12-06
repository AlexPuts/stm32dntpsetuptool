#ifndef setuptool_H
#define setuptool_H

#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include "ntpserver.h"
#include "settingsform.h"

class QDialogButtonBox;
class QLabel;
class QPushButton;
class QTimer;
class QUdpSocket;
namespace Ui {
class setuptool;
}

class setuptool : public QMainWindow
{
    Q_OBJECT

public:
    explicit setuptool(QMainWindow *parent = 0);

private slots:
    void startBroadcasting();
    void stopBroadcasting();
    void broadcastDatagram();
    void processPendingDatagrams();
    void openSettings(QListWidgetItem*);


private:
    Ui::setuptool *ui;
    QUdpSocket *udpSocket;
    QTimer *timer;
    int messageNo;
    QVector<QUdpSocket*> broadcast_sockets,receive_sockets;
    int send = 0, receive = 0;
    NTPserver* serv;
    SettingsForm* Settings;
    QVector <NTPserver*> ntp_servers;
    QHostAddress groupAddress;
};

#endif
