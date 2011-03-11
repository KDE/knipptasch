/* This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)
    Copyright (c) 2007 David Jarvie (software@astrojar.org.uk)
    Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
//-----------------------------------------------------------------------------
// KDE color selection combo box

// layout management added Oct 1997 by Mario Weilguni
// <mweilguni@sime.com>


#ifndef KCOLORCOMBO_H
#define KCOLORCOMBO_H

#include <QtGui/QComboBox>
#include <QtCore/QList>


class KColorComboPrivate;

/**
 * Combobox for colors.
 *
 * The combobox provides some preset colors to be selected, and an entry to
 * select a custom color using a color dialog.
 *
 * \image html kcolorcombo.png "KDE Color Combo Box"
 */
class KColorCombo : public QComboBox
{
        Q_OBJECT
        Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY activated USER true )
        Q_PROPERTY( QList<QColor> colors READ colors WRITE setColors )

    public:
        /**
         * Constructs a color combo box.
         */
        explicit KColorCombo( QWidget *parent = 0 );
        ~KColorCombo();

        /**
         * Selects the color @p col.
         */
        void setColor( const QColor &col );
        /**
         * Returns the currently selected color.
         **/
        QColor color() const;

        /**
         * Find whether the currently selected color is a custom color selected
         * using a color dialog.
         **/
        bool isCustomColor() const;

        /**
         * Set a custom list of colors to choose from, in place of the standard
         * list.
         * @param cols list of colors. If empty, the selection list reverts to
         *             the standard list.
         **/
        void setColors( const QList<QColor> &colors );

        /**
         * Return the list of colors available for selection.
         * @return list of colors
         **/
        QList<QColor> colors() const;

        /**
         * Clear the color list and don't show it, till the next setColor() call
         **/
        void showEmptyList();

    Q_SIGNALS:
        /**
         * Emitted when a new color box has been selected.
         */
        void activated( const QColor &col );
        /**
         * Emitted when a new item has been highlighted.
         */
        void highlighted( const QColor &col );

    protected:
        virtual void paintEvent( QPaintEvent *event );

    private:
        void addColors();
        void setCustomColor( const QColor &color, bool lookupInPresets = true );

    private slots:
        void slotActivated( int index );
        void slotHighlighted( int index );

    private:
        friend class KColorComboPrivate;
        KColorComboPrivate *const d;

        Q_DISABLE_COPY( KColorCombo )
};

#endif  // KCOLORCOMBO_H
