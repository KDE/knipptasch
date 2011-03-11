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
#include "pluginmanager.h"
#include "plugin.h"

#include "preferences.h"

#include <QPluginLoader>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QDebug>

#include <sys/stat.h>
#include <QCoreApplication>


Q_GLOBAL_STATIC( Knipptasch::PluginManager, private_knipptasch_pluginmanager )


namespace Knipptasch
{


    struct PluginManager::Private {
        Private()
            : watcher( 0 ) {
        }

        ~Private() {
            delete watcher;
        }


        QFileSystemWatcher *watcher;
        QHash<QByteArray, QPluginLoader *> loader;
    };



    PluginManager *PluginManager::self()
    {
        return private_knipptasch_pluginmanager();
    }


    PluginManager::PluginManager( QObject *parent )
        : QObject( parent ),
          d( new PluginManager::Private )
    {
        loadPlugins();
    }


    PluginManager::~PluginManager()
    {
        delete d;
    }


    QSet<QByteArray> PluginManager::allPluginIdentifiers() const
    {
        return d->loader.keys().toSet();
    }


    QSet<QByteArray> PluginManager::enabledPluginIdentifiers() const
    {
        QSet<QByteArray> set;
        QHash<QByteArray, QPluginLoader *>::const_iterator it;

        for( it = d->loader.constBegin(); it != d->loader.constEnd(); ++it ) {
            Plugin *plugin = qobject_cast<Plugin *>( it.value()->instance() );
            Q_ASSERT( plugin );
            Q_ASSERT( plugin->identifier() == it.key() );

            if( plugin->isEnabled() ) {
                set.insert( it.key() );
            }
        }

        return set;
    }


    QSet<QByteArray> PluginManager::disabledPluginIdentifiers() const
    {
        QSet<QByteArray> set;
        QHash<QByteArray, QPluginLoader *>::const_iterator it;

        for( it = d->loader.constBegin(); it != d->loader.constEnd(); ++it ) {
            Plugin *plugin = qobject_cast<Plugin *>( it.value()->instance() );
            Q_ASSERT( plugin );
            Q_ASSERT( plugin->identifier() == it.key() );

            if( !plugin->isEnabled() ) {
                set.insert( it.key() );
            }
        }

        return set;
    }


    bool PluginManager::containsPlugin( const QByteArray &identifier ) const
    {
        return d->loader.contains( identifier );
    }


    Knipptasch::Plugin *PluginManager::plugin( const QByteArray &identifier )
    {
        Q_ASSERT( d->loader.contains( identifier ) );

        QPluginLoader *loader = d->loader.value( identifier );
        Q_ASSERT( loader );

        Plugin *plugin = qobject_cast<Plugin *>( loader->instance() );
        Q_ASSERT( plugin );

        return plugin;
    }


    void PluginManager::loadPlugins()
    {
        unloadDisabledPlugins();

        /*
            foreach( QObject *plugin, QPluginLoader::staticInstances() ) {
                load(plugin)
            }
        */
        const QStringList dirs = Preferences::self()->pluginDirectories();
        const QStringList prefixes = Preferences::self()->pluginNamePrefixes();

        foreach( const QString & d, dirs ) {
            QFileInfoList list = QDir( d ).entryInfoList( QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks );

            foreach( const QFileInfo & info, list ) {

                foreach( const QString & prefix, prefixes ) {
                    if( !info.fileName().startsWith( prefix, Qt::CaseInsensitive ) ) {
                        continue;
                    }

                    QPluginLoader *loader = new QPluginLoader( info.absoluteFilePath(), this );

                    if( loader->load() ) {
                        loadPlugin( loader );
                    } else {
                        qDebug() << QString( "PluginManager::loadPlugins(): '%1'" ).arg( loader->errorString() );
                        delete loader;
                    }
                }
            }
        }

        if( !d->watcher ) {
            d->watcher = new QFileSystemWatcher( Preferences::self()->pluginDirectories(), this );
            connect( d->watcher, SIGNAL( directoryChanged( QString ) ), this, SLOT( loadPlugins() ) );
        }
    }


    bool PluginManager::unloadPlugins()
    {
        QHash<QByteArray, QPluginLoader *>::iterator it;

        bool failed = unloadDisabledPlugins();
        for( it = d->loader.begin(); it != d->loader.end(); ++it ) {
            Plugin *plugin = qobject_cast<Plugin *>( it.value()->instance() );
            Q_ASSERT( plugin );
            Q_ASSERT( plugin->identifier() == it.key() );

            if( plugin->isEnabled() ) {
                if( !plugin->setDisabled() ) {
                    failed = true;

                    continue;
                }

                emit pluginDisabled( it.key() );
            }

            it.value()->unload();
            d->loader.erase( it );

            emit pluginUnloaded( it.key() );
        }

        return failed;
    }


    bool PluginManager::unloadDisabledPlugins()
    {
        QHash<QByteArray, QPluginLoader *>::iterator it;

        for( it = d->loader.begin(); it != d->loader.end(); ++it ) {
            Plugin *plugin = qobject_cast<Plugin *>( it.value()->instance() );
            Q_ASSERT( plugin );
            Q_ASSERT( plugin->identifier() == it.key() );

            if( plugin->isEnabled() ) {
                continue;
            }

            it.value()->unload();
            d->loader.erase( it );

            emit pluginUnloaded( it.key() );
        }

        return true;
    }


    bool PluginManager::enablePlugin( const QByteArray &identifier, bool enable )
    {
        if( !d->loader.contains( identifier ) ) {
            qDebug() << "PluginManager::enablePlugin(): No plugin" << identifier << "found.";
            return false;
        }

        Knipptasch::Plugin *p = plugin( identifier );
        Q_ASSERT( p );

        const QString key = QString( "Plugins/%1_enabled" ).arg( QString( p->identifier() ) );

        Preferences::self()->setValue( key, enable );
        Preferences::self()->sync();

        if( p->isEnabled() == enable ) {
            qDebug() << "PluginManager::enablePlugin(): Plugin" << identifier << "is already enabled.";
            return true;
        }

        if( p->setEnabled( enable ) ) {
            if( enable ) {
                Q_ASSERT( p->isEnabled() );
                emit pluginEnabled( p->identifier() );
            } else {
                Q_ASSERT( !p->isEnabled() );
                emit pluginDisabled( p->identifier() );
            }
        } else {
            qDebug() << "PluginManager::enablePlugin(): Error while"
                     << ( enable ? "enable" : "disable" )
                     << "plugin" << identifier;
        }

        Q_ASSERT( enable == p->isEnabled() );

        return true;
    }


    bool PluginManager::disablePlugin( const QByteArray &identifier )
    {
        return enablePlugin( identifier, false );
    }


    void PluginManager::aboutToQuit()
    {
        unloadPlugins();

        if( d->watcher ) {
            disconnect( d->watcher, 0, 0, 0 );
            d->watcher->deleteLater();

            d->watcher = 0;
        }
    }


    void PluginManager::loadPlugin( QPluginLoader *loader )
    {
        Plugin *plugin = qobject_cast<Plugin *>( loader->instance() );

        if( !plugin ) {
            qDebug() << QString( "loading plugin '%1': failed (no Knipptasch plugin)" ).arg( loader->fileName() );

            loader->unload();
            delete loader;

            return;
        }

        Q_ASSERT( plugin );

        if( d->loader.contains( plugin->identifier() ) ) {
            qDebug() << QString( "loading plugin '%1': already loaded" ).arg( plugin->shortName() );

            loader->unload();
            delete loader;

            return;
        }

        d->loader.insert( plugin->identifier(), loader );
        qDebug() << QString( "loading plugin '%1': success" ).arg( plugin->shortName() );

        const QString key = QString( "Plugins/%1_enabled" ).arg( QString( plugin->identifier() ) );
        if( Preferences::self()->value( key, plugin->isEnabledByDefault() ).toBool() ) {
            if( plugin->setEnabled() ) {
                qDebug() << QString( "enable plugin '%1': success" ).arg( plugin->shortName() );

                emit pluginEnabled( plugin->identifier() );
            } else {
                qDebug() << QString( "enable plugin '%1': failed" ).arg( plugin->shortName() );
            }
        } else {
            qDebug() << QString( "enable plugin '%1': skip (plugin disabled by config value)" ).arg( plugin->shortName() );
        }

        emit pluginLoaded( plugin->identifier() );
    }


} // EndNamspace Knipptasch


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
