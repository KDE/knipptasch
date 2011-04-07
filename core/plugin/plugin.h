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
#ifndef KNIPPTASCH_CORE_PLUGIN_INTERFACE_H
#define KNIPPTASCH_CORE_PLUGIN_INTERFACE_H

#include "knipptasch_core_export.h"

#include <QtCore/QtPlugin>
#include <QtCore/QPointer>
#include <QtCore/QString>

class QPixmap;
class QUrl;


/**
 * @namespace Knipptasch
 * @brief Default namespace for Knipptasch
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
namespace Knipptasch
{
    /**
     * @class Plugin
     * @ingroup Core
     * @ingroup Plugin
     * @brief An abstract class for implementing a plugin
     *
     * @author Stefan Böhmann <kde@hilefoks.org>
     */
    class KNIPPTASCH_CORE_EXPORT Plugin
    {
        public:
            /**
             * Default Destructor
             */
            virtual ~Plugin();


            /**
             *
             */
            bool isEnabled() const;


            /**
             *
             */
            virtual bool isEnabledByDefault() const;


            /**
             * @brief Enable plugin
             *
             * @see PluginManager::setEnabled
             *
             * @return true if plugin was successfully enabled
             */
            bool setEnabled( bool enable = true );


            /**
             * @brief Disable plugin
             *
             * @see PluginManager::setDisabled
             *
             * @return true if plugin was successfully disabled
             */
            bool setDisabled();



            /**
             * @brief Plugin Identifier
             */
            virtual QByteArray identifier() const = 0;


            /**
             * @brief Short name for the plugin
             *
             * This is the short name of the plugin, used for options structures.
             * It must consist of only alphanumerics (no spaces or punctuation).
             *
             * @return Short plugin name
             */
            virtual QString shortName() const = 0;


            /**
             * @brief Plugin Name
             *
             * The full name of the plugin.
             */
            virtual QString name() const = 0;


            /**
             * @brief Plugin Icon
             *
             * The icon of the plugin.
             */
            virtual QPixmap icon() const = 0;


            /**
             * @brief Plugin Description
             *
             * The description of the plugin.
             */
            virtual QString description() const = 0;


            /**
             * @brief Plugin Decription
             *
             * The description of the plugin.
             */
            virtual QString author() const = 0;


            /**
             * @brief Plugin Decription
             *
             * The description of the plugin.
             */
            virtual QUrl homepage() const;


            /**
             * @brief Plugin version
             *
             * Free-form string of the plugin version. Human readable
             */
            virtual QByteArray version() const = 0;


            /**
             * @brief Plugin options widget
             *
             * This method is called by the Knipptasch options system to retrieve
             * a widget containing the options for this plugin.
             *
             * This will then be embedded in the options dialog, so this
             * should be considered when designing the widget.
             *
             * Should return NULL when there are no user-configurable options.
             * The calling method is responsible for deleting the options.
             *
             * TODO: replace QWidget by AbstractConfigPage
             */
            virtual QWidget *options() const;


        protected:
            /**
             * Default Constructor
             */
            Plugin();


            /**
             * @see setEnabled
             */
            virtual bool enable();


            /**
             * @see setDisabled
             */
            virtual bool disable();

        private:
            bool m_enabled;
    };

} // EndNamspace Knipptasch


Q_DECLARE_INTERFACE( Knipptasch::Plugin, "org.kde.Knipptasch.Plugin/0.2" );

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
