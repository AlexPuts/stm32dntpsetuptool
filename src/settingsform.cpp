#include "settingsform.h"
#include "ui_settingsform.h"
#include "ntpserver.h"


SettingsForm::SettingsForm(NTPserver* serv) :
    ui(new Ui::SettingsForm)
{

    server = serv;
    ntp_udpSocket = server->ntp_udpSocket_ptr;


    ui->setupUi(this);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->ipAddressLineEdit->setValidator(ipValidator);

    ui_combo_boxes.push_back(ui->ntpComboBox);
    ui_combo_boxes.push_back(ui->mbtcpComboBox);
    ui_combo_boxes.push_back(ui->melsecComboBox);
    ui_combo_boxes.push_back(ui->gpsComboBox);

    for(int i = 0; i < ui_combo_boxes.size(); i++)
    {
        ui_combo_boxes.at(i)->addItem("Disabled");
        ui_combo_boxes.at(i)->addItem("Enabled");
    }
    bool get_ntp_protocol();
    bool get_mbtcp_protocol();
    bool get_melsec_protocol();
    ui->ntpComboBox->setCurrentIndex(serv->get_ntp_protocol()&&1);
    ui->mbtcpComboBox->setCurrentIndex(serv->get_mbtcp_protocol()&&1);
    ui->melsecComboBox->setCurrentIndex(serv->get_melsec_protocol()&&1);
    ui->gpsComboBox->setCurrentIndex(serv->get_gps_enabled()&&1);
    ui->generalInfoTextEdit->append("Current IP:" + serv->get_ip_current_s());
    ui->generalInfoTextEdit->append("EEPROM IP:" + serv->get_ip_eeprom_s());
    ui->generalInfoTextEdit->append("Serial:" + QString::number(serv->get_serial()));
    ui->generalInfoTextEdit->append("Uptime: " + QString::number(serv->get_uptime()))
    /*+ " days"+QString::number((serv->get_uptime()/3600)) + "hours" +'\n')*/;
    ui->ipAddressLineEdit->setText(serv->get_ip_current_s());
    connect(ui->setTimePushButton,SIGNAL(clicked()),this,SLOT(slotSetTime()));
    connect(ui->sendPushButton,SIGNAL(clicked()),this,SLOT(slotSendSettings()));


/*

    qint32 get_manufacture_date();
    qint32 get_system_time();
    qint32 get_signature();
    QString get_title();
    QString get_gps_chip();
    QString get_main_chip();
    QString get_eth_chip();
    QString get_firmware_version();
    QString get_ip_current_s();
    QString get_ip_eprom_s();
*/

}
SettingsForm::SettingsForm() :
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
}
SettingsForm::~SettingsForm()
{
    delete ui;
}
void SettingsForm::slotSetTime()
{
    QDateTime currentDateTime =QDateTime::currentDateTime();
    qint32 unixtime = currentDateTime.toTime_t();
    QByteArray datagram(10, 0);
    memcpy(datagram.data(),"sttime",6);
    memcpy(datagram.data()+6,&unixtime,4);
    for(int i = 0; i < datagram.length(); i++) datagram.data()[i]^=60;
    server->ntp_udpSocket_ptr->writeDatagram(datagram.data(),datagram.size(),QHostAddress(server->get_ip_current_s()), 7800);
}

void SettingsForm::slotSendSettings()
{
    qint32 current_ip  = 0;
    long s1=0,s2=0,s3=0,s4=0;
    uint8_t byte_true = true, byte_false = false;
    QByteArray datagram(14, 0);
    memcpy(datagram.data(),"stsets",6);


    QStringList server_octets = ui->ipAddressLineEdit->text().split(".");
    s1 = server_octets.at(0).toLong();
    s2 = server_octets.at(1).toLong();
    s3 = server_octets.at(2).toLong();
    s4 = server_octets.at(3).toLong();
    current_ip = (s4 << 24) | (s3 << 16) | (s2 << 8) | s1;


    memcpy(datagram.data()+6,&current_ip,4);
    memcpy(datagram.data()+6,&byte_true,1);
    memcpy(datagram.data()+7,&byte_true,1);
    memcpy(datagram.data()+8,&byte_false,1);
    memcpy(datagram.data()+9,&byte_true,1);
    memcpy(datagram.data()+10,&current_ip,4);

    for(int i = 0; i < datagram.length(); i++) datagram.data()[i]^=60;

   server->ntp_udpSocket_ptr->
            writeDatagram(datagram.data(),datagram.size(),
            QHostAddress(server->get_ip_current_s()), 7800);


}
