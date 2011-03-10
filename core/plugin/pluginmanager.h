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

#ifndef KNIPPTASCH_CORE_PLUGINMANAGER_H
#define KNIPPTASCH_CORE_PLUGINMANAGER_H

#include "knipptasch_core_export.h"

#include <QObject>


class QPluginLoader;


/**
 * @namespace Knipptasch
 * @brief Default namespace for Knipptasch
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
namespace Knipptasch
{

    class Plugin;

        
    class KNIPPTASCH_CORE_EXPORT PluginManager : public QObject
    {
        Q_OBJECT

        public:
            /**
             * Default constructor. Locates all plugins, sets watchers
             * on those directories to locate new ones and loads those 
             * enabled in the config.
             */
            PluginManager(QObject *parent = 0);


            /**
             * Destructor
             */
            ~PluginManager();
            
            
            /**
             * @brief Accessing the singleton instance
             * 
             *  Method for accessing the singleton instance of the class.
             *  Instanciates if no instance yet exists.
             * 
             * @return Pointer to the plugin manager
             */
            static PluginManager* self();

            
            /**
             * @brief Get all available plugin identifiers
             * 
             *  Returns a set of all currently available plugin identifiers
             *  found in all plugin directories.
             * 
             * @see enabledPluginIdentifiers
             * @see disabledPluginIdentifiers
             * 
             * @return A set of all available plugin identifiers.
             */
            QSet<QByteArray> allPluginIdentifiers() const;

            
            /**
             * @brief Get available identifiers from all enabled plugins.
             * 
             *  Returns a set of identifiers of all currently enabled plugins.
             * 
             * @see allPluginIdentifiers
             * @see disabledPluginIdentifiers
             * 
             * @return A set of identifiers of all enabled plugins.
             */
            QSet<QByteArray> enabledPluginIdentifiers() const;
            
            
            /**
             * @brief Get available identifiers from all disabled plugins.
             * 
             *  Returns a set of identifiers of all currently disabled plugins.
             * 
             * @see allPluginIdentifiers
             * @see enabledPluginIdentifiers
             * 
             * @return A set of identifiers of all disabled plugins.
             */            
            QSet<QByteArray> disabledPluginIdentifiers() const;

            
            /**
             * 
             */
            bool containsPlugin(const QByteArray &identifier) const;
            
            
            /**
             * @brief Get the plugin specified by identifier
             */
            Knipptasch::Plugin* plugin(const QByteArray &identifier);
            
            
            /**
             * @brief Get all plugins
             */
            QList<Knipptasch::Plugin*> plugins();
            

        public slots:
            /**
             * @brief load all Knipptasch plugins.
             * 
             *  This loads all Knipptasch plugins and plugins were Plugin::isEnabled() is true
             *  will be enabled.
             */
            void loadPlugins();

            
            /**
             * @brief Unloads all Knipptasch plugins.
             * 
             * @return true if unloading all plugins success, else false
             */
            bool unloadPlugins();

            
            /**
             * @brief Unloads all disabled Knipptasch plugins.
             * 
             * @return true if unloading all plugins success, else false
             */
            bool unloadDisabledPlugins();
            
            
            /**
             * @brief Enable the given plugin and enable it by default.
             * 
             *  Enable the given plugin and enable it by default 
             * (e.g. next time the application starts, it will enable this plugin).
             * 
             * @return true if the plugin could be enabled else false or when @p identifier 
             * 
             */
            bool enablePlugin(const QByteArray &identifier, bool enable = true);
            
            
            /**
             * @brief Disable the given plugin and disable it by default.
             * 
             *  Disable the given plugin and disable it by default 
             * (e.g. next time the application starts, it will not enable this plugin).
             */
            bool disablePlugin(const QByteArray &identifier);


            /**
             * unload all plugins and disable directory watcher
             * to redo this call loadPlugins()
             */
            void aboutToQuit();
            
        signals:
            /**
             * @brief emitted when a plugin was loaded
             */
            void pluginLoaded(QByteArray identifier);
            
            
            /**
             * @brief emitted when a plugin was unloaded
             */
            void pluginUnloaded(QByteArray identifier);
            
            
            /**
             * @brief emitted when a plugin was enabled
             */
            void pluginEnabled(QByteArray identifier);
            
            
            /**
             * @brief emitted when a plugin was disabled
             */
            void pluginDisabled(QByteArray identifier);

            
        private:
            void loadPlugin(QPluginLoader *loader);

        private:
            class Private;
            Private *d;        
    };

} // EndNamspace Knipptasch

#endif
 

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
