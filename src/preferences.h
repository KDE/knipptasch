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
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QColor>
#include <QVariant>

#define POSITIVE_AMOUNT_FOREGROUND_COLOR QColor( 0, 140, 0 )
#define NEGATIVE_AMOUNT_FOREGROUND_COLOR QColor( 140, 0, 0 )
#define AVAILABLE_WARRANTY_FOREGROUND_COLOR QColor( 0, 0, 140 )
#define EXPIRED_WARRANTY_FOREGROUND_COLOR QColor( 186, 189, 182 )
#define DEFAULT_POSTING_BACKGROUND_COLOR QColor( 90, 255, 70, 40 )
#define CURRENT_POSTING_BACKGROUND_COLOR QColor( 255, 210, 0, 40 )
#define FUTURE_POSTING_BACKGROUND_COLOR QColor( 100, 200, 255, 40 )
#define INCOMPLETE_POSTING_BACKGROUND_COLOR QColor( 190, 0, 0, 40 )


class Preferences
{
    public:
        enum EnumInvalidPostingPosition {
            Inline,
            ToTop,
            ToBottom,
            AboveCurrent,
            BelowCurrent
        };

        static Preferences* self();

        void sync();

        QVariant value(const QString &group, const QString &key, const QVariant &defaultValue = QVariant()) const;

        template <class T>
        T value(const QString &group, const QString &key, const QVariant &defaultValue = QVariant()) const
        {
            return value( group, key, defaultValue ).value<T>();
        }

        void setValue(const QString &group, const QString &key, const QVariant &value);


        bool showStatusbar() const
        {
            return value<bool>( "General", "ShowStatusbar", true );
        }

        void setShowStatusbar(bool state)
        {
            setValue( "General", "ShowStatusbar", state );
        }

        QStringList recentFilesList() const
        {
            return value<QStringList>( "RecentFiles", "List" );
        }

        void setRecentFilesList(const QStringList &list)
        {
            setValue( "RecentFiles", "List", list );
        }

        int recentFilesListMaximumCount()
        {
            return Preferences::self()->value<int>( "RecentFiles", "MaximumCount", 10 );
        }

        void setRecentFilesListMaximumCount(int v)
        {
            setValue( "RecentFiles", "MaximumCount", v > 0 ? v : 0 );
        }

        bool positiveAmountForegroundEnabled()
        {
            return value<bool>( "Colors", "PositiveAmountForegroundEnabled", true );
        }

        void setPositiveAmountForegroundEnabled(bool b)
        {
            setValue( "Colors", "PositiveAmountForegroundEnabled", b );
        }

        QColor positiveAmountForegroundColor()
        {
            return value<QColor>( "Colors", "PositiveAmountForegroundColor", POSITIVE_AMOUNT_FOREGROUND_COLOR );
        }

        void setPositiveAmountForegroundColor(const QColor &color)
        {
            setValue( "Colors", "PositiveAmountForegroundColor", color );
        }

        bool negativeAmountForegroundEnabled()
        {
            return value<bool>( "Colors", "NegativeAmountForegroundEnabled", true );
        }

        void setNegativeAmountForegroundEnabled(bool b)
        {
            setValue( "Colors", "NegativeAmountForegroundEnabled", b );
        }

        QColor negativeAmountForegroundColor()
        {
            return value<QColor>( "Colors", "NegativeAmountForegroundColor", NEGATIVE_AMOUNT_FOREGROUND_COLOR );
        }

        void setNegativeAmountForegroundColor(const QColor &color)
        {
            setValue( "Colors", "NegativeAmountForegroundColor", color );
        }

        bool availableWarrantyForegroundEnabled()
        {
            return value<bool>( "Colors", "AvailableWarrantyForegroundEnabled", true );
        }

        void setAvailableWarrantyForegroundEnabled(bool b)
        {
            setValue( "Colors", "AvailableWarrantyForegroundEnabled", b );
        }

        QColor availableWarrantyForegroundColor()
        {
            return value<QColor>( "Colors", "AvailableWarrantyForegroundColor", AVAILABLE_WARRANTY_FOREGROUND_COLOR );
        }

        void setAvailableWarrantyForegroundColor(const QColor &color)
        {
            setValue( "Colors", "AvailableWarrantyForegroundColor", color );
        }

        bool expiredWarrantyForegroundEnabled()
        {
            return value<bool>( "Colors", "ExpiredWarrantyForegroundEnabled", true );
        }

        void setExpiredWarrantyForegroundEnabled(bool b)
        {
            setValue( "Colors", "ExpiredWarrantyForegroundEnabled", b );
        }

        QColor expiredWarrantyForegroundColor()
        {
            return value<QColor>( "Colors", "ExpiredWarrantyForegroundColor", EXPIRED_WARRANTY_FOREGROUND_COLOR );
        }

        void setExpiredWarrantyForegroundColor(const QColor &color)
        {
            setValue( "Colors", "ExpiredWarrantyForegroundColor", color );
        }

        bool currentPostingBackgroundEnabled()
        {
            return value<bool>( "Colors", "CurrentPostingBackgroundEnabled", true );
        }

        void setCurrentPostingBackgroundEnabled(bool b)
        {
            setValue( "Colors", "CurrentPostingBackgroundEnabled", b );
        }

        QColor currentPostingBackgroundColor()
        {
            return value<QColor>( "Colors", "CurrentPostingBackgroundColor", CURRENT_POSTING_BACKGROUND_COLOR );
        }

        void setCurrentPostingBackgroundColor(const QColor &color)
        {
            setValue( "Colors", "CurrentPostingBackgroundColor", color );
        }

        bool futurePostingBackgroundEnabled()
        {
            return value<bool>( "Colors", "FuturePostingBackgroundEnabled", true );
        }

        void setFuturePostingBackgroundEnabled(bool b)
        {
            setValue( "Colors", "FuturePostingBackgroundEnabled", b );
        }

        QColor futurePostingBackgroundColor()
        {
            return value<QColor>( "Colors", "FuturePostingBackgroundColor", FUTURE_POSTING_BACKGROUND_COLOR );
        }

        void setFuturePostingBackgroundColor(const QColor &color)
        {
            setValue( "Colors", "FuturePostingBackgroundColor", color );
        }

        bool incompletePostingBackgroundEnabled()
        {
            return value<bool>( "Colors", "IncompletePostingBackgroundEnabled", true );
        }

        void setIncompletePostingBackgroundEnabled(bool b)
        {
            setValue( "Colors", "IncompletePostingBackgroundEnabled", b );
        }

        QColor incompletePostingBackgroundColor()
        {
            return value<QColor>( "Colors", "IncompletePostingBackgroundColor", INCOMPLETE_POSTING_BACKGROUND_COLOR );
        }

        void setIncompletePostingBackgroundColor(const QColor &color)
        {
            setValue( "Colors", "IncompletePostingBackgroundColor", color );
        }

        bool defaultPostingBackgroundEnabled()
        {
            return value<bool>( "Colors", "DefaultPostingBackgroundEnabled", true );
        }

        void setDefaultPostingBackgroundEnabled(bool b)
        {
            setValue( "Colors", "DefaultPostingBackgroundEnabled", b );
        }

        QColor defaultPostingBackgroundColor()
        {
            return value<QColor>( "Colors", "DefaultPostingBackgroundColor", DEFAULT_POSTING_BACKGROUND_COLOR );
        }

        void setDefaultPostingBackgroundColor(const QColor &color)
        {
            setValue( "Colors", "DefaultPostingBackgroundColor", color );
        }

        //static Preferences::EnumInvalidPostingPosition invalidPostingPosition() { return AboveCurrent; }

    private:
        Preferences();
        ~Preferences();
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
