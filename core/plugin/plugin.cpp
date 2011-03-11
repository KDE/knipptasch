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
#include "plugin.h"
#include <QUrl>


namespace Knipptasch
{

    Plugin::Plugin()
        : m_enabled( false )
    {
    }

    Plugin::~Plugin()
    {
    }

    bool Plugin::isEnabled() const
    {
        return m_enabled;
    }


    bool Plugin::isEnabledByDefault() const
    {
        return false;
    }


    bool Plugin::setEnabled( bool b )
    {
        if( !b ) {
            return setDisabled();
        }

        if( !isEnabled() ) {
            m_enabled = enable();
        }

        return isEnabled();
    }


    bool Plugin::setDisabled()
    {
        if( isEnabled() ) {
            m_enabled = !disable();
        }

        return !isEnabled();
    }


    QUrl Plugin::homepage() const
    {
        return QUrl();
    }


    QWidget *Plugin::options() const
    {
        return 0;
    }

    bool Plugin::enable()
    {
        return true;
    }

    bool Plugin::disable()
    {
        return true;
    }

} //EndNamespace Knipptasch


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
