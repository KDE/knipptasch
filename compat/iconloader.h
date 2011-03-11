/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 * Copyright 2010 by Volker Lanz <vl@fidra.de>
 * Copyright 2005-09 by the Quassel Project <>
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
#ifndef ICONLOADER_H
#define ICONLOADER_H

#if defined(HAVE_KDE)

#include <kiconloader.h>

class IconLoader : public KIconLoader
{
};

#else

#include <QPixmap>


class IconLoader : public QObject
{
    public:
        enum Group {
            NoGroup = -1,   ///< No group
            Desktop = 0,    ///< Desktop icons
            Toolbar,        ///< Toolbar icons
            MainToolbar,    ///< Main toolbar icons
            Small,          ///< Small icons, e.g. for buttons
            Panel,          ///< Panel icons
            Dialog,         ///< Icons for use in dialog title etc.
            LastGroup
        };

        /// Standard icon sizes
        enum StdSizes {
            SizeSmall = 16,         ///< Small icons for menu entries
            SizeSmallMedium = 22,   ///< Slightly larger small icons for toolbars, panels, etc
            SizeMedium = 32,        ///< Medium-sized icons for the desktop
            SizeLarge = 48,         ///< Large icons for the panel
            SizeHuge = 64,          ///< Huge icons for iconviews
            SizeEnormous = 128      ///< Enormous icons for iconviews
        };

        explicit IconLoader( QObject *parent = 0 );

        static IconLoader *global();

        /// Load a pixmap for the given name and group
        QPixmap loadIcon( const QString &name, Group group, int size = 0 );

        QString theme() const {
            return m_Theme;
        }
        void setTheme( const QString &name );

    protected:
        QString findIconPath( const QString &name, int size ) const;

    private:
        QString m_Theme;
        QStringList m_ThemedIconDirNames;
        QStringList m_PlainIconDirNames;
};


QPixmap DesktopIcon( const QString &name, int size = 0 );
QPixmap BarIcon( const QString &name, int size = 0 );
QPixmap MainBarIcon( const QString &name, int size = 0 );
QPixmap SmallIcon( const QString &name, int size = 0 );


#endif

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
