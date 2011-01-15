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
#include "preferences.h"

#include <QDebug>


PreferencesAppearanceConfigPage::PreferencesAppearanceConfigPage(ConfigWidget* parent)
  : AbstractConfigPage( tr( "Appearance" ), DesktopIcon("preferences-desktop-theme"), parent ),
    ui( new Ui::PreferencesAppearanceConfigPage )
{
    ui->setupUi( this );

    connect( ui->positiveAmountForegroundEnabled, SIGNAL( toggled(bool) ),
             ui->fgPositiveAmountWidget, SLOT( setEditable(bool) ) );
    connect( ui->negativeAmountForegroundEnabled, SIGNAL( toggled(bool) ),
             ui->fgNegativeAmountWidget, SLOT( setEditable(bool) ) );
    connect( ui->availableWarrantyForegroundEnabled, SIGNAL( toggled(bool) ),
             ui->fgAvailableWarrantyWidget, SLOT( setEditable(bool) ) );
    connect( ui->expiredWarrantyForegroundEnabled, SIGNAL( toggled(bool) ),
             ui->fgExpiredWarrantyWidget, SLOT( setEditable(bool) ) );
    connect( ui->currentPostingBackgroundEnabled, SIGNAL( toggled(bool) ),
             ui->bgCurrentPostingWidget, SLOT( setEditable(bool) ) );
    connect( ui->futurePostingBackgroundEnabled, SIGNAL( toggled(bool) ),
             ui->bgFuturePostingWidget, SLOT( setEditable(bool) ) );
    connect( ui->defaultPostingBackgroundEnabled, SIGNAL( toggled(bool) ),
             ui->bgNormalPostingWidget, SLOT( setEditable(bool) ) );
    connect( ui->incompletePostingBackgroundEnabled, SIGNAL( toggled(bool) ),
             ui->bgIncompletePostingWidget, SLOT( setEditable(bool) ) );

    connect( ui->positiveAmountForegroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->negativeAmountForegroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->availableWarrantyForegroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->expiredWarrantyForegroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->currentPostingBackgroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->futurePostingBackgroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->defaultPostingBackgroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->incompletePostingBackgroundEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->fgPositiveAmountWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->fgNegativeAmountWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->fgAvailableWarrantyWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->fgExpiredWarrantyWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgCurrentPostingWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgFuturePostingWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgNormalPostingWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );
    connect( ui->bgIncompletePostingWidget, SIGNAL( colorChanged(QColor) ),
             this, SIGNAL( pageModified() ) );

    revert();
}


PreferencesAppearanceConfigPage::~PreferencesAppearanceConfigPage()
{
    delete ui;
}


bool PreferencesAppearanceConfigPage::isModified() const
{
    Preferences *p = Preferences::self();

    if( ui->positiveAmountForegroundEnabled->isChecked() != p->positiveAmountForegroundEnabled() ) {
        return true;
    }

    if( ui->negativeAmountForegroundEnabled->isChecked() != p->negativeAmountForegroundEnabled() ) {
        return true;
    }

    if( ui->availableWarrantyForegroundEnabled->isChecked() != p->availableWarrantyForegroundEnabled() ) {
        return true;
    }

    if( ui->expiredWarrantyForegroundEnabled->isChecked() != p->expiredWarrantyForegroundEnabled() ) {
        return true;
    }

    if( ui->currentPostingBackgroundEnabled->isChecked() != p->currentPostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->futurePostingBackgroundEnabled->isChecked() != p->futurePostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->defaultPostingBackgroundEnabled->isChecked() != p->defaultPostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->incompletePostingBackgroundEnabled->isChecked() != p->incompletePostingBackgroundEnabled() ) {
        return true;
    }

    if( ui->fgPositiveAmountWidget->color() != p->positiveAmountForegroundColor() ) {
        return true;
    }

    if( ui->fgNegativeAmountWidget->color() != p->negativeAmountForegroundColor() ) {
        return true;
    }

    if( ui->fgAvailableWarrantyWidget->color() != p->availableWarrantyForegroundColor() ) {
        return true;
    }

    if( ui->fgExpiredWarrantyWidget->color() != p->expiredWarrantyForegroundColor() ) {
        return true;
    }

    if( ui->bgCurrentPostingWidget->color() != p->currentPostingBackgroundColor() ) {
        return true;
    }

    if( ui->bgFuturePostingWidget->color() != p->futurePostingBackgroundColor() ) {
        return true;
    }

    if( ui->bgNormalPostingWidget->color() != p->defaultPostingBackgroundColor() ) {
        return true;
    }

    if( ui->bgIncompletePostingWidget->color() != p->incompletePostingBackgroundColor() ) {
        return true;
    }

    return false;
}


bool PreferencesAppearanceConfigPage::commit()
{
    Preferences *p = Preferences::self();
    p->setPositiveAmountForegroundEnabled( ui->positiveAmountForegroundEnabled->checkState() );
    p->setPositiveAmountForegroundColor( ui->fgPositiveAmountWidget->color() );
    p->setNegativeAmountForegroundEnabled( ui->negativeAmountForegroundEnabled->isChecked() );
    p->setNegativeAmountForegroundColor( ui->fgNegativeAmountWidget->color() );
    p->setAvailableWarrantyForegroundEnabled( ui->availableWarrantyForegroundEnabled->isChecked() );
    p->setAvailableWarrantyForegroundColor( ui->fgAvailableWarrantyWidget->color() );
    p->setExpiredWarrantyForegroundEnabled( ui->availableWarrantyForegroundEnabled->isChecked() );
    p->setExpiredWarrantyForegroundColor( ui->fgExpiredWarrantyWidget->color() );
    p->setCurrentPostingBackgroundEnabled( ui->currentPostingBackgroundEnabled->isChecked() );
    p->setCurrentPostingBackgroundColor( ui->bgCurrentPostingWidget->color() );
    p->setFuturePostingBackgroundEnabled( ui->futurePostingBackgroundEnabled->isChecked() );
    p->setFuturePostingBackgroundColor( ui->bgFuturePostingWidget->color() );
    p->setDefaultPostingBackgroundEnabled( ui->defaultPostingBackgroundEnabled->isChecked() );
    p->setDefaultPostingBackgroundColor( ui->bgNormalPostingWidget->color() );
    p->setIncompletePostingBackgroundEnabled( ui->incompletePostingBackgroundEnabled->isChecked() );
    p->setIncompletePostingBackgroundColor( ui->bgIncompletePostingWidget->color() );

    emit pageModified();

    return true;
}


void PreferencesAppearanceConfigPage::revert()
{
    bool block = blockSignals( true );

    Preferences *p = Preferences::self();
    ui->positiveAmountForegroundEnabled->setChecked( p->positiveAmountForegroundEnabled() );
    ui->negativeAmountForegroundEnabled->setChecked( p->negativeAmountForegroundEnabled() );
    ui->availableWarrantyForegroundEnabled->setChecked( p->availableWarrantyForegroundEnabled() );
    ui->expiredWarrantyForegroundEnabled->setChecked( p->expiredWarrantyForegroundEnabled() );
    ui->currentPostingBackgroundEnabled->setChecked( p->currentPostingBackgroundEnabled() );
    ui->futurePostingBackgroundEnabled->setChecked( p->futurePostingBackgroundEnabled() );
    ui->defaultPostingBackgroundEnabled->setChecked( p->defaultPostingBackgroundEnabled() );
    ui->incompletePostingBackgroundEnabled->setChecked( p->incompletePostingBackgroundEnabled() );

    ui->fgPositiveAmountWidget->setColor( p->positiveAmountForegroundColor() );
    ui->fgNegativeAmountWidget->setColor( p->negativeAmountForegroundColor() );
    ui->fgAvailableWarrantyWidget->setColor( p->availableWarrantyForegroundColor() );
    ui->fgExpiredWarrantyWidget->setColor( p->expiredWarrantyForegroundColor() );
    ui->bgCurrentPostingWidget->setColor( p->currentPostingBackgroundColor() );
    ui->bgFuturePostingWidget->setColor( p->futurePostingBackgroundColor() );
    ui->bgNormalPostingWidget->setColor( p->defaultPostingBackgroundColor() );
    ui->bgIncompletePostingWidget->setColor( p->incompletePostingBackgroundColor() );

    blockSignals( block );

    emit pageModified();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
