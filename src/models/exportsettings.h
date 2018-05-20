/**
Copyright (C) 2018 MD. Ibrahim Khan

Project Name:
Author: MD. Ibrahim Khan
Author's Email: ib.arshad777@gmail.com

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of the contributors may
   be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTIONS) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#ifndef EXPORTSETTINGS_H
#define EXPORTSETTINGS_H
#include <QObject>
#include <QString>
#include <QThread>

class ExportSettings : public QThread
{
    Q_OBJECT

public:
    enum WIFI_NETWORK_TYPES { WIFI_TYPE_WPAPSK, WIFI_TYPE_ALL, WIFI_TYPE_NONE };
    enum EXPORT_RESULT { EXPORT_OK, EXPORT_ERR_CREATE, EXPORT_NO_WIFI_DAT };
    explicit ExportSettings(QObject *parent, QString dir);
    void setEnableSSH(bool arg1);
    void setWiFiData(QString ssid, QString passwd, WIFI_NETWORK_TYPES net_type);
    void run() override;

signals:
    void exportOpResult(int result);

private:
    bool hasSSH;
    bool hasWiFi;
    QString export_dir;
    QString wifi_ssid;
    QString wifi_passwd;
    WIFI_NETWORK_TYPES wifi_net_type;
    void exportSettings();
    QString getWiFiTypeStr();
};

#endif // EXPORTSETTINGS_H
