/*
 * Copyright 2010,2011 by Stefan Böhmann <kde@hilefoks.org>
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
#include "preferencespluginconfigpage.h"

#include "compat/iconloader.h"

#include <Knipptasch/Preferences>
#include <plugin/pluginmanager.h>
#include <plugin/plugin.h>

#include <QVBoxLayout>
#include <QListWidget>
#include <QTimer>

#include <QDebug>



PreferencesPluginConfigPage::PreferencesPluginConfigPage( Knipptasch::Preferences *pref, ConfigWidget *parent )
    : AbstractConfigPage( tr( "Plugin" ), tr( "Plugin Options" ), DesktopIcon( "preferences-plugin" ), parent ),
      m_view( new QListWidget( this ) )
{
    m_view->setIconSize( QSize( 32, 32 ) );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( m_view );
    setLayout( layout );

    connect( m_view, SIGNAL( itemChanged( QListWidgetItem * ) ), this, SIGNAL( pageModified() ) );

    QTimer::singleShot( 100, this, SLOT( revert() ) );
}


PreferencesPluginConfigPage::~PreferencesPluginConfigPage()
{
}


bool PreferencesPluginConfigPage::isValid() const
{
    return true;
}


bool PreferencesPluginConfigPage::isModified() const
{
    Knipptasch::PluginManager *manager = Knipptasch::PluginManager::self();

    for( int i = 0; i < m_view->count(); ++i ) {
        QListWidgetItem *item = m_view->item( i );
        Q_ASSERT( item );

        const QByteArray identifier = item->data( Qt::UserRole ).toByteArray();
        if( !manager->containsPlugin( identifier ) ) {
            return true;
        }

        Knipptasch::Plugin *plugin = manager->plugin( identifier );
        Q_ASSERT( plugin );

        bool checkState = item->checkState() == Qt::Checked;
        if( checkState != plugin->isEnabled() ) {
            return true;
        }
    }

    return false;
}


bool PreferencesPluginConfigPage::commit()
{
    setErrorMessageEnabled( false );

    Knipptasch::PluginManager *manager =  Knipptasch::PluginManager::self();

    for( int i = 0; i < m_view->count(); ++i ) {
        QListWidgetItem *item = m_view->item( i );
        Q_ASSERT( item );

        const QByteArray identifier = item->data( Qt::UserRole ).toByteArray();
        if( !manager->containsPlugin( identifier ) ) {
            // Perhaps the plugin was uninstalled in the meantime?
            qDebug() << "PreferencesPluginConfigPage::commit(): Plugin " << identifier
                     << "not found. Perhaps the plugin was uninstalled in the meantime?";
            continue;
        }

        Knipptasch::Plugin *plugin = manager->plugin( identifier );
        Q_ASSERT( plugin );
        Q_ASSERT( identifier == plugin->identifier() );

        bool checkState = item->checkState() == Qt::Checked;
        if( !manager->enablePlugin( identifier, item->checkState() == Qt::Checked ) ) {
            qDebug() << "PreferencesPluginConfigPage::commit(): Failed to change state of plugin" << identifier;

            setErrorMessageDescription( tr( "Failed to change state of plugin %1." ).arg( plugin->shortName() ) );
            setErrorMessageEnabled( true );
        }
    }

    revert();

    return true;
}


void PreferencesPluginConfigPage::revert()
{
    m_view->clear();

    Knipptasch::PluginManager *manager = Knipptasch::PluginManager::self();

    QSet<QByteArray> set = manager->allPluginIdentifiers();
    foreach( const QByteArray & it, set ) {
        Knipptasch::Plugin *plugin = manager->plugin( it );

        QPixmap icon = plugin->icon();
        if( icon.isNull() ) {
            icon = BarIcon( "preferences-plugin" );
        }

        QListWidgetItem *item = new QListWidgetItem( icon, plugin->name() );
        item->setData( Qt::UserRole, plugin->identifier() );
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
        item->setCheckState( plugin->isEnabled() ? Qt::Checked : Qt::Unchecked );

        m_view->addItem( item );
    }

    pageModified();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
