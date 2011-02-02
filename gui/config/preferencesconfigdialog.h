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
#ifndef PREFERENCESCONFIGDIALOG_H
#define PREFERENCESCONFIGDIALOG_H

#include "configdialog.h"

class Preferences;


/**
 * @class PreferencesConfigDialog
 * @brief
 *
 * @see ConfigDialog
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class PreferencesConfigDialog : public ConfigDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(PreferencesConfigDialog)

    public:
        /**
         * Constructs a new PreferencesConfigDialog
         */
        explicit PreferencesConfigDialog(Preferences *pref, QWidget *parent = 0);

        /**
         * Destructs the config dialog.
         */
        virtual ~PreferencesConfigDialog();
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
