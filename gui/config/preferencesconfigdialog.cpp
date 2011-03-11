/*
 * Copyright 2011  Stefan Böhmann <kde@hilefoks.org>
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
#include "preferencesconfigdialog.h"
#include "preferencesconfigwidget.h"

#include <QCoreApplication>


PreferencesConfigDialog::PreferencesConfigDialog( Knipptasch::Preferences *pref, QWidget *parent )
    : ConfigDialog( parent )
{
    setWindowTitle( tr( "Configure Knipptasch - %1" ).arg( QCoreApplication::applicationName() ) );

    ConfigWidget *old = configWidget();
    setConfigWidget( new PreferencesConfigWidget( pref, this ) );
    delete old;
}


PreferencesConfigDialog::~PreferencesConfigDialog()
{
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
