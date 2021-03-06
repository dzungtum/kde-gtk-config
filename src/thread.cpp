/* KDE GTK Configuration Module
 * 
 * Copyright 2011 José Antonio Sanchez Reynaga <joanzare@gmail.com>
 * Copyright 2011 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "thread.h"
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QDirIterator>
#include <QDebug>
#include <QUrl>
#include <kio/deletejob.h>
#include <KTar>

Thread::Thread(const QString& accion)
    : action(accion)
{}

void Thread::start()
{
    emit started();

    bool success = false;
    if(urlPackage.isEmpty()) {
        qWarning() << "*** ERROR: There's nothing to do";
    } else if(action == "icon") {
//         qDebug() << "Installing icons theme";
        success = Installer::installIconTheme(urlPackage);
    } else if(action == "theme") {
//         qDebug() << "Installing GTK theme";
        success = Installer::installTheme(urlPackage);
    }
    setError(success);
    emitResult();
}

void Thread::setUrlPackage(const QString& package)
{
    urlPackage = package;
}

bool Thread::isSuccess() const
{
    return error()==0;
}

void ThreadAnalisysTheme::start()
{
    bool success = false;
//     qDebug()<< "File to install" << packageTheme;

    KTar package(packageTheme);
    if(!package.open(QIODevice::ReadOnly)) {
//         qDebug() << "ERROR extracting the package theme" << packageTheme;
        setError(1);
        emitResult();
        return;
    }
//     qDebug() << "** EXTRACTING ICONS TO A TEMPORAL FOLDER";
    //We proceed unpacking the package in a temporal directory
    QDir temporal(QDir::tempPath()+"/CGC/theme");

    //Make sure it's already created
    temporal.mkpath(temporal.path());
    
    package.directory()->copyTo(temporal.path());
    
    // Package extracted in the temp dir. Now we want to know the name
    QString folder=temporal.entryList(QDir::AllDirs|QDir::NoDotAndDotDot).first();

//     qDebug() << "FOUND THEME FOLDER = " << folder;
//     qDebug() << "\n******* THEME " << temporal.path()+'/'+folder;

    //We know the path of the folder to analyze
    QDirIterator it(temporal.path()+'/'+folder);

    while(it.hasNext()) {

        QString file = it.next();
//         qDebug() << "trying file" << file;

        if(it.fileName()=="gtkrc") {
//             qDebug() << "FILE : " << file;
            success = true;
            break;
        }
    }

    QUrl tempUrl = QUrl::fromLocalFile(temporal.path());
//     qDebug() << "Deleting temps. Successful:" << success;
    if(!KIO::del(tempUrl, KIO::HideProgressInfo)->exec())
        qWarning() << "There was not cleanning";
//     else
//         qDebug() << "Cleanning complete" << temporal.path();
    
    if(!success)
        setError(2);
    emitResult();
}

void ThreadAnalisysTheme::setPackageTheme(const QString& theme)
{
    packageTheme = theme;
}

bool ThreadAnalisysTheme::isSuccess() const
{
    return error()==0;
}

void ThreadAnalisysThemeIcon::start()
{
    bool success = false;
//     qDebug()<< "*************** GTK THEME INSTALLATION";
//     qDebug()<< "File to install" << packageTheme;

    KTar package(packageTheme);
    if(!package.open(QIODevice::ReadOnly)) {
        qWarning() << "ERROR extracting the package theme" << packageTheme;
        return;
    }
//     qDebug() << "** EXTRACTING ICONS TO A TEMPORAL FOLDER";
    QDir temporal(QDir::tempPath()+"/CGC/icon");
    temporal.mkpath(temporal.path());

    package.directory()->copyTo(temporal.path());

    //archive extracted in the temp directory
    QString folder= temporal.entryList(QDir::AllDirs|QDir::NoDotAndDotDot).first();
//     qDebug() << "FOUND THEME FOLDER = " << folder;
//     qDebug() << "\n******* THEME " << temporal.path()+'/'+folder;

    QDirIterator it(temporal.path()+'/'+folder);

    while(it.hasNext()) {

        QString file = it.next();

        if(it.fileName()=="index.theme") {
            //archivo index.theme
//             qDebug() << "FILE : " << file;
            success = true;
            break;
        }

    }

    QUrl tempUrl = QUrl::fromLocalFile(temporal.path());
//     qDebug() << "Deleting temps. Successful:" << success;
    if(KIO::del(tempUrl, KIO::HideProgressInfo)->exec()) {
        qWarning() << "Cleaning was not succesful";
    }
//     else
//         qDebug() << "Cleanning complete." << temporal.path();
    if(!success)
        setError(2);
    emitResult();
}

void ThreadAnalisysThemeIcon::setPackageTheme(const QString& theme)
{
    packageTheme = theme;
}

bool ThreadAnalisysThemeIcon::isSuccess()
{
    return error()==0;
}

bool ThreadErase::isSuccess()
{
    return error()==0;
}

void ThreadErase::setThemeForErase(const QString& theme)
{
    Q_ASSERT(!theme.isEmpty());
    themeForErase = theme;
}

void ThreadErase::start()
{
    KIO::DeleteJob* job = KIO::del(QUrl::fromLocalFile(themeForErase), KIO::HideProgressInfo);
    connect(job, SIGNAL(finished(KJob*)), SLOT(deleted(KJob*)));
}

void ThreadErase::deleted(KJob* job)
{
    setError(job->error());
    setErrorText(job->errorText());
    emitResult();
}
