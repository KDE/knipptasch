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



/**
 * @namespace Knipptasch
 * @brief Default namespace for Knipptasch
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
namespace Knipptasch
{
    
    /**
     * @class XmlReader
     * @brief
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

            virtual QByteArray pluginIdentifier() const = 0;
            virtual QString pluginName() const = 0;
            virtual QByteArray pluginVersion() const = 0;
    };
    
} // EndNamspace Knipptasch


Q_DECLARE_INTERFACE( Knipptasch::Plugin, "org.kde.Knipptasch.Plugin/0.1" );

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1: