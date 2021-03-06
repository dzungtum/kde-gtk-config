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

#ifndef THREAD_H
#define THREAD_H

#include <installer.h>
#include <kjob.h>

/**
 Used so that we don't block the GUI when we install the themes
  */
class Thread: public KJob
{
Q_OBJECT
public:
    Thread(const QString& );
    void start() override;
    
    ///sets the icon path
    void setUrlPackage(const QString& );
    
    ///@returns whether it was successful
    bool isSuccess() const;

signals:
    void started();

private:
    QString action;
    QString urlPackage;
};

class ThreadAnalisysTheme: public KJob
{
public:
    void start() override;
    
    ///@returns whether the theme is correct
    bool isSuccess() const;
    
    ///sets the theme to analyze
    void setPackageTheme(const QString& );
private:
    QString packageTheme;
};

class ThreadAnalisysThemeIcon: public KJob
{
public:
    void start() override;
    
    ///@returns whether the theme is correct
    bool isSuccess();
    
    ///sets what theme to use
    void setPackageTheme(const QString& );
private:
    QString packageTheme;
};

/**
  Thread to erase the icons
  */
class ThreadErase: public KJob
{
Q_OBJECT
public:
    void start() override;
    
    bool isSuccess();
    void setThemeForErase(const QString& theme );
private:
    QString themeForErase;

public slots:
    void deleted(KJob*);
};

#endif // THREAD_H


