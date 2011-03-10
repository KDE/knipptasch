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
#include "csvexportplugin.h"
#include "csvexportdialog.h"

#include "compat/iconloader.h"

#include <QPointer>


CsvExportPlugin::CsvExportPlugin(QObject *parent)
  : QObject( parent )
{
}


bool CsvExportPlugin::isEnabledByDefault() const
{
    return true;
}


QByteArray CsvExportPlugin::identifier() const
{
    return "csv_export_plugin";
}


QString CsvExportPlugin::shortName() const
{
    return tr( "CSV Export" );
}


QString CsvExportPlugin::name() const
{
    return tr( "CSV Export Plugin" );
}


QPixmap CsvExportPlugin::icon() const
{
    return exportActionIcon();
}


QString CsvExportPlugin::description() const
{
    return tr( "TODO" );
}


QString CsvExportPlugin::author() const
{
    return tr( "Stefan Böhmann" );
}


QByteArray CsvExportPlugin::version() const
{
    return "0.0.1";
}


QString CsvExportPlugin::exportActionName() const
{
    return QObject::tr( "Export &CSV File..." );
}


QPixmap CsvExportPlugin::exportActionIcon() const
{
    return BarIcon("text-csv");
}


void CsvExportPlugin::exportAccount(const Account *account, const QList<const Posting*> &selected, QWidget *parent) const
{
    QPointer<CsvExportDialog> dialog = new CsvExportDialog( account, selected, parent );

    dialog->exec();

    delete dialog;
}


Q_EXPORT_PLUGIN2( "CsvExportPlugin", CsvExportPlugin );

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
