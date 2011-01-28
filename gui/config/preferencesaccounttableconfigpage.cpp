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
#include "preferencesaccounttableconfigpage.h"
#include "ui_preferencesaccounttableconfigpage.h"

#include "compat/iconloader.h"
#include "preferences.h"

#include <QDebug>
#include <compat/utils.h>


PreferencesAccountTableConfigPage::PreferencesAccountTableConfigPage(ConfigWidget* parent)
  : AbstractConfigPage( tr( "Account Table" ), tr( "Account Table Settings" ), DesktopIcon("table"), parent ),
    ui( new Ui::PreferencesAccountTableConfigPage )
{
    ui->setupUi( this );

    ui->sortPostingsBy->clear();
    ui->sortPostingsBy->addItem( tr( "Maturity" ), Preferences::Maturity );
    ui->sortPostingsBy->addItem( tr( "Value Date" ), Preferences::ValueDate );

    connect( ui->sortPostingsBy, SIGNAL( currentIndexChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->defaultWarrantyPeriod, SIGNAL( valueChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->movableColumns, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->cascadingSectionResize, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->doubleClickResizeColumnToContent, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->resetCurrentColumnOnRowChanged, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->autoCompletionEnabled, SIGNAL( stateChanged(int) ),
             this, SIGNAL( pageModified() ) );

    revert();
}


PreferencesAccountTableConfigPage::~PreferencesAccountTableConfigPage()
{
    delete ui;
}


bool PreferencesAccountTableConfigPage::isModified() const
{
    Preferences *p = Preferences::self();

    int sortPostingBy = ui->sortPostingsBy->itemData( ui->sortPostingsBy->currentIndex() ).toInt();
    if( sortPostingBy != p->sortPostingsBy() ) {
        return true;
    }

    if( ui->defaultWarrantyPeriod->value() != p->defaultLengthOfWarrantyInMonth() ) {
        return true;
    }

    if( ui->movableColumns->isChecked() != p->movableColumns() ) {
        return true;
    }

    if( ui->cascadingSectionResize->isChecked() != p->cascadingSectionResize() ) {
        return true;
    }

    if( ui->doubleClickResizeColumnToContent->isChecked() != p->doubleClickResizeColumnToCountent() ) {
        return true;
    }

    if( ui->resetCurrentColumnOnRowChanged->isChecked() != p->resetCurrentIndexWhenCurrentRowChanged() ) {
        return true;
    }

    if( ui->autoCompletionEnabled->isChecked() != p->autoCompletionEnabled() ) {
        return true;
    }

    return false;
}


bool PreferencesAccountTableConfigPage::commit()
{
    Preferences *p = Preferences::self();

    p->setSortPostingsBy( ui->sortPostingsBy->itemData( ui->sortPostingsBy->currentIndex() ).toInt() );
    p->setDefaultLengthOfWarrantyInMonth( ui->defaultWarrantyPeriod->value() );
    p->setMovableColumns( ui->movableColumns->isChecked() );
    p->setCascadingSectionResize( ui->cascadingSectionResize->isChecked() );
    p->setDoubleClickResizeColumnToCountent( ui->doubleClickResizeColumnToContent->isChecked() );
    p->setResetCurrentIndexWhenCurrentRowChanged( ui->resetCurrentColumnOnRowChanged->isChecked() );
    p->setAutoCompletionEnabled( ui->autoCompletionEnabled->isChecked() );

    emit pageModified();

    return true;
}


void PreferencesAccountTableConfigPage::revert()
{
    bool block = blockSignals( true );

    Preferences *p = Preferences::self();

    ui->sortPostingsBy->setCurrentIndex( ui->sortPostingsBy->findData( p->sortPostingsBy() ) );
    ui->defaultWarrantyPeriod->setValue( p->defaultLengthOfWarrantyInMonth() );
    ui->movableColumns->setChecked( p->movableColumns() );
    ui->cascadingSectionResize->setChecked( p->cascadingSectionResize() );
    ui->doubleClickResizeColumnToContent->setChecked( p->doubleClickResizeColumnToCountent() );
    ui->resetCurrentColumnOnRowChanged->setChecked( p->resetCurrentIndexWhenCurrentRowChanged() );
    ui->autoCompletionEnabled->setChecked( p->autoCompletionEnabled() );

    blockSignals( block );

    emit pageModified();
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
