/*
 * Copyright 2010, 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef KNIPPTASCH_CORE_PREFERENCES_H
#define KNIPPTASCH_CORE_PREFERENCES_H

#include "knipptasch_core_export.h"

#include <QSettings>


/**
 * @namespace Knipptasch
 * @brief Default namespace for Knipptasch
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
namespace Knipptasch
{

    /**
     * @class Preferences
     * @ingroup Core
     * @brief
     *
     * @author Stefan Böhmann <kde@hilefoks.org>
     */
    class KNIPPTASCH_CORE_EXPORT Preferences : public QSettings
    {
        public:
            enum PostingSortOrder { Maturity, ValueDate };
            enum EnumOnStartupAction { WelcomePage, BlankFile, LastFile, DefaultFile };

            static Preferences *self();

            explicit Preferences( QObject *parent = 0 );


            QStringList pluginDirectories() const;

            QStringList pluginNamePrefixes() const;

            // == General ==

            EnumOnStartupAction onStartupAction() const;
            void setOnStartupAction( EnumOnStartupAction value );

            QString onStartupActionDefaultFile() const;
            void setOnStartupActionDefaultFile( const QString &fileName );

            bool showStatusBar() const;
            void setShowStatusBar( bool state );

            int defaultLengthOfWarrantyInMonth() const;
            void setDefaultLengthOfWarrantyInMonth( int value );

            QStringList recentFilesList() const;
            void setRecentFilesList( const QStringList &list );

            int recentFilesListMaximumCount() const;
            void setRecentFilesListMaximumCount( int v );

            bool splashScreenEnabled() const;
            void setSplashScreenEnabled( bool b );

            bool hideEmptyTabBar() const;
            void setHideEmptyTabBar( bool b );

            bool middleClickCloseTab() const;
            void setMiddleClickCloseTab( bool b );

            bool movableTabs() const;
            void setMovableTabs( bool b );

            bool closeButtonOnTabs() const;
            void setCloseButtonOnTabs( bool b );

            bool tabCornerCloseButton() const;
            void setTabCornerCloseButton( bool b );

            // == Appearance ==

            bool positiveAmountForegroundEnabled() const;
            void setPositiveAmountForegroundEnabled( bool b );

            QColor positiveAmountForegroundColor() const;
            void setPositiveAmountForegroundColor( const QColor &color );

            bool negativeAmountForegroundEnabled() const;
            void setNegativeAmountForegroundEnabled( bool b );

            QColor negativeAmountForegroundColor() const;
            void setNegativeAmountForegroundColor( const QColor &color );

            bool availableWarrantyForegroundEnabled() const;
            void setAvailableWarrantyForegroundEnabled( bool b );

            QColor availableWarrantyForegroundColor() const;
            void setAvailableWarrantyForegroundColor( const QColor &color );

            bool expiredWarrantyForegroundEnabled() const;
            void setExpiredWarrantyForegroundEnabled( bool b );

            QColor expiredWarrantyForegroundColor() const;
            void setExpiredWarrantyForegroundColor( const QColor &color );

            bool currentPostingBackgroundEnabled() const;
            void setCurrentPostingBackgroundEnabled( bool b );

            QColor currentPostingBackgroundColor() const;
            void setCurrentPostingBackgroundColor( const QColor &color );

            bool futurePostingBackgroundEnabled() const;
            void setFuturePostingBackgroundEnabled( bool b );

            QColor futurePostingBackgroundColor() const;
            void setFuturePostingBackgroundColor( const QColor &color );

            bool incompletePostingBackgroundEnabled() const;
            void setIncompletePostingBackgroundEnabled( bool b );

            QColor incompletePostingBackgroundColor() const;
            void setIncompletePostingBackgroundColor( const QColor &color );

            bool defaultPostingBackgroundEnabled() const;
            void setDefaultPostingBackgroundEnabled( bool b );

            QColor defaultPostingBackgroundColor() const;
            void setDefaultPostingBackgroundColor( const QColor &color );

            QSize minimumCategoryComboBoxPopupSize() const;
            void setMinimumCategoryComboBoxPopupSize( const QSize &size );

            // == TableView ==

            PostingSortOrder postingSortOrder() const;
            void setPostingSortOrder( PostingSortOrder value );

            QString horizontalHeaderState() const;
            void setHorizontalHeaderState( const QString &str );

            bool movableColumns() const;
            void setMovableColumns( bool b );

            bool cascadingSectionResize() const;
            void setCascadingSectionResize( bool b );

            bool doubleClickResizeColumnToCountent() const;
            void setDoubleClickResizeColumnToCountent( bool b );

            bool resetCurrentIndexWhenCurrentRowChanged();
            void setResetCurrentIndexWhenCurrentRowChanged( bool b );

            QString userDefinedDateFormat() const;
            void setUserDefinedDateFormat( const QString &str );

            bool autoCompletionEnabled();
            void setAutoCompletionEnabled( bool b );

            // == MainWindow ==

            QString windowGeometry() const;
            void setWindowGeometry( const QString &str );

            QString windowState() const;
            void setWindowState( const QString &str );
    };

} // EndNamspace Knipptasch

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
