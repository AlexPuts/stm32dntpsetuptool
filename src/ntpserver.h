#ifndef NTPSERVER_H
#define NTPSERVER_H
#include <QtGlobal>
#include <QString>
#include <QtNetwork>
class QUdpSocket;

class NTPserver
{
public:
    NTPserver();
    NTPserver(QByteArray &datagram,QUdpSocket* udpSocket);

    bool get_ntp_protocol();
    bool get_mbtcp_protocol();
    bool get_melsec_protocol();
    bool get_gps_enabled();
    qint32 get_ip_current();
    qint32 get_ip_eeprom();
    qint32 get_serial();
    qint32 get_manufacture_date();
    qint32 get_uptime();
    qint32 get_system_time();
    qint32 get_signature();
    QString get_title();
    QString get_gps_chip();
    QString get_main_chip();
    QString get_eth_chip();
    QString get_firmware_version();
    QString get_ip_current_s();
    QString get_ip_eeprom_s();
    QUdpSocket* ntp_udpSocket_ptr;

private:
    bool ntp_protocol;
    bool mbtcp_protocol;
    bool melsec_protocol;
    bool gps_enabled;
    qint32 ip_current;
    qint32 ip_eeprom;
    qint32 serial;
    qint32 manufacture_date;
    qint32 uptime;
    qint32 system_time;
    qint32 signature;
    QString title;
    QString gps_chip;
    QString main_chip;
    QString eth_chip;
    QString firmware_version;
    QString ip_current_s;
    QString ip_eeprom_s;
};

#endif // NTPSERVER_H
