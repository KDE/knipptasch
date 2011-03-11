/*
 * Copyright 2010, 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#include "csvimportplugin.h"
#include "csvimportdialog.h"

#include "backend/account.h"

#include <compat/iconloader.h>
#include <QPointer>



CsvImportPlugin::CsvImportPlugin( QObject *parent )
    : QObject( parent )
{

}


bool CsvImportPlugin::isEnabledByDefault() const
{
    return true;
}


QByteArray CsvImportPlugin::identifier() const
{
    return "csv_import_plugin";
}


QString CsvImportPlugin::shortName() const
{
    return tr( "CSV Import" );
}


QString CsvImportPlugin::name() const
{
    return tr( "CSV Import Plugin" );
}


QPixmap CsvImportPlugin::icon() const
{
    return importActionIcon();
}


QString CsvImportPlugin::description() const
{
    return tr( "TODO" );
}


QString CsvImportPlugin::author() const
{
    return tr( "Stefan Böhmann" );
}


QByteArray CsvImportPlugin::version() const
{
    return "0.0.1";
}


QString CsvImportPlugin::importActionName() const
{
    return QObject::tr( "Import &CSV File..." );
}


QPixmap CsvImportPlugin::importActionIcon() const
{
    return BarIcon( "text-csv" );
}


Account *CsvImportPlugin::importAccount( QWidget *parent ) const
{
    QPointer<CsvImportDialog> dialog = new CsvImportDialog( parent );
    Account *account = 0;

    if( dialog->exec() == QDialog::Accepted ) {
        if( dialog ) {
            account = dialog->account();
        }
    }

    delete dialog;

    return account;
}


Q_EXPORT_PLUGIN2( "CsvImportPlugin", CsvImportPlugin );

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
