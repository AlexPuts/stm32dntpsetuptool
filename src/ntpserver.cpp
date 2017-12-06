#include "ntpserver.h"
NTPserver::NTPserver(){}
NTPserver::NTPserver(QByteArray &datagram, QUdpSocket* udpSocket)
{
    ntp_udpSocket_ptr = udpSocket;
    for(int i = 0; i < 64; i++)
    {
        *(datagram.data()+i)^=60;
        if(i<13) main_chip.push_back(*(datagram.data()+i));
    }
    memcpy(&ip_eeprom,datagram.data()+14,4);
    memcpy(&ip_current,datagram.data()+19,4);
    memcpy(&uptime,datagram.data()+23,4);

    if(*(datagram.data()+27)) ntp_protocol = true;
    if(*(datagram.data()+28)) mbtcp_protocol = true;
    if(*(datagram.data()+29)) melsec_protocol = true;
    if(*(datagram.data()+30)) gps_enabled = true;

    memcpy(&serial,datagram.data()+31,4);
    const int NBYTES = 4;
    for(int i = 0 ; i < NBYTES ; i++)
    {
        ip_eeprom_s+= QString::number((ip_eeprom >> (i * 8))& 0xFF);
        ip_current_s+= QString::number((ip_current >> (i * 8))& 0xFF);
        if(i!=3)
        {
            ip_eeprom_s+='.';
            ip_current_s+='.';
        }
    }
    title+=  main_chip + "@"+ ip_current_s;
    signature = system_time^serial^uptime;
}
bool NTPserver::get_ntp_protocol(){return ntp_protocol;}
bool NTPserver::get_mbtcp_protocol(){return mbtcp_protocol;}
bool NTPserver::get_melsec_protocol(){return melsec_protocol;}
bool NTPserver::get_gps_enabled() {return gps_enabled;}
qint32 NTPserver::get_ip_current() {return ip_current;}
qint32 NTPserver::get_ip_eeprom(){return ip_eeprom;}
qint32 NTPserver::get_serial(){return serial;}
qint32 NTPserver::get_manufacture_date(){return manufacture_date;}
qint32 NTPserver::get_uptime(){return uptime;}
qint32 NTPserver::get_system_time(){return system_time;}
qint32 NTPserver::get_signature(){return signature;}
QString NTPserver::get_ip_eeprom_s(){return ip_eeprom_s;}
QString NTPserver::get_title(){return title;}
QString NTPserver::get_gps_chip(){return gps_chip;}
QString NTPserver::get_main_chip(){return main_chip;}
QString NTPserver::get_eth_chip(){return eth_chip;}
QString NTPserver::get_firmware_version(){return firmware_version;}
QString NTPserver::get_ip_current_s(){return ip_current_s;}
