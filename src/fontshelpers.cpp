/* KDE GTK Configuration Module
 * 
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

#include "fontshelpers.h"
#include <QFontDatabase>
#include <QRegularExpression>
#include <QDebug>
#include <QStringList>

QString fontToString(const QFont& f)
{
    return f.family() + ' ' + f.styleName() + ' ' + QString::number(f.pointSize());
}

QFont stringToFont(const QString& font)
{
    QFontDatabase fdb;
    QString fontFamily;
    int familyIdx=-1;
    QStringList allFamilies = fdb.families();
    for(int idx=font.indexOf(' '); idx<font.size() && idx>=0; idx=font.indexOf(' ', idx+1)) {
        QString testFont = font.left(idx);
        if(allFamilies.contains(testFont)) {
            fontFamily = testFont;
            familyIdx = idx;
        }
    }

    QFont f;
    f.setFamily(fontFamily);
    QRegularExpression fontRx(QStringLiteral(" (.*) +([0-9]+)$"));
    QRegularExpressionMatch match = fontRx.match(font, familyIdx);
    if (match.isValid()) {
        QString fontStyle = match.captured(1).trimmed();
        int fontSize = match.captured(2).toInt();
        f.setStyleName(fontStyle);
        f.setPointSize(fontSize);
    } else {
        qWarning() << "Couldn't figure out syle and size" << font;
    }
    return f;
}
