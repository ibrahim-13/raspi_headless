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

#include "oscheck.h"
#include "datastore.h"
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

/*!
 * \brief OSCheck::OSCheck
 * \param parent - Parent variable
 * \details Constructor for the class
 */

OSCheck::OSCheck(QObject *parent) : QThread(parent)
{
}

/*!
 * \brief OSCheck::OSCheck
 * \param parent - Parent variable
 * \param dirName - Directory location to search the files in
 * \details Constructor for the class
 */

OSCheck::OSCheck(QObject *parent, QString dirName)  : QThread(parent)
{
    this->directory = dirName;
}

/*!
 * \brief OSCheck::run
 * \details Overriden functin from QThread. Checks for specific files in the given directory and emits signal with status.
 */

void OSCheck::run()
{
    QDir dir(this->directory);
    QFileInfoList list = dir.entryInfoList();
    QFileInfoList::iterator it;
    for(it = list.begin(); it != list.end(); it++)
    {
        if(it->fileName() == QString(TextData::OS_INSTALLATION_FILE_NAME))
        {
            emit updIsOSInstalled(true);
            return;
        }
    }
    emit updIsOSInstalled(false);
}
