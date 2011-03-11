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
#include "preferencesappearanceconfigpage.h"
#include "ui_preferencesappearanceconfigpage.h"

#include "compat/iconloader.h"

#include <Knipptasch/Preferences>

#include <QDebug>


PreferencesAppearanceConfigPage::PreferencesAppearanceConfigPage( Knipptasch::Preferences *pref, ConfigWidget *parent )
    : AbstractConfigPage( tr( "Appearance" ), DesktopIcon( "preferences-desktop-theme" ), parent ),
      ui( new Ui::PreferencesAppearanceConfigPage ),
      m_preferences( pref )
{
    ui->setupUi( this );

    connect( ui->positiveAmountForegroundEnabled, SIGNAL( toggled( bool ) ),
             ui->fgPositiveAmountWidget, SLOT( setEditable( bool ) ) );
    connect( ui->negativeAmountForegroundEnabled, SIGNAL( toggled( bool ) ),
             ui->fgNegativeAmountWidget, SLOT( setEditable( bool ) ) );
    connect( ui->availableWarrantyForegroundEnabled, SIGNAL( toggled( bool ) ),
             ui->fgAvailableWarrantyWidget, SLOT( setEditable( bool ) ) );
    connect( ui->expiredWarrantyForegroundEnabled, SIGNAL( toggled( bool ) ),
             ui->fgExpiredWarrantyWidget, SLOT( setEditable( bool ) ) );
    connect( ui->currentPostingBackgroundEnabled, SIGNAL( toggled( bool ) ),
             ui->bgCurrentPostingWidget, SLOT( setEditable( bool ) ) );
    connect( ui->futurePostingBackgroundEnabled, SIGNAL( toggled( bool ) ),
             ui->bgFuturePostingWidget, SLOT( setEditable( bool ) ) );
    connect( ui->defaultPostingBackgroundEnabled, SIGNAL( toggled( bool ) ),
             ui->bgNormalPostingWidget, SLOT( setEditable( bool ) ) );
    connect( ui->incompletePostingBackgroundEnabled, SIGNAL( toggled( bool ) ),
             ui->bgIncompletePostingWidget, SLOT( setEditable( bool ) ) );

    connect( ui->positiveAmountForegroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->negativeAmountForegroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->availableWarrantyForegroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->expiredWarrantyForegroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->currentPostingBackgroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->futurePostingBackgroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->defaultPostingBackgroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->incompletePostingBackgroundEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->fgPositiveAmountWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->fgNegativeAmountWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->fgAvailableWarrantyWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->fgExpiredWarrantyWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgCurrentPostingWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgFuturePostingWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgNormalPostingWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgIncompletePostingWidget, SIGNAL( colorChanged( QColor ) ),
             this, SIGNAL( pageModified() ) );

    revert();
}


PreferencesAppearanceConfigPage::~PreferencesAppearanceConfigPage()
{
    delete ui;
}


bool PreferencesAppearanceConfigPage::isModified() const
{
    if( ui->positiveAmountForegroundEnabled->isChecked() != m_preferences->positiveAmountForegroundEnabled() ) {
        return true;
    }

    if( ui->negativeAmountForegroundEnabled->isChecked() != m_preferences->negativeAmountForegroundEnabled() ) {
        return true;
    }

    if( ui->availableWarrantyForegroundEnabled->isChecked() != m_preferences->availableWarrantyForegroundEnabled() ) {
        return true;
    }

    if( ui->expiredWarrantyForegroundEnabled->isChecked() != m_preferences->expiredWarrantyForegroundEnabled() ) {
        return true;
    }

    if( ui->currentPostingBackgroundEnabled->isChecked() != m_preferences->currentPostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->futurePostingBackgroundEnabled->isChecked() != m_preferences->futurePostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->defaultPostingBackgroundEnabled->isChecked() != m_preferences->defaultPostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->incompletePostingBackgroundEnabled->isChecked() != m_preferences->incompletePostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->fgPositiveAmountWidget->color() != m_preferences->positiveAmountForegroundColor() ) {
        return true;
    }

    if( ui->fgNegativeAmountWidget->color() != m_preferences->negativeAmountForegroundColor() ) {
        return true;
    }

    if( ui->fgAvailableWarrantyWidget->color() != m_preferences->availableWarrantyForegroundColor() ) {
        return true;
    }

    if( ui->fgExpiredWarrantyWidget->color() != m_preferences->expiredWarrantyForegroundColor() ) {
        return true;
    }

    if( ui->bgCurrentPostingWidget->color() != m_preferences->currentPostingBackgroundColor() ) {
        return true;
    }

    if( ui->bgFuturePostingWidget->color() != m_preferences->futurePostingBackgroundColor() ) {
        return true;
    }

    if( ui->bgNormalPostingWidget->color() != m_preferences->defaultPostingBackgroundColor() ) {
        return true;
    }

    if( ui->bgIncompletePostingWidget->color() != m_preferences->incompletePostingBackgroundColor() ) {
        return true;
    }

    return false;
}


bool PreferencesAppearanceConfigPage::commit()
{
    m_preferences->setPositiveAmountForegroundEnabled( ui->positiveAmountForegroundEnabled->checkState() );
    m_preferences->setPositiveAmountForegroundColor( ui->fgPositiveAmountWidget->color() );
    m_preferences->setNegativeAmountForegroundEnabled( ui->negativeAmountForegroundEnabled->isChecked() );
    m_preferences->setNegativeAmountForegroundColor( ui->fgNegativeAmountWidget->color() );
    m_preferences->setAvailableWarrantyForegroundEnabled( ui->availableWarrantyForegroundEnabled->isChecked() );
    m_preferences->setAvailableWarrantyForegroundColor( ui->fgAvailableWarrantyWidget->color() );
    m_preferences->setExpiredWarrantyForegroundEnabled( ui->availableWarrantyForegroundEnabled->isChecked() );
    m_preferences->setExpiredWarrantyForegroundColor( ui->fgExpiredWarrantyWidget->color() );
    m_preferences->setCurrentPostingBackgroundEnabled( ui->currentPostingBackgroundEnabled->isChecked() );
    m_preferences->setCurrentPostingBackgroundColor( ui->bgCurrentPostingWidget->color() );
    m_preferences->setFuturePostingBackgroundEnabled( ui->futurePostingBackgroundEnabled->isChecked() );
    m_preferences->setFuturePostingBackgroundColor( ui->bgFuturePostingWidget->color() );
    m_preferences->setDefaultPostingBackgroundEnabled( ui->defaultPostingBackgroundEnabled->isChecked() );
    m_preferences->setDefaultPostingBackgroundColor( ui->bgNormalPostingWidget->color() );
    m_preferences->setIncompletePostingBackgroundEnabled( ui->incompletePostingBackgroundEnabled->isChecked() );
    m_preferences->setIncompletePostingBackgroundColor( ui->bgIncompletePostingWidget->color() );

    emit pageModified();

    return true;
}


void PreferencesAppearanceConfigPage::revert()
{
    bool block = blockSignals( true );

    ui->positiveAmountForegroundEnabled->setChecked( m_preferences->positiveAmountForegroundEnabled() );
    ui->negativeAmountForegroundEnabled->setChecked( m_preferences->negativeAmountForegroundEnabled() );
    ui->availableWarrantyForegroundEnabled->setChecked( m_preferences->availableWarrantyForegroundEnabled() );
    ui->expiredWarrantyForegroundEnabled->setChecked( m_preferences->expiredWarrantyForegroundEnabled() );
    ui->currentPostingBackgroundEnabled->setChecked( m_preferences->currentPostingBackgroundEnabled() );
    ui->futurePostingBackgroundEnabled->setChecked( m_preferences->futurePostingBackgroundEnabled() );
    ui->defaultPostingBackgroundEnabled->setChecked( m_preferences->defaultPostingBackgroundEnabled() );
    ui->incompletePostingBackgroundEnabled->setChecked( m_preferences->incompletePostingBackgroundEnabled() );

    ui->fgPositiveAmountWidget->setColor( m_preferences->positiveAmountForegroundColor() );
    ui->fgNegativeAmountWidget->setColor( m_preferences->negativeAmountForegroundColor() );
    ui->fgAvailableWarrantyWidget->setColor( m_preferences->availableWarrantyForegroundColor() );
    ui->fgExpiredWarrantyWidget->setColor( m_preferences->expiredWarrantyForegroundColor() );
    ui->bgCurrentPostingWidget->setColor( m_preferences->currentPostingBackgroundColor() );
    ui->bgFuturePostingWidget->setColor( m_preferences->futurePostingBackgroundColor() );
    ui->bgNormalPostingWidget->setColor( m_preferences->defaultPostingBackgroundColor() );
    ui->bgIncompletePostingWidget->setColor( m_preferences->incompletePostingBackgroundColor() );

    blockSignals( block );

    emit pageModified();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
