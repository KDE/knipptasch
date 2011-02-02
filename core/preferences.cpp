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
#include "preferences.h"

#include <QStringList>
#include <QColor>
#include <QSize>


#define POSITIVE_AMOUNT_FOREGROUND_COLOR QColor( 0, 140, 0 )
#define NEGATIVE_AMOUNT_FOREGROUND_COLOR QColor( 140, 0, 0 )
#define AVAILABLE_WARRANTY_FOREGROUND_COLOR QColor( 0, 0, 140 )
#define EXPIRED_WARRANTY_FOREGROUND_COLOR QColor( 186, 189, 182 )
#define DEFAULT_POSTING_BACKGROUND_COLOR QColor( 90, 255, 70, 40 )
#define CURRENT_POSTING_BACKGROUND_COLOR QColor( 255, 210, 0, 40 )
#define FUTURE_POSTING_BACKGROUND_COLOR QColor( 100, 200, 255, 40 )
#define INCOMPLETE_POSTING_BACKGROUND_COLOR QColor( 190, 0, 0, 40 )


namespace Knipptasch
{


Preferences::Preferences(QObject *parent)
  : QSettings( QSettings::UserScope, "kde.org", "knipptasch", parent )
{
}


Preferences::~Preferences()
{
}



Preferences::EnumOnStartupAction Preferences::onStartupAction() const
{
    return static_cast<EnumOnStartupAction>( value( "General/OnStartupAction", static_cast<int>( WelcomePage ) ).toInt() );
}


void Preferences::setOnStartupAction(Preferences::EnumOnStartupAction value)
{
    setValue( "General/OnStartupAction", static_cast<int>( value ) );
    sync();
}


QString Preferences::onStartupActionDefaultFile() const
{
    return value( "General/OnStartupActionDefaultFile" ).toString();
}


void Preferences::setOnStartupActionDefaultFile(const QString &fileName)
{
    setValue( "General/OnStartupActionDefaultFile", fileName );
    sync();
}


bool Preferences::showStatusBar() const
{
    return value( "General/ShowStatusBar", true ).toBool();
}


void Preferences::setShowStatusBar(bool state)
{
    setValue( "General/ShowStatusBar", state );
    sync();
}


int Preferences::defaultLengthOfWarrantyInMonth() const
{
    return value( "General/DefaultLengthOfWarrantyInMonth", 24 ).toInt();
}


void Preferences::setDefaultLengthOfWarrantyInMonth(int value)
{
    setValue( "General/DefaultLengthOfWarrantyInMonth", value );
    sync();
}


QStringList Preferences::recentFilesList() const
{
    return value( "General/RecentFilesList" ).toStringList();
}


void Preferences::setRecentFilesList(const QStringList &list)
{
    setValue( "General/RecentFilesList", list );
    sync();
}


int Preferences::recentFilesListMaximumCount() const
{
    return value( "General/RecentFilesMaximumCount", 10 ).toInt();
}


void Preferences::setRecentFilesListMaximumCount(int v)
{
    setValue( "General/RecentFilesMaximumCount", v > 0 ? v : 0 );
    sync();
}


bool Preferences::splashScreenEnabled() const
{
    return value( "General/SplashScreenEnabled", true ).toBool();
}


void Preferences::setSplashScreenEnabled(bool b)
{
    setValue( "General/SplashScreenEnabled", b );
    sync();
}


bool Preferences::hideEmptyTabBar() const
{
    return value( "General/HideEmptyTabBar", false ).toBool();
}


void Preferences::setHideEmptyTabBar(bool b)
{
    setValue( "General/HideEmptyTabBar", b );
    sync();
}


bool Preferences::middleClickCloseTab() const
{
    return value( "General/MiddleClickCloseTab", false ).toBool();
}


void Preferences::setMiddleClickCloseTab(bool b)
{
    setValue( "General/MiddleClickCloseTab", b );
    sync();
}


bool Preferences::movableTabs() const
{
    return value( "General/MovableTabs", true ).toBool();
}


void Preferences::setMovableTabs(bool b)
{
    setValue( "General/MovableTabs", b );
    sync();
}


bool Preferences::closeButtonOnTabs() const
{
    return value( "General/CloseButtonOnTabs", true ).toBool();
}


void Preferences::setCloseButtonOnTabs(bool b)
{
    setValue( "General/CloseButtonOnTabs", b );
    sync();
}


bool Preferences::tabCornerCloseButton() const
{
    return value( "General/TabCornerCloseButton", true ).toBool();
}


void Preferences::setTabCornerCloseButton(bool b)
{
    setValue( "General/TabCornerCloseButton", b );
    sync();
}


// == Appearance ==

bool Preferences::positiveAmountForegroundEnabled() const
{
    return value( "Appearance/PositiveAmountForegroundEnabled", true ).toBool();
}


void Preferences::setPositiveAmountForegroundEnabled(bool b)
{
    setValue( "Appearance/PositiveAmountForegroundEnabled", b );
    sync();
}


QColor Preferences::positiveAmountForegroundColor() const
{
    return value( "Appearance/PositiveAmountForegroundColor", POSITIVE_AMOUNT_FOREGROUND_COLOR ).value<QColor>();
}


void Preferences::setPositiveAmountForegroundColor(const QColor &color)
{
    setValue( "Appearance/PositiveAmountForegroundColor", color );
    sync();
}


bool Preferences::negativeAmountForegroundEnabled() const
{
    return value( "Appearance/NegativeAmountForegroundEnabled", true ).toBool();
}


void Preferences::setNegativeAmountForegroundEnabled(bool b)
{
    setValue( "Appearance/NegativeAmountForegroundEnabled", b );
    sync();
}


QColor Preferences::negativeAmountForegroundColor() const
{
    return value( "Appearance/NegativeAmountForegroundColor", NEGATIVE_AMOUNT_FOREGROUND_COLOR ).value<QColor>();
}


void Preferences::setNegativeAmountForegroundColor(const QColor &color)
{
    setValue( "Appearance/NegativeAmountForegroundColor", color );
    sync();
}

bool Preferences::availableWarrantyForegroundEnabled() const
{
    return value( "Appearance/AvailableWarrantyForegroundEnabled", true ).toBool();
}


void Preferences::setAvailableWarrantyForegroundEnabled(bool b)
{
    setValue( "Appearance/AvailableWarrantyForegroundEnabled", b );
    sync();
}


QColor Preferences::availableWarrantyForegroundColor() const
{
        return value( "Appearance/AvailableWarrantyForegroundColor", AVAILABLE_WARRANTY_FOREGROUND_COLOR ).value<QColor>();
}


void Preferences::setAvailableWarrantyForegroundColor(const QColor &color)
{
    setValue( "Appearance/AvailableWarrantyForegroundColor", color );
    sync();
}


bool Preferences::expiredWarrantyForegroundEnabled() const
{
    return value( "Appearance/ExpiredWarrantyForegroundEnabled", true ).toBool();
}


void Preferences::setExpiredWarrantyForegroundEnabled(bool b)
{
    setValue( "Appearance/ExpiredWarrantyForegroundEnabled", b );
    sync();
}


QColor Preferences::expiredWarrantyForegroundColor() const
{
    return value( "Appearance/ExpiredWarrantyForegroundColor", EXPIRED_WARRANTY_FOREGROUND_COLOR ).value<QColor>();
}


void Preferences::setExpiredWarrantyForegroundColor(const QColor &color)
{
    setValue( "Appearance/ExpiredWarrantyForegroundColor", color );
    sync();
}


bool Preferences::currentPostingBackgroundEnabled() const
{
    return value( "Appearance/CurrentPostingBackgroundEnabled", true ).toBool();
}


void Preferences::setCurrentPostingBackgroundEnabled(bool b)
{
    setValue( "Appearance/CurrentPostingBackgroundEnabled", b );
    sync();
}


QColor Preferences::currentPostingBackgroundColor() const
{
    return value( "Appearance/CurrentPostingBackgroundColor", CURRENT_POSTING_BACKGROUND_COLOR ).value<QColor>();
}


void Preferences::setCurrentPostingBackgroundColor(const QColor &color)
{
    setValue( "Appearance/CurrentPostingBackgroundColor", color );
    sync();
}


bool Preferences::futurePostingBackgroundEnabled() const
{
    return value( "Appearance/FuturePostingBackgroundEnabled", true ).toBool();
}


void Preferences::setFuturePostingBackgroundEnabled(bool b)
{
    setValue( "Appearance/FuturePostingBackgroundEnabled", b );
    sync();
}


QColor Preferences::futurePostingBackgroundColor() const
{
    return value( "Appearance/FuturePostingBackgroundColor", FUTURE_POSTING_BACKGROUND_COLOR ).value<QColor>();
}


void Preferences::setFuturePostingBackgroundColor(const QColor &color)
{
    setValue( "Appearance/FuturePostingBackgroundColor", color );
    sync();
}


bool Preferences::incompletePostingBackgroundEnabled() const
{
    return value( "Appearance/IncompletePostingBackgroundEnabled", true ).toBool();
}


void Preferences::setIncompletePostingBackgroundEnabled(bool b)
{
    setValue( "Appearance/IncompletePostingBackgroundEnabled", b );
    sync();
}


QColor Preferences::incompletePostingBackgroundColor() const
{
    return value( "Appearance/IncompletePostingBackgroundColor", INCOMPLETE_POSTING_BACKGROUND_COLOR ).value<QColor>();
}


void Preferences::setIncompletePostingBackgroundColor(const QColor &color)
{
    setValue( "Appearance/IncompletePostingBackgroundColor", color );
    sync();
}


bool Preferences::defaultPostingBackgroundEnabled() const
{
    return value( "Appearance/DefaultPostingBackgroundEnabled", true ).toBool();
}


void Preferences::setDefaultPostingBackgroundEnabled(bool b)
{
    setValue( "Appearance/DefaultPostingBackgroundEnabled", b );
    sync();
}


QColor Preferences::defaultPostingBackgroundColor() const
{
    return value( "Appearance/DefaultPostingBackgroundColor", DEFAULT_POSTING_BACKGROUND_COLOR ).value<QColor>();
}


void Preferences::setDefaultPostingBackgroundColor(const QColor &color)
{
    setValue( "Appearance/DefaultPostingBackgroundColor", color );
    sync();
}


QSize Preferences::minimumCategoryComboBoxPopupSize() const
{
    return value( "Appearance/MinimumCategoryComboBoxPopupSize", QSize( 250, 300 ) ).value<QSize>();
}


void Preferences::setMinimumCategoryComboBoxPopupSize(const QSize &size)
{
    setValue( "Appearance/MinimumCategoryComboBoxPopupSize", size );
    sync();
}


// == TableView ==


int Preferences::sortPostingsBy() const
{
    return value( "TableView/SortPostingsBy", static_cast<int>( Maturity ) ).toInt();
}


void Preferences::setSortPostingsBy(int value)
{
    setValue( "TableView/SortPostingsBy", value );
    sync();
}


QString Preferences::horizontalHeaderState() const
{
    return value( "TableView/HorizontalHeaderState" ).toByteArray();
}


void Preferences::setHorizontalHeaderState(const QString &str)
{
    setValue( "TableView/HorizontalHeaderState", str );
    sync();
}


bool Preferences::movableColumns() const
{
    return value( "TableView/MovableColumns", true ).toBool();
}


void Preferences::setMovableColumns(bool b)
{
    setValue( "TableView/MovableColumns", b );
    sync();
}


bool Preferences::cascadingSectionResize() const
{
    return value( "TableView/CascadingSectionResize", false ).toBool();
}


void Preferences::setCascadingSectionResize(bool b)
{
    setValue( "TableView/CascadingSectionResize", b );
    sync();
}


bool Preferences::doubleClickResizeColumnToCountent() const
{
    return value( "TableView/DoubleClickResizeColumnToCountent", false ).toBool();
}


void Preferences::setDoubleClickResizeColumnToCountent(bool b)
{
    setValue( "TableView/DoubleClickResizeColumnToCountent", b );
    sync();
}


bool Preferences::resetCurrentIndexWhenCurrentRowChanged()
{
    return value( "TableView/ResetCurrentIndexWhenCurrentRowChanged", true ).toBool();
}


void Preferences::setResetCurrentIndexWhenCurrentRowChanged(bool b)
{
    setValue( "TableView/ResetCurrentIndexWhenCurrentRowChanged", b );
    sync();
}


QString Preferences::userDefinedDateFormat() const
{
    return value( "TableView/UserDefinedDateFormat" ).toString();
}


void Preferences::setUserDefinedDateFormat(const QString &str)
{
    setValue( "TableView/UserDefinedDateFormat", str );
    sync();
}


bool Preferences::autoCompletionEnabled()
{
    return value( "TableView/AutoCompletionEnabled", true ).toBool();
}


void Preferences::setAutoCompletionEnabled(bool b)
{
    setValue( "TableView/AutoCompletionEnabled", b );
    sync();
}


} // EndNamspace Knipptasch

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
