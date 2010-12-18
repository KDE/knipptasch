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
#include <QSettings>

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

        static Preferences* self() {
            static Preferences pref;
            return &pref;
        }

        void sync()
        {
            m_settings.sync();
        }
        
        bool showStatusBar() const
        {
            return m_settings.value( "General/ShowStatusBar", true ).toBool();
        }

        void setShowStatusBar(bool state)
        {
            m_settings.setValue( "General/ShowStatusBar", state );
            sync();
        }

        int defaultLengthOfWarrantyInMonth() const
        {
            return m_settings.value( "General/DefaultLengthOfWarrantyInMonth", 24 ).toInt();
        }

        void setDefaultLengthOfWarrantyInMonth(int value)
        {
            m_settings.setValue( "General/DefaultLengthOfWarrantyInMonth", value );
            sync();
        }
        
        QStringList recentFilesList() const
        {
            return m_settings.value( "General/RecentFilesList" ).toStringList();
        }

        void setRecentFilesList(const QStringList &list)
        {
            m_settings.setValue( "General/RecentFilesList", list );
            sync();
        }

        int recentFilesListMaximumCount() const
        {
            return m_settings.value( "General/RecentFilesMaximumCount", 10 ).toInt();
        }

        void setRecentFilesListMaximumCount(int v)
        {
            m_settings.setValue( "General/RecentFilesMaximumCount", v > 0 ? v : 0 );
            sync();
        }

        bool positiveAmountForegroundEnabled() const
        {
            return m_settings.value( "Appearance/PositiveAmountForegroundEnabled", true ).toBool();
        }

        void setPositiveAmountForegroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/PositiveAmountForegroundEnabled", b );
            sync();
        }

        QColor positiveAmountForegroundColor() const
        {
            return m_settings.value( "Appearance/PositiveAmountForegroundColor", POSITIVE_AMOUNT_FOREGROUND_COLOR ).value<QColor>();
        }

        void setPositiveAmountForegroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/PositiveAmountForegroundColor", color );
            sync();
        }

        bool negativeAmountForegroundEnabled() const
        {
            return m_settings.value( "Appearance/NegativeAmountForegroundEnabled", true ).toBool();
        }

        void setNegativeAmountForegroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/NegativeAmountForegroundEnabled", b );
            sync();
        }

        QColor negativeAmountForegroundColor() const
        {
            return m_settings.value( "Appearance/NegativeAmountForegroundColor", NEGATIVE_AMOUNT_FOREGROUND_COLOR ).value<QColor>();
        }

        void setNegativeAmountForegroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/NegativeAmountForegroundColor", color );
            sync();
        }

        bool availableWarrantyForegroundEnabled() const
        {
            return m_settings.value( "Appearance/AvailableWarrantyForegroundEnabled", true ).toBool();
        }

        void setAvailableWarrantyForegroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/AvailableWarrantyForegroundEnabled", b );
            sync();
        }

        QColor availableWarrantyForegroundColor() const
        {
             return m_settings.value( "Appearance/AvailableWarrantyForegroundColor", AVAILABLE_WARRANTY_FOREGROUND_COLOR ).value<QColor>();
        }

        void setAvailableWarrantyForegroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/AvailableWarrantyForegroundColor", color );
            sync();
        }

        bool expiredWarrantyForegroundEnabled() const
        {
            return m_settings.value( "Appearance/ExpiredWarrantyForegroundEnabled", true ).toBool();
        }

        void setExpiredWarrantyForegroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/ExpiredWarrantyForegroundEnabled", b );
            sync();
        }

        QColor expiredWarrantyForegroundColor() const
        {
            return m_settings.value( "Appearance/ExpiredWarrantyForegroundColor", EXPIRED_WARRANTY_FOREGROUND_COLOR ).value<QColor>();
        }

        void setExpiredWarrantyForegroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/ExpiredWarrantyForegroundColor", color );
            sync();
        }

        bool currentPostingBackgroundEnabled() const
        {
            return m_settings.value( "Appearance/CurrentPostingBackgroundEnabled", true ).toBool();
        }

        void setCurrentPostingBackgroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/CurrentPostingBackgroundEnabled", b );
            sync();
        }

        QColor currentPostingBackgroundColor() const
        {
            return m_settings.value( "Appearance/CurrentPostingBackgroundColor", CURRENT_POSTING_BACKGROUND_COLOR ).value<QColor>();
        }

        void setCurrentPostingBackgroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/CurrentPostingBackgroundColor", color );
            sync();
        }

        bool futurePostingBackgroundEnabled() const
        {
            return m_settings.value( "Appearance/FuturePostingBackgroundEnabled", true ).toBool();
        }

        void setFuturePostingBackgroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/FuturePostingBackgroundEnabled", b );
            sync();
        }

        QColor futurePostingBackgroundColor() const
        {
            return m_settings.value( "Appearance/FuturePostingBackgroundColor", FUTURE_POSTING_BACKGROUND_COLOR ).value<QColor>();
        }

        void setFuturePostingBackgroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/FuturePostingBackgroundColor", color );
            sync();
        }

        bool incompletePostingBackgroundEnabled() const
        {
            return m_settings.value( "Appearance/IncompletePostingBackgroundEnabled", true ).toBool();
        }

        void setIncompletePostingBackgroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/IncompletePostingBackgroundEnabled", b );
            sync();
        }

        QColor incompletePostingBackgroundColor() const
        {
            return m_settings.value( "Appearance/IncompletePostingBackgroundColor", INCOMPLETE_POSTING_BACKGROUND_COLOR ).value<QColor>();
        }

        void setIncompletePostingBackgroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/IncompletePostingBackgroundColor", color );
            sync();
        }

        bool defaultPostingBackgroundEnabled() const
        {
            return m_settings.value( "Appearance/DefaultPostingBackgroundEnabled", true ).toBool();
        }

        void setDefaultPostingBackgroundEnabled(bool b)
        {
            m_settings.setValue( "Appearance/DefaultPostingBackgroundEnabled", b );
            sync();
        }

        QColor defaultPostingBackgroundColor() const
        {
            return m_settings.value( "Appearance/DefaultPostingBackgroundColor", DEFAULT_POSTING_BACKGROUND_COLOR ).value<QColor>();
        }

        void setDefaultPostingBackgroundColor(const QColor &color)
        {
            m_settings.setValue( "Appearance/DefaultPostingBackgroundColor", color );
            sync();
        }

        bool sortPostingsByMaturity() const
        {
            return m_settings.value( "TableView/SortPostingsByMaturity", true ).toBool();
        }
        
        void setSortPostingsByMaturity(bool b)
        {
            m_settings.setValue( "TableView/SortPostingsByMaturity", b );
            sync();
        }
        
        QString horizontalHeaderState() const 
        {
            return m_settings.value( "TableView/HorizontalHeaderState" ).toByteArray();
        }
        
        void setHorizontalHeaderState(const QString &str) 
        {
            m_settings.setValue( "TableView/HorizontalHeaderState", str );
            sync();
        }

        bool movableColumns() const
        {
            return m_settings.value( "TableView/MovableColumns", true ).toBool();
        }
        
        void setMovableColumns(bool b)
        {
            m_settings.setValue( "TableView/MovableColumns", b );
            sync();
        }
        
        bool cascadingSectionResize() const
        {
            return m_settings.value( "TableView/CascadingSectionResize", false ).toBool();
        }
        
        void setCascadingSectionResize(bool b)
        {
            m_settings.setValue( "TableView/CascadingSectionResize", b );
            sync();
        }
        
        bool doubleClickResizeColumnToCountent() const
        {
            return m_settings.value( "TableView/DoubleClickResizeColumnToCountent", false ).toBool();
        }
        
        void setDoubleClickResizeColumnToCountent(bool b)
        {
            m_settings.setValue( "TableView/DoubleClickResizeColumnToCountent", b );
            sync();
        }
        
        QString windowGeometry() const
        {
            return m_settings.value( "Window/WindowGeometry" ).toByteArray();
        }
        
        void setWindowGeometry(const QString &str)
        {
            m_settings.setValue( "Window/WindowGeometry", str );
            sync();
        }
        
        QString windowState() const
        {
            return m_settings.value( "Window/WindowState" ).toString();
        }
        
        void setWindowState(const QString &str)
        {
            m_settings.setValue( "Window/WindowState", str );
            sync();
        }
               
        bool movableTabs() const
        {
            return m_settings.value( "Window/MovableTabs", true ).toBool();
        }
        
        void setMovableTabs(bool b)
        {
            m_settings.setValue( "Window/MovableTabs", b );
            sync();
        } 
        
        bool closeButtonOnTabs() const
        {
            return m_settings.value( "Window/CloseButtonOnTabs", true ).toBool();
        }
        
        void setCloseButtonOnTabs(bool b)
        {
            m_settings.setValue( "Window/CloseButtonOnTabs", b );
            sync();
        }
        
          
        bool tabCornerCloseButton() const
        {
            return m_settings.value( "Window/TabCornerCloseButton", true ).toBool();
        }
        
        void setTabCornerCloseButton(bool b)
        {
            m_settings.setValue( "Window/TabCornerCloseButton", b );
            sync();
        }
        
    private:
        Preferences() 
        {
        }
                
        ~Preferences()
        {
        }
        
    private:
        QSettings m_settings;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
