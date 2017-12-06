#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include "ntpserver.h"
#include <QComboBox>
#include <QtNetwork>
#include <QDateTime>

class QUdpSocket;
namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(NTPserver* = 0);
    explicit SettingsForm();
    ~SettingsForm();

private:
    Ui::SettingsForm *ui;
    QVector<QComboBox*> ui_combo_boxes;
    NTPserver* server;
    QUdpSocket* ntp_udpSocket;
    QUdpSocket* bcast_udpSocket;
public slots:
    void slotSetTime();
    void slotSendSettings();
};

#endif // SETTINGSFORM_H
