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

#include <Knipptasch/Preferences>

#include <QDebug>
#include <compat/utils.h>


PreferencesAccountTableConfigPage::PreferencesAccountTableConfigPage( Knipptasch::Preferences *pref, ConfigWidget *parent )
    : AbstractConfigPage( tr( "Account Table" ), tr( "Account Table Settings" ), DesktopIcon( "view-form-table" ), parent ),
      ui( new Ui::PreferencesAccountTableConfigPage ),
      m_preferences( pref )
{
    ui->setupUi( this );

    ui->sortPostingsBy->clear();
    ui->sortPostingsBy->addItem( tr( "Maturity" ), Knipptasch::Preferences::Maturity );
    ui->sortPostingsBy->addItem( tr( "Value Date" ), Knipptasch::Preferences::ValueDate );

    connect( ui->sortPostingsBy, SIGNAL( currentIndexChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->defaultWarrantyPeriod, SIGNAL( valueChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->movableColumns, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->cascadingSectionResize, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->doubleClickResizeColumnToContent, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->resetCurrentColumnOnRowChanged, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    connect( ui->autoCompletionEnabled, SIGNAL( stateChanged( int ) ),
             this, SIGNAL( pageModified() ) );

    revert();
}


PreferencesAccountTableConfigPage::~PreferencesAccountTableConfigPage()
{
    delete ui;
}


bool PreferencesAccountTableConfigPage::isModified() const
{
    int sortPostingBy = ui->sortPostingsBy->itemData( ui->sortPostingsBy->currentIndex() ).toInt();
    if( sortPostingBy != m_preferences->postingSortOrder() ) {
        return true;
    }

    if( ui->defaultWarrantyPeriod->value() != m_preferences->defaultLengthOfWarrantyInMonth() ) {
        return true;
    }

    if( ui->movableColumns->isChecked() != m_preferences->movableColumns() ) {
        return true;
    }

    if( ui->cascadingSectionResize->isChecked() != m_preferences->cascadingSectionResize() ) {
        return true;
    }

    if( ui->doubleClickResizeColumnToContent->isChecked() != m_preferences->doubleClickResizeColumnToCountent() ) {
        return true;
    }

    if( ui->resetCurrentColumnOnRowChanged->isChecked() != m_preferences->resetCurrentIndexWhenCurrentRowChanged() ) {
        return true;
    }

    if( ui->autoCompletionEnabled->isChecked() != m_preferences->autoCompletionEnabled() ) {
        return true;
    }

    return false;
}


bool PreferencesAccountTableConfigPage::commit()
{
    m_preferences->setPostingSortOrder( static_cast<Knipptasch::Preferences::PostingSortOrder>(
                                            ui->sortPostingsBy->itemData( ui->sortPostingsBy->currentIndex() ).toInt()
                                        )
                                      );
    m_preferences->setDefaultLengthOfWarrantyInMonth( ui->defaultWarrantyPeriod->value() );
    m_preferences->setMovableColumns( ui->movableColumns->isChecked() );
    m_preferences->setCascadingSectionResize( ui->cascadingSectionResize->isChecked() );
    m_preferences->setDoubleClickResizeColumnToCountent( ui->doubleClickResizeColumnToContent->isChecked() );
    m_preferences->setResetCurrentIndexWhenCurrentRowChanged( ui->resetCurrentColumnOnRowChanged->isChecked() );
    m_preferences->setAutoCompletionEnabled( ui->autoCompletionEnabled->isChecked() );

    emit pageModified();

    return true;
}


void PreferencesAccountTableConfigPage::revert()
{
    bool block = blockSignals( true );

    ui->sortPostingsBy->setCurrentIndex( ui->sortPostingsBy->findData( m_preferences->postingSortOrder() ) );
    ui->defaultWarrantyPeriod->setValue( m_preferences->defaultLengthOfWarrantyInMonth() );
    ui->movableColumns->setChecked( m_preferences->movableColumns() );
    ui->cascadingSectionResize->setChecked( m_preferences->cascadingSectionResize() );
    ui->doubleClickResizeColumnToContent->setChecked( m_preferences->doubleClickResizeColumnToCountent() );
    ui->resetCurrentColumnOnRowChanged->setChecked( m_preferences->resetCurrentIndexWhenCurrentRowChanged() );
    ui->autoCompletionEnabled->setChecked( m_preferences->autoCompletionEnabled() );

    blockSignals( block );

    emit pageModified();
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
