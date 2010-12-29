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
#ifndef EXPORTPLUGIN_H
#define EXPORTPLUGIN_H

#include <QString>
#include <QList>

class Posting;
class QString;
class QWidget;
class QPixmap;
class Account;


/**
 * @class ExportPlugin
 * @brief Abstract base class that provides an interface for export plugins.
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class ExportPlugin
{
    public:
        /**
         * Default Destructor
         */
        virtual ~ExportPlugin() {}

        /**
         * The name of the plugin.
         * @return The name of the plugin
         */
        virtual QString exportActionName() const = 0;

        /**
         * The icon of the plugin.
         * @return The icon of the plugin
         */
        virtual QPixmap exportActionIcon() const = 0;

        /**
         * The toolTip of the plugin.
         * @return The toolTip of the plugin
         */
        virtual QString exportActionToolTip() const
        {
            return QString();
        }


        virtual void exportAccount(const Account *account,
                                   const QList<const Posting*> &selected,
                                   QWidget *parent = 0) const = 0;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
