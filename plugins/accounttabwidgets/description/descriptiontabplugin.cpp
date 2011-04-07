/*
 * Copyright 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#include "descriptiontabplugin.h"

#include <Knipptasch/Account>
#include <Knipptasch/Posting>
#include <Knipptasch/Money>
#include <Knipptasch/Category>

#include <compat/iconloader.h>

#include <QPixmap>
#include <QInputDialog>
#include <QDate>

#include <QDebug>



DescriptionTabPlugin::DescriptionTabPlugin( QObject *parent )
    : QObject( parent )
{
    qsrand( QTime::currentTime().msec() );
}


bool DescriptionTabPlugin::isEnabledByDefault() const
{
    return true;
}


QByteArray DescriptionTabPlugin::identifier() const
{
    return "descriptiontabplugin";
}


QString DescriptionTabPlugin::shortName() const
{
    return tr( "Description" );
}


QString DescriptionTabPlugin::name() const
{
    return tr( "Description Tab Plugin" );
}


QPixmap DescriptionTabPlugin::icon() const
{
    return BarIcon( "format-text-blockquote" );
}


QString DescriptionTabPlugin::description() const
{
    return tr( "TODO" );
}


QString DescriptionTabPlugin::author() const
{
    return tr( "Stefan Böhmann" );
}


QByteArray DescriptionTabPlugin::version() const
{
    return "0.0.1";
}


AbstractAccountTabWidget *DescriptionTabPlugin::widget()
{
    return 0;
}


bool DescriptionTabPlugin::enable()
{
    return true;
}


bool DescriptionTabPlugin::disable()
{
    return true;
}


Q_EXPORT_PLUGIN2( "DescriptionTabPlugin", DescriptionTabPlugin );


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
