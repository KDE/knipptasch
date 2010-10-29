/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "preferences.h"

#if defined(HAVE_KDE)
#include <KGlobal>
#include <KConfig>
#include <KConfigGroup>
#else
#include <QSettings>
#endif

#include <QDebug>


Preferences* Preferences::self()
{
    static Preferences pref;
    return &pref;
}


Preferences::Preferences()
{

}


Preferences::~Preferences()
{

}


void Preferences::sync()
{
#if defined(HAVE_KDE)
    KConfig config;
    config.sync();
#else
    QSettings settings;
    settings.sync();
#endif
}


QVariant Preferences::value(const QString &group, const QString &key, const QVariant &defaultValue) const
{
    QVariant value;

#if defined(HAVE_KDE)
    KConfig config;
    KConfigGroup configgroup( &config, group.isEmpty() ? "General" : group );
    value = configgroup.readEntry( key, defaultValue );
#else
    QSettings settings;
    QString k = group.isEmpty() ? key : QString( "%1/%2" ).arg( group ).arg( key );
    value = settings.value( k, defaultValue );
#endif

    return value;
}


void Preferences::setValue(const QString &group, const QString &key, const QVariant &val)
{
#if defined(HAVE_KDE)
    KConfig config;
    KConfigGroup configgroup( &config, group.isEmpty() ? "General" : group );
    configgroup.writeEntry( key, val );

    configgroup.config()->sync();
#else
    QSettings settings;
    QString k = group.isEmpty() ? key : QString( "%1/%2" ).arg( group ).arg( key );
    settings.setValue( k, val );
#endif
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
