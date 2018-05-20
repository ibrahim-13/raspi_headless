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

#include "exportsettings.h"
#include "datastore.h"
#include <QIODevice>
#include <QFile>
#include <QTextStream>

/*!
 * \brief ExportSettings::ExportSettings
 * \param parent - Parent variable
 * \param dir - Directory where to export the settings to.
 * \details Constructor for the class
 */

ExportSettings::ExportSettings(QObject *parent, QString dir) : QThread(parent)
{
    this->export_dir = dir;
    this->hasSSH = false;
    this->hasWiFi = false;
}

/*!
 * \brief ExportSettings::setEnableSSH
 * \param arg1 - bool variable to enable or disable SSH functionality
 * \details Enable or disable SSH
 */

void ExportSettings::setEnableSSH(bool arg1)
{
    this->hasSSH = arg1;
}

/*!
 * \brief ExportSettings::setWiFiData
 * \param ssid - SSID of the WiFi network
 * \param passwd - Password of the WiFi network
 * \param net_type - Network security type of the WiFi network
 * \details Set the WiFi informations.
 */

void ExportSettings::setWiFiData(QString ssid, QString passwd, WIFI_NETWORK_TYPES net_type)
{
    this->hasWiFi = true;
    this->wifi_ssid = ssid;
    this->wifi_passwd = passwd;
    this->wifi_net_type = net_type;
}

/*!
 * \brief ExportSettings::exportSettings
 * \details Export setting to the defined directory. Requires all params related to WiFi to be valid. If one of them is empty, it returns error and aborts the export operation.
 */

void ExportSettings::exportSettings()
{
    if(hasSSH)
    {
        QFile ssh_file(this->export_dir + TextData::FILE_NAME_SSH);
        if(ssh_file.open(QIODevice::WriteOnly | QIODevice::ReadWrite))
            ssh_file.flush();
        else
        {
            emit exportOpResult(ExportSettings::EXPORT_ERR_CREATE);
            return;
        }
    }

    if(hasWiFi)
    {
        if(this->wifi_ssid == "" || this->wifi_passwd == "")
        {
            exportOpResult(ExportSettings::EXPORT_NO_WIFI_DAT);
            return;
        }

        QFile wifi_file(this->export_dir + TextData::FILE_NAME_WIFI_CONF);

        if(wifi_file.exists())
            wifi_file.remove();

        if(wifi_file.open(QIODevice::WriteOnly | QIODevice::ReadWrite))
        {
            QTextStream stream(&wifi_file);
            stream << QString(TextData::WIFI_CONF_FILE_P1_L1) + QString(TextData::WIFI_CONF_FILE_P1_L2) + QString(TextData::WIFI_CONF_FILE_P1_L3) +
                      QString(TextData::WIFI_CONF_FILE_P1_L4) + QString(TextData::WIFI_CONF_FILE_P1_L5) + QString(TextData::WIFI_CONF_FILE_P1_L6) +
                      this->wifi_ssid +
                      QString(TextData::WIFI_CONF_FILE_P2_L1) + QString(TextData::WIFI_CONF_FILE_P2_L2) + QString(TextData::WIFI_CONF_FILE_P2_L3) +
                      this->wifi_passwd +
                      QString(TextData::WIFI_CONF_FILE_P3_L1) + QString(TextData::WIFI_CONF_FILE_P3_L2) +
                      getWiFiTypeStr() +
                      QString(TextData::WIFI_CONF_FILE_P4_L1) + QString(TextData::WIFI_CONF_FILE_P4_L2);
        }
        else
        {
            emit exportOpResult(ExportSettings::EXPORT_ERR_CREATE);
            return;
        }
    }

    emit exportOpResult(ExportSettings::EXPORT_OK);
}

/*!
 * \brief ExportSettings::getWiFiTypeStr
 * \return QString class containing the WiFi network type
 * \details Converts and returns the WiFi network secutiry type to QString format.
 */

QString ExportSettings::getWiFiTypeStr()
{
    switch(this->wifi_net_type)
    {
    case WIFI_TYPE_WPAPSK:
        return TextData::WIFI_NET_TYPE_WPAPSK;
    case WIFI_TYPE_ALL:
        return TextData::WIFI_NET_TYPE_ALL;
    case WIFI_TYPE_NONE:
        return TextData::WIFI_NET_TYPE_NONE;
    default:
        return TextData::WIFI_NET_TYPE_ALL;
    }
}

/*!
 * \brief ExportSettings::run
 * \details Overriden function from QThread. This function is run in a seperate thread.
 */

void ExportSettings::run()
{
    exportSettings();
}
