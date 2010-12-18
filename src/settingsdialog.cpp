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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "compat/iconloader.h"

#include "preferences.h"

#include <QPushButton>

#if defined(HAVE_KDE)
#include <kdeversion.h>
#endif

#ifndef KDE_IS_VERSION
#define KDE_IS_VERSION(a,b,c) 1
#endif


SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog( parent ),
    ui( new Ui::SettingsDialog )
{
    ui->setupUi( this );

    setWindowTitle( tr( "Configure Knipptasch - %1" ).arg( QCoreApplication::applicationName() ) );

    QIcon icon = DesktopIcon( QCoreApplication::applicationName().toLower() );
    if( icon.isNull() ) {
        icon = qApp->windowIcon();
    }
    ui->iconLabel->setPixmap( icon.pixmap( 64, 64 ) );

#if KDE_IS_VERSION(4,5,0)
    ui->positiveAmountForegroundColor->setAlphaChannelEnabled( true );
    ui->negativeAmountForegroundColor->setAlphaChannelEnabled( true );
    ui->availableWarrantyForegroundColor->setAlphaChannelEnabled( true );
    ui->expiredWarrantyForegroundColor->setAlphaChannelEnabled( true );
    ui->currentPostingBackgroundColor->setAlphaChannelEnabled( true );
    ui->futurePostingBackgroundColor->setAlphaChannelEnabled( true );
    ui->defaultPostingBackgroundColor->setAlphaChannelEnabled( true );
    ui->incompletePostingBackgroundColor->setAlphaChannelEnabled( true );
#endif

    connect( ui->positiveAmountForegroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->negativeAmountForegroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->availableWarrantyForegroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->expiredWarrantyForegroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->currentPostingBackgroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->futurePostingBackgroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->defaultPostingBackgroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->incompletePostingBackgroundEnabled, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->positiveAmountForegroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->negativeAmountForegroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->availableWarrantyForegroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->expiredWarrantyForegroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->currentPostingBackgroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->futurePostingBackgroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->defaultPostingBackgroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );
    connect( ui->incompletePostingBackgroundColor, SIGNAL( changed(QColor) ), this, SLOT( onValueChanged() ) );

    connect( ui->buttonBox, SIGNAL( accepted() ), this, SLOT( onApplyChanges() ) );
    connect( ui->buttonBox->button( QDialogButtonBox::Apply ), SIGNAL( clicked(bool) ), this, SLOT( onApplyChanges() ) );
    connect( ui->buttonBox->button( QDialogButtonBox::RestoreDefaults ), SIGNAL( clicked(bool) ), this, SLOT( onRestoreDefaults() ) );

    loadPreferences();
}


SettingsDialog::~SettingsDialog()
{
    delete ui;
}


void SettingsDialog::onValueChanged()
{
    ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( true );

    do {
        if( ui->positiveAmountForegroundEnabled->isChecked() !=
            Preferences::self()->positiveAmountForegroundEnabled()
        ) break;

        if( ui->negativeAmountForegroundEnabled->isChecked() !=
            Preferences::self()->negativeAmountForegroundEnabled()
        ) break;

        if( ui->availableWarrantyForegroundEnabled->isChecked() !=
            Preferences::self()->availableWarrantyForegroundEnabled()
        ) break;

        if( ui->expiredWarrantyForegroundEnabled->isChecked() !=
            Preferences::self()->expiredWarrantyForegroundEnabled()
        ) break;

        if( ui->currentPostingBackgroundEnabled->isChecked() !=
            Preferences::self()->currentPostingBackgroundEnabled()
        ) break;

        if( ui->futurePostingBackgroundEnabled->isChecked() !=
            Preferences::self()->futurePostingBackgroundEnabled()
        ) break;

        if( ui->defaultPostingBackgroundEnabled->isChecked() !=
            Preferences::self()->defaultPostingBackgroundEnabled()
        ) break;

        if( ui->incompletePostingBackgroundEnabled->isChecked() !=
            Preferences::self()->incompletePostingBackgroundEnabled()
        ) break;

        if( ui->positiveAmountForegroundColor->color() !=
            Preferences::self()->positiveAmountForegroundColor()
        ) break;

        if( ui->negativeAmountForegroundColor->color() !=
            Preferences::self()->negativeAmountForegroundColor()
        ) break;

        if( ui->availableWarrantyForegroundColor->color() !=
            Preferences::self()->availableWarrantyForegroundColor()
        ) break;

        if( ui->expiredWarrantyForegroundColor->color() !=
            Preferences::self()->expiredWarrantyForegroundColor()
        ) break;

        if( ui->currentPostingBackgroundColor->color() !=
            Preferences::self()->currentPostingBackgroundColor()
        ) break;

        if( ui->futurePostingBackgroundColor->color() !=
            Preferences::self()->futurePostingBackgroundColor()
        ) break;

        if( ui->defaultPostingBackgroundColor->color() !=
            Preferences::self()->defaultPostingBackgroundColor()
        ) break;

        if( ui->incompletePostingBackgroundColor->color() !=
            Preferences::self()->incompletePostingBackgroundColor()
        ) break;

        ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    } while( false );
}


void SettingsDialog::onApplyChanges()
{
    Preferences::self()->setPositiveAmountForegroundEnabled(
        ui->positiveAmountForegroundEnabled->isChecked() );

    Preferences::self()->setPositiveAmountForegroundColor(
        ui->positiveAmountForegroundColor->color() );

    Preferences::self()->setNegativeAmountForegroundEnabled(
        ui->negativeAmountForegroundEnabled->isChecked() );

    Preferences::self()->setNegativeAmountForegroundColor(
        ui->negativeAmountForegroundColor->color() );

    Preferences::self()->setAvailableWarrantyForegroundEnabled(
        ui->availableWarrantyForegroundEnabled->isChecked() );

    Preferences::self()->setAvailableWarrantyForegroundColor(
        ui->availableWarrantyForegroundColor->color() );

    Preferences::self()->setExpiredWarrantyForegroundEnabled(
        ui->availableWarrantyForegroundEnabled->isChecked() );

    Preferences::self()->setExpiredWarrantyForegroundColor(
        ui->expiredWarrantyForegroundColor->color() );

    Preferences::self()->setCurrentPostingBackgroundEnabled(
        ui->currentPostingBackgroundEnabled->isChecked() );

    Preferences::self()->setCurrentPostingBackgroundColor(
        ui->currentPostingBackgroundColor->color() );

    Preferences::self()->setFuturePostingBackgroundEnabled(
        ui->futurePostingBackgroundEnabled->isChecked() );

    Preferences::self()->setFuturePostingBackgroundColor(
        ui->futurePostingBackgroundColor->color() );

    Preferences::self()->setDefaultPostingBackgroundEnabled(
        ui->defaultPostingBackgroundEnabled->isChecked() );

    Preferences::self()->setDefaultPostingBackgroundColor(
        ui->defaultPostingBackgroundColor->color() );

    Preferences::self()->setIncompletePostingBackgroundEnabled(
        ui->incompletePostingBackgroundEnabled->isChecked() );

    Preferences::self()->setIncompletePostingBackgroundColor(
        ui->incompletePostingBackgroundColor->color() );

    onValueChanged();
}


void SettingsDialog::onRestoreDefaults()
{
    ui->positiveAmountForegroundEnabled->setChecked( true );
    ui->negativeAmountForegroundEnabled->setChecked( true );
    ui->availableWarrantyForegroundEnabled->setChecked( true );
    ui->expiredWarrantyForegroundEnabled->setChecked( true );
    ui->currentPostingBackgroundEnabled->setChecked( true );
    ui->futurePostingBackgroundEnabled->setChecked( true );
    ui->defaultPostingBackgroundEnabled->setChecked( true );
    ui->incompletePostingBackgroundEnabled->setChecked( true );

#if defined(HAVE_KDE)
    Preferences::self()->positiveAmountForegroundColorItem()->setDefault();
    Preferences::self()->negativeAmountForegroundColorItem()->setDefault();
    Preferences::self()->availableWarrantyForegroundColorItem()->setDefault();
    Preferences::self()->expiredWarrantyForegroundColorItem()->setDefault();
    Preferences::self()->currentPostingBackgroundColorItem()->setDefault();
    Preferences::self()->futurePostingBackgroundColorItem()->setDefault();
    Preferences::self()->defaultPostingBackgroundColorItem()->setDefault();
    Preferences::self()->incompletePostingBackgroundColorItem()->setDefault();
    
    loadPreferences();
#else
    ui->positiveAmountForegroundColor->setColor( POSITIVE_AMOUNT_FOREGROUND_COLOR );
    ui->negativeAmountForegroundColor->setColor( NEGATIVE_AMOUNT_FOREGROUND_COLOR );
    ui->availableWarrantyForegroundColor->setColor( AVAILABLE_WARRANTY_FOREGROUND_COLOR );
    ui->expiredWarrantyForegroundColor->setColor( EXPIRED_WARRANTY_FOREGROUND_COLOR );
    ui->currentPostingBackgroundColor->setColor( CURRENT_POSTING_BACKGROUND_COLOR );
    ui->futurePostingBackgroundColor->setColor( FUTURE_POSTING_BACKGROUND_COLOR );
    ui->defaultPostingBackgroundColor->setColor( DEFAULT_POSTING_BACKGROUND_COLOR );
    ui->incompletePostingBackgroundColor->setColor( INCOMPLETE_POSTING_BACKGROUND_COLOR );
    
    onValueChanged();
#endif
}


void SettingsDialog::loadPreferences()
{
    ui->positiveAmountForegroundEnabled->setChecked(
        Preferences::self()->positiveAmountForegroundEnabled()
    );

    ui->negativeAmountForegroundEnabled->setChecked(
        Preferences::self()->negativeAmountForegroundEnabled()
    );

    ui->availableWarrantyForegroundEnabled->setChecked(
        Preferences::self()->availableWarrantyForegroundEnabled()
    );

    ui->expiredWarrantyForegroundEnabled->setChecked(
        Preferences::self()->expiredWarrantyForegroundEnabled()
    );

    ui->currentPostingBackgroundEnabled->setChecked(
        Preferences::self()->currentPostingBackgroundEnabled()
    );

    ui->futurePostingBackgroundEnabled->setChecked(
        Preferences::self()->futurePostingBackgroundEnabled()
    );

    ui->defaultPostingBackgroundEnabled->setChecked(
        Preferences::self()->defaultPostingBackgroundEnabled()
    );

    ui->incompletePostingBackgroundEnabled->setChecked(
        Preferences::self()->incompletePostingBackgroundEnabled()
    );

    ui->positiveAmountForegroundColor->setColor(
        Preferences::self()->positiveAmountForegroundColor()
    );

    ui->negativeAmountForegroundColor->setColor(
        Preferences::self()->negativeAmountForegroundColor()
    );

    ui->availableWarrantyForegroundColor->setColor(
        Preferences::self()->availableWarrantyForegroundColor()
    );

    ui->expiredWarrantyForegroundColor->setColor(
        Preferences::self()->expiredWarrantyForegroundColor()
    );

    ui->currentPostingBackgroundColor->setColor(
        Preferences::self()->currentPostingBackgroundColor()
    );

    ui->futurePostingBackgroundColor->setColor(
        Preferences::self()->futurePostingBackgroundColor()
    );

    ui->defaultPostingBackgroundColor->setColor(
        Preferences::self()->defaultPostingBackgroundColor()
    );

    ui->incompletePostingBackgroundColor->setColor(
        Preferences::self()->incompletePostingBackgroundColor()
    );

    onValueChanged();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
