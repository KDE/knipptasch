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
#include "xmlexportplugin.h"
#include "xmlwriter.h"

#include "backend/account.h"
#include "compat/iconloader.h"

#if defined(HAVE_KDE)
#include <KFileDialog>
#else
#include <QFileDialog>
#endif

#include <QCoreApplication>


XmlExportPlugin::XmlExportPlugin(QObject *parent)
  : QObject( parent )
{
}


bool XmlExportPlugin::isEnabledByDefault() const
{
    return true;
}


QByteArray XmlExportPlugin::identifier() const
{
    return "xml_export_plugin";
}


QString XmlExportPlugin::shortName() const
{
    return tr( "XML Export" );
}


QString XmlExportPlugin::name() const
{
    return tr( "XML Export Plugin" );
}


QPixmap XmlExportPlugin::icon() const
{
    return exportActionIcon();
}


QString XmlExportPlugin::description() const
{
    return tr( "TODO" );
}


QString XmlExportPlugin::author() const
{
    return tr( "Stefan Böhmann" );
}


QByteArray XmlExportPlugin::version() const
{
    return "0.0.1";
}


QString XmlExportPlugin::exportActionName() const
{
    return QObject::tr( "Export &XML File..." );
}


QPixmap XmlExportPlugin::exportActionIcon() const
{
    return BarIcon("text-xml");
}


void XmlExportPlugin::exportAccount(const Account *account, const QList<const Posting*> &selected, QWidget *parent) const
{
    Q_UNUSED( selected );

    QString filename;

#if defined(HAVE_KDE)
    filename = KFileDialog::getSaveFileName( KUrl(), "*.xml|" + QObject::tr( "XML File" ), parent );
#else
    filename = QFileDialog::getSaveFileName( parent, // krazy:exclude=qclasses
                     QObject::tr( "Export XML File - %1" ).arg( QCoreApplication::applicationName() ),
                     QString(), QObject::tr( "XML File" ) + " (*.xml)"
                   );
#endif

    //TODO try... catch
    XmlWriter writer;
    writer.write( account, filename );
}


Q_EXPORT_PLUGIN2( "XmlExportPlugin", XmlExportPlugin );

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
