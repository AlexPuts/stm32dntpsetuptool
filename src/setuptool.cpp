/*https://alexputz.github.io/projects/*/

#include "setuptool.h"
#include "ui_setuptool.h"
#include "ntpserver.h"
#include <QtWidgets>
#include <QtNetwork>
#define SCAN_ATTEMPTS 5

setuptool::setuptool(QMainWindow *parent)
    : QMainWindow(parent),
      ui(new Ui::setuptool)
{
    ui->setupUi(this);
    serv = new NTPserver();
    ui->readytostartLabel->setText("Ready to scan");
    ui->receiveLabel->setText("");
    timer = new QTimer(this);



    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
      if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
       {
         udpSocket = new QUdpSocket(this);
         udpSocket->abort();
         udpSocket->bind(QHostAddress::Broadcast,4800, QUdpSocket::ShareAddress);
         udpSocket->joinMulticastGroup(groupAddress);
         broadcast_sockets.push_back(udpSocket);
         udpSocket = new QUdpSocket(this);
         udpSocket->abort();
         udpSocket->bind(QHostAddress::Broadcast,7800, QUdpSocket::ShareAddress);
         udpSocket->joinMulticastGroup(groupAddress);
         receive_sockets.push_back(udpSocket);
        }
    }
    ui->scanprogressBar->setMaximum(SCAN_ATTEMPTS);

    messageNo = 1;
    connect(ui->scanPushButton, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    connect(ui->quitPushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
    connect(timer, SIGNAL(timeout()), this, SLOT(processPendingDatagrams()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openSettings(QListWidgetItem*)));

    setWindowTitle(tr("Stm32duino ntp setuptool"));}

void setuptool::startBroadcasting()
{
    ui->scanPushButton->setText("Stop");
    ui->readytostartLabel->setText("Scanning");
    ntp_servers.clear();
    ui->listWidget->clear();
    send = messageNo = receive = 0;
    ui->scanprogressBar->setValue(0);
    disconnect(ui->scanPushButton, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    connect(ui->scanPushButton, SIGNAL(clicked()), this, SLOT(stopBroadcasting()));
    timer->start(1000);
}

void setuptool::stopBroadcasting()
{
    ui->scanPushButton->setText("Start");
    ui->readytostartLabel->setText("Ready to scan");
    ui->scanprogressBar->setValue(SCAN_ATTEMPTS);
    disconnect(ui->scanPushButton, SIGNAL(clicked()), this, SLOT(stopBroadcasting()));
    connect(ui->scanPushButton, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    timer->stop();
}

void setuptool::broadcastDatagram()
{
    if(send<SCAN_ATTEMPTS)
    {
    foreach (QUdpSocket* udpSocket , broadcast_sockets)
    {
            QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
            udpSocket->writeDatagram(datagram.data(),datagram.size(),QHostAddress("255.255.255.255"), 4800);
    }
    ui->scanprogressBar->setValue(ui->scanprogressBar->value()+1);
    messageNo++;
    send++;
   }
    else stopBroadcasting();
}

void setuptool::processPendingDatagrams()
{
    foreach (QUdpSocket* udpSocket , receive_sockets)
    {
    if(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        receive++;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        ui->receiveLabel->setText("Receive: " + QString::number(receive));
        *serv = NTPserver(datagram,udpSocket);
        bool found = false;
        for(int i = 0; i <ntp_servers.size();i++)
        {
            if(serv->get_signature()==ntp_servers.at(i)->get_signature())
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            ui->listWidget->addItem(serv->get_title());
            ntp_servers.push_back(serv);
        }
    }
    }
}
void setuptool::openSettings(QListWidgetItem *item)
{

    for(int i = 0; i <ntp_servers.size();i++)
    {
        if(item->text()==ui->listWidget->item(i)->text())
        {

            Settings=new SettingsForm(ntp_servers[i]);
            break;
        }
    }

    Settings->show();
    Settings->raise();
}

