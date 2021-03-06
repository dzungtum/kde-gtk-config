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

#include "appearancegtk3.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>
#include <KSharedConfig>
#include <KConfigGroup>

#undef signals
#include <gio/gio.h>
#include <gtk/gtk.h>
#define signals Q_SIGNALS

QStringList AppearanceGTK3::installedThemes() const
{
    QFileInfoList availableThemes;
    foreach(const QString& themesDir, QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "themes", QStandardPaths::LocateDirectory)) {
        QDir root(themesDir);
        availableThemes += root.entryInfoList(QDir::NoDotAndDotDot|QDir::AllDirs);
    }

    //Also show the user-installed themes
    QDir user(QDir::homePath()+"/.themes");
    availableThemes += user.entryInfoList(QDir::NoDotAndDotDot|QDir::AllDirs);

    //we just want actual themes
    QStringList themes;

    // Check that the theme contains a gtk-3.* subdirectory
    QStringList gtk3SubdirPattern{QStringLiteral("gtk-3.*")};
    for(QFileInfoList::const_iterator it=availableThemes.constBegin(); it!=availableThemes.constEnd(); ++it) {
        QDir themeDir(it->filePath());
        if(!themeDir.entryList(gtk3SubdirPattern, QDir::Dirs).isEmpty())
            themes += it->filePath();
    }

    return themes;
}

bool AppearanceGTK3::saveSettings(const KSharedConfig::Ptr& file) const
{
    KConfigGroup group(file, "Settings");

    group.writeEntry("gtk-font-name", m_settings["font"]);
    group.writeEntry("gtk-theme-name", m_settings["theme"]);
    group.writeEntry("gtk-icon-theme-name", m_settings["icon"]);
    group.writeEntry("gtk-fallback-icon-theme", m_settings["icon_fallback"]);
    group.writeEntry("gtk-cursor-theme-name", m_settings["cursor"]);
    group.writeEntry("gtk-toolbar-style", m_settings["toolbar_style"]);
    group.writeEntry("gtk-menu-images", m_settings["show_icons_menus"]);
    group.writeEntry("gtk-button-images", m_settings["show_icons_buttons"]);
    group.writeEntry("gtk-primary-button-warps-slider", m_settings["primary_button_warps_slider"]);
    group.writeEntry("gtk-application-prefer-dark-theme", m_settings["application_prefer_dark_theme"]);

    const bool sync = group.sync();
    Q_ASSERT(sync);
    return true;
}

bool AppearanceGTK3::loadSettings(const KSharedConfig::Ptr& file)
{
    KConfigGroup group(file, "Settings");

    if (!file || !group.isValid()) {
        qWarning() << "Cannot open the GTK3 config file" << file;
        return false;
    }

    m_settings = QMap<QString, QString> {
        {"toolbar_style", "GTK_TOOLBAR_ICONS"},
        {"show_icons_buttons", "0"},
        {"show_icons_menus", "0"},
        {"primary_button_warps_slider", "false"},
        {"application_prefer_dark_theme", "false"}
    };

    m_settings["theme"] = group.readEntry("gtk-theme-name");
    m_settings["icon"] = group.readEntry("gtk-icon-theme-name");
    m_settings["icon_fallback"] = group.readEntry("gtk-fallback-icon-theme");
    m_settings["cursor"] = group.readEntry("gtk-cursor-theme-name");
    m_settings["font"] = group.readEntry("gtk-font-name");
    m_settings["toolbar_style"] = group.readEntry("gtk-toolbar-style");
    m_settings["show_icons_buttons"] = group.readEntry("gtk-button-images");
    m_settings["show_icons_menus"] = group.readEntry("gtk-menu-images");
    m_settings["primary_button_warps_slider"] = group.readEntry("gtk-primary-button-warps-slider");
    m_settings["application_prefer_dark_theme"] = group.readEntry("gtk-application-prefer-dark-theme");
    for(auto it = m_settings.begin(); it != m_settings.end(); ) {
        if (it.value().isEmpty())
            it = m_settings.erase(it);
        else
            ++it;
    }
    return true;
}

QString AppearanceGTK3::configFileName() const
{
    return QStringLiteral("gtk-3.0/settings.ini");
}

QString AppearanceGTK3::defaultConfigFile() const
{
    QString root = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    if(root.isEmpty())
        root = QFileInfo(QDir::home(), ".config").absoluteFilePath();

    return root + '/' + configFileName();
}

bool AppearanceGTK3::getApplicationPreferDarkTheme() const
{
    return m_settings["application_prefer_dark_theme"] == "1" || m_settings["application_prefer_dark_theme"] == "true";
}

void AppearanceGTK3::setApplicationPreferDarkTheme(const bool& enable)
{
    m_settings["application_prefer_dark_theme"] = enable ? "true" : "false";
}

bool AppearanceGTK3::saveSettings(const QString& file) const
{
    auto cfg = KSharedConfig::openConfig(file, KConfig::NoGlobals);
    return saveSettings(cfg);
}

bool AppearanceGTK3::loadSettings(const QString& path)
{
    auto cfg = KSharedConfig::openConfig(path, KConfig::NoGlobals);
    return loadSettings(cfg);
}

bool AppearanceGTK3::loadSettings()
{
    auto cfg = KSharedConfig::openConfig(configFileName(), KConfig::NoGlobals);
    return loadSettings(cfg);
}

bool AppearanceGTK3::saveSettings() const
{
    // FIXME kdebz#387417
    // We should maybe use GSettings everywhere in future, but at this moment we
    // need this to have this configuration available in sandboxed applications which
    // is only possible through dconf
    gtk_init(nullptr, nullptr);

    g_autoptr(GSettings) gsettings = g_settings_new("org.gnome.desktop.interface");
    g_settings_set_string(gsettings, "gtk-theme", m_settings["theme"].toUtf8().constData());
    g_settings_set_string(gsettings, "icon-theme", m_settings["icon"].toUtf8().constData());
    g_settings_set_string(gsettings, "cursor-theme", m_settings["cursor"].toUtf8().constData());
    g_settings_set_string(gsettings, "font-name", m_settings["font"].toUtf8().constData());

    QString toolbarStyle;
    if (m_settings["toolbar_style"] == QStringLiteral("GTK_TOOLBAR_ICONS")) {
        toolbarStyle = QStringLiteral("icons");
    } else if (m_settings["toolbar_style"] == QStringLiteral("GTK_TOOLBAR_TEXT")) {
        toolbarStyle = QStringLiteral("text");
    } else if (m_settings["toolbar_style"] == QStringLiteral("GTK_TOOLBAR_BOTH")) {
        toolbarStyle = QStringLiteral("both");
    } else if (m_settings["toolbar_style"] == QStringLiteral("GTK_TOOLBAR_BOTH_HORIZ")) {
        toolbarStyle = QStringLiteral("both-horiz");
    }

    g_settings_set_string(gsettings, "toolbar-style", toolbarStyle.toUtf8().constData());

    g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", getApplicationPreferDarkTheme(), nullptr);

    auto cfg = KSharedConfig::openConfig(configFileName(), KConfig::NoGlobals);
    return saveSettings(cfg);
}
