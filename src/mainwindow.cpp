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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datastore.h"
#include "workers/oscheck.h"
#include "models/exportsettings.h"

#include <QThread>
#include <QFileDialog>
#include <QMessageBox>

/*!
 * \brief MainWindow::MainWindow
 * \param parent Parent widget
 * \details Constructor for MainWindow
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->isOSInstalled = false;
    ui->comboBoxNetType->addItem(TextData::COMBOBOX_TXT_WPAPSK, ExportSettings::WIFI_TYPE_WPAPSK);
    ui->comboBoxNetType->addItem(TextData::COMBOBOX_TXT_NONE, ExportSettings::WIFI_TYPE_NONE);
    ui->comboBoxNetType->addItem(TextData::COMBOBOX_TXT_MULTI, ExportSettings::WIFI_TYPE_ALL);
}

/*!
 * \brief MainWindow::~MainWindow
 * \details Deconstructor for MainWindow
 */

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::on_pushButton_clicked
 * \details Executes when "Browse button is clicked
 */

void MainWindow::on_pushButton_clicked()
{
    QString folderLocation = QFileDialog::getExistingDirectory(this, TextData::QFILEDIAG_TITLE, nullptr, QFileDialog::ShowDirsOnly);
    if(folderLocation == "")
    {
        ui->labelDirectory->setText(TextData::QFILEDIAG_LBL_NOT_SELECTED);
        ResetAllInputState();
    }
    else
    {
        ui->labelDirectory->setText(TextData::QFILEDIAG_LBL_SELECTED + folderLocation);
        this->directory = folderLocation;
        StartOSCheck();
    }
}

/*!
 * \brief MainWindow::on_checkBoxEnableSSH_stateChanged
 * \param arg1 State of the QCheckBox
 * \details State change action for the checkbox
 */

void MainWindow::on_checkBoxEnableSSH_stateChanged(int arg1)
{
    switch(arg1)
    {
    case Qt::Checked:
        ui->checkBoxEnableSSH->setText(TextData::CHKBOX_ENABLED);
        break;
    case Qt::Unchecked:
        ui->checkBoxEnableSSH->setText(TextData::CHKBOX_DISABLED);
        break;
    }
}

/*!
 * \brief MainWindow::on_checkBoxEnableWiFi_stateChanged
 * \param arg1 State of the QCheckBox
 * \details State change action for the checkbox
 */

void MainWindow::on_checkBoxEnableWiFi_stateChanged(int arg1)
{
    switch(arg1)
    {
    case Qt::Checked:
        ui->checkBoxEnableWiFi->setText("");
        ui->checkBoxEnableWiFi->setText(TextData::CHKBOX_ENABLED);
        ChangeWiFiInputState(true);
        break;
    case Qt::Unchecked:
        ui->checkBoxEnableWiFi->setText("");
        ui->checkBoxEnableWiFi->setText(TextData::CHKBOX_DISABLED);
        ChangeWiFiInputState(false);
        break;
    }
}

/*!
 * \brief MainWindow::ChangeWiFiInputState
 * \param state State for the WiFi input fields. "true" for enabled, "false" for disabled
 * \details Changes WiFi input state
 */

void MainWindow::ChangeWiFiInputState(bool state)
{
    ui->lineEditSSID->setEnabled(state);
    ui->lineEditPasswd->setEnabled(state);
    ui->comboBoxNetType->setEnabled(state);
}

/*!
 * \brief MainWindow::ResetAllInputState
 * \details Resets input into default state
 */

void MainWindow::ResetAllInputState()
{
    ui->labelDirectory->setText(TextData::QFILEDIAG_LBL_NOT_SELECTED);
    ui->labelOS->setText(TextData::LBL_OS_NOT_INSTALLED);
    ui->checkBoxEnableSSH->setText(TextData::CHKBOX_DISABLED);
    ui->checkBoxEnableSSH->setChecked(false);
    ui->checkBoxEnableWiFi->setText(TextData::CHKBOX_DISABLED);
    ui->checkBoxEnableWiFi->setChecked(false);
    ChangeWiFiInputState(false);
}

/*!
 * \brief MainWindow::updIsOSInstalled
 * \param isInstalled State if the OS is installed
 * \details Updates the state of the OS installation in the selected directory
 */

void MainWindow::updIsOSInstalled(bool isInstalled)
{
    this->isOSInstalled = isInstalled;
    if(isInstalled)
        ui->labelOS->setText(TextData::LBL_OS_INSTALLED);
    else
        ui->labelOS->setText(TextData::LBL_OS_NOT_INSTALLED);
}

/*!
 * \brief MainWindow::StartOSCheck
 * \param dir Selected installation directory
 * \details Starts the OS installation detections in a seperate thread
 */

void MainWindow::StartOSCheck()
{
    OSCheck *tmp = new OSCheck(this, this->directory);
    QObject::connect(tmp, OSCheck::updIsOSInstalled, this, MainWindow::updIsOSInstalled);
    QObject::connect(tmp, OSCheck::finished, tmp, QObject::deleteLater);
    tmp->start();
}

/*!
 * \brief MainWindow::exportOpResult
 * \param result - Return code for the export operation. Returns enum value form ExportSettings::EXPORT_RESULT.
 * \details This function receives the status of the export operation.
 */

void MainWindow::exportOpResult(int result)
{
    switch(result)
    {
    case ExportSettings::EXPORT_OK:
        QMessageBox::information(this, TextData::QMSGBOX_RESULT_TITLE, TextData::QMSGBOX_RESULT_SUCCESS, QMessageBox::Ok);
        return;
    case ExportSettings::EXPORT_ERR_CREATE:
        QMessageBox::warning(this, TextData::QMSGBOX_RESULT_TITLE, TextData::QMSGBOX_RESULT_FILE_ERROR, QMessageBox::Ok);
        return;
    case ExportSettings::EXPORT_NO_WIFI_DAT:
        QMessageBox::warning(this, TextData::QMSGBOX_RESULT_TITLE, TextData::QMSGBOX_RESULT_WIFI_DAT_ERROR, QMessageBox::Ok);
        return;
    default:
        QMessageBox::warning(this, TextData::QMSGBOX_RESULT_TITLE, TextData::QMSGBOX_RESULT_OUTOFBOUND, QMessageBox::Ok);
    }
}

/*!
 * \brief MainWindow::on_pushButtonReset_clicked
 * \details Callback receiver for Reset button click
 */

void MainWindow::on_pushButtonReset_clicked()
{
    ResetAllInputState();
}

/*!
 * \brief MainWindow::on_pushButtonAuthor_clicked
 * \details Callback receiver for Author button click
 */

void MainWindow::on_pushButtonAuthor_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(TextData::QMSGBOX_AUTHOR_TITLE);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(TextData::QMSGBOX_AUTHOR_TEXT);
    msgBox.exec();
}

/*!
 * \brief MainWindow::on_pushButtonApply_clicked
 * \details Callback receiver for Apply button click
 */

void MainWindow::on_pushButtonApply_clicked()
{
    if(!this->isOSInstalled)
    {
        QMessageBox::warning(this, TextData::QMSGBOX_OS_INFO_TITLE, TextData::QMSGBOX_OS_INFO_NO_INSTALL, QMessageBox::Ok);
        return;
    }
    ExportSettings *tmp = new ExportSettings(this, this->directory);

    if(ui->checkBoxEnableSSH->isChecked())
        tmp->setEnableSSH(true);

    if(ui->checkBoxEnableWiFi->isChecked())
    {
        tmp->setWiFiData(ui->lineEditSSID->text(), ui->lineEditPasswd->text(), (ExportSettings::WIFI_NETWORK_TYPES)ui->comboBoxNetType->itemData(ui->comboBoxNetType->currentIndex()).toInt());
    }

    QObject::connect(tmp, ExportSettings::exportOpResult, this, MainWindow::exportOpResult);
    QObject::connect(tmp, ExportSettings::finished, tmp, ExportSettings::deleteLater);
    tmp->start();
}
