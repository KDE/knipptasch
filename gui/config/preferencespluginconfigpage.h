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
#ifndef PREFERENCESPLUGINCONFIGPAGE_H
#define PREFERENCESPLUGINCONFIGPAGE_H

#include "abstractconfigpage.h"


class QListWidget;

namespace Knipptasch
{
    class Preferences;
}


/**
 * @class PreferencesPluginConfigPage
 * @brief
 *
 * @see AbstractConfigPage
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class PreferencesPluginConfigPage : public AbstractConfigPage
{
        Q_OBJECT
        Q_DISABLE_COPY( PreferencesPluginConfigPage )

    public:
        /**
         * Constructs a new PreferencesPluginConfigPage
         */
        explicit PreferencesPluginConfigPage( Knipptasch::Preferences *pref, ConfigWidget *parent );


        /**
         * Destructs the config page.
         */
        virtual ~PreferencesPluginConfigPage();


        /**
         * Returns whether the current state of the page is valid.
         * This base implementation returns true.
         */
        virtual bool isValid() const;


        /**
         * Returns whether the current state of the dialog is different
         * from the current configuration.
         */
        virtual bool isModified() const;

    public slots:
        /**
         * Apply all changes.
         */
        virtual bool commit();


        /**
         * Revert all changes and reload the initial data.
         */
        virtual void revert();

    private:
        QListWidget *m_view;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
