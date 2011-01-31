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
#include "xmlimportplugin.h"
#include "xmlreader.h"

#include "backend/account.h"
#include "backend/posting.h"
#include "backend/money.h"
#include "backend/category.h"

#include "compat/iconloader.h"

#if defined(HAVE_KDE)
#include <KFileDialog>
#else
#include <QFileDialog>
#endif

#include <QCoreApplication>
#include <QDebug>



XmlImportPlugin::XmlImportPlugin(QObject *parent)
  : QObject( parent )
{
    qsrand( QTime::currentTime().msec() );
}


QByteArray XmlImportPlugin::pluginIdentifier() const
{
    return "xml_import_plugin";
}


QString XmlImportPlugin::pluginName() const
{
    return tr( "XML Import Plugin" );

}


QByteArray XmlImportPlugin::pluginVersion() const
{
    return "0.0.1";
}


QString XmlImportPlugin::importActionName() const
{
    return QObject::tr( "Import &XML File..." );
}


QPixmap XmlImportPlugin::importActionIcon() const
{
    return BarIcon("text-xml");
}


Account* XmlImportPlugin::importAccount(QWidget *parent) const
{
    QString filename;

#if defined(HAVE_KDE)
    filename = KFileDialog::getOpenFileName( KUrl(), "*.xml|" + QObject::tr( "XML File" ), parent );
#else
    filename = QFileDialog::getOpenFileName( parent, // krazy:exclude=qclasses
                     QObject::tr( "Import XML File - %1" ).arg( QCoreApplication::applicationName() ),
                     QString(), QObject::tr( "XML File" ) + " (*.xml)"
                   );
#endif

    if( filename.isEmpty() ) {
        return 0;
    }

    //TODO try... catch

    Account *acc = new Account;
    XmlReader reader;
    reader.read( acc, filename );

    return acc;
}


Q_EXPORT_PLUGIN2( "XmlImportPlugin", XmlImportPlugin );

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
