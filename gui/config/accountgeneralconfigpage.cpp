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
#include "accountgeneralconfigpage.h"

#include "ui_accountgeneralconfigpage.h"
#include "compat/iconloader.h"

#include <Knipptasch/Account>
#include <Knipptasch/Money>


AccountGeneralConfigPage::AccountGeneralConfigPage(Account *account, ConfigWidget* parent)
  : AbstractConfigPage( tr( "General" ), tr( "General Options" ), DesktopIcon("view-bank-account"), parent ),
    ui( new Ui::AccountGeneralConfigPage ),
    m_account( account )
{
    ui->setupUi( this );

    //TODO implement the account and category limit stuff
    ui->accountLimitGroupBox->setVisible( false );

    connect( ui->name, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->institution, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->bic, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->owner, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->accountNumber, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->iban, SIGNAL( textChanged(QString) ), this, SIGNAL( pageModified() ) );
    connect( ui->openingDate, SIGNAL( dateChanged(QDate) ), this, SIGNAL( pageModified() ) );
    connect( ui->openingBalance, SIGNAL( valueChanged(double) ), this, SIGNAL( pageModified() ) );
    connect( ui->useInLimit, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->inLimit, SIGNAL( valueChanged(double) ), this, SIGNAL( pageModified() ) );
    connect( ui->useOutLimit, SIGNAL( stateChanged(int) ), this, SIGNAL( pageModified() ) );
    connect( ui->outLimit, SIGNAL( valueChanged(double) ), this, SIGNAL( pageModified() ) );

    revert();
}


AccountGeneralConfigPage::~AccountGeneralConfigPage()
{
    delete ui;
}


bool AccountGeneralConfigPage::isModified() const
{
    Q_ASSERT( m_account );

    if( ui->name->text().trimmed() != m_account->name().trimmed() ) {
        return true;
    }

    if( ui->institution->text().trimmed() != m_account->institution().trimmed() ) {
        return true;
    }

    if( ui->bic->text().trimmed() != m_account->bic().trimmed() ) {
        return true;
    }

    if( ui->owner->text().trimmed() != m_account->owner().trimmed() ) {
        return true;
    }

    if( ui->accountNumber->text().trimmed() != m_account->number().trimmed() ) {
        return true;
    }

    if( ui->iban->text().trimmed() != m_account->iban().trimmed() ) {
        return true;
    }

    if( ui->openingDate->date() != m_account->openingDate() ) {
        return true;
    }

    if( ui->openingBalance->value() != m_account->openingBalance() ) {
        return true;
    }

    if( ui->useInLimit->isChecked() != m_account->maximumBalanceEnabled() ) {
        return true;
    }

    if( ui->inLimit->value() != m_account->maximumBalance() ) {
        return true;
    }

    if( ui->useOutLimit->isChecked() != m_account->minimumBalanceEnabled() ) {
        return true;
    }

    if( ui->outLimit->value() != m_account->minimumBalance() ) {
        return true;
    }

    return false;
}


bool AccountGeneralConfigPage::commit()
{
    Q_ASSERT( m_account );

    m_account->setName( ui->name->text().trimmed() );
    m_account->setInstitution( ui->institution->text().trimmed() );
    m_account->setBic( ui->bic->text().trimmed() );
    m_account->setOwner( ui->owner->text().trimmed() );
    m_account->setNumber( ui->accountNumber->text().trimmed() );
    m_account->setIban( ui->iban->text().trimmed() );
    m_account->setOpeningDate( ui->openingDate->date() );
    m_account->setOpeningBalance( ui->openingBalance->value() );
    m_account->setMaximumBalanceEnabled( ui->useInLimit->isChecked() );
    m_account->setMaximumBalance( ui->inLimit->value() );
    m_account->setMinimumBalanceEnabled( ui->useOutLimit->isChecked() );
    m_account->setMinimumBalance( ui->outLimit->value() );

    return true;
}


void AccountGeneralConfigPage::revert()
{
    Q_ASSERT( m_account );

    bool b = blockSignals( true );

    ui->name->setText( m_account->name()  );
    ui->institution->setText( m_account->institution() );
    ui->bic->setText( m_account->bic() );
    ui->owner->setText( m_account->owner() );
    ui->accountNumber->setText( m_account->number() );
    ui->iban->setText( m_account->iban() );
    ui->openingDate->setDate( m_account->openingDate() );
    ui->openingBalance->setValue( m_account->openingBalance() );
    ui->useInLimit->setChecked( m_account->maximumBalanceEnabled() );
    ui->inLimit->setValue( m_account->maximumBalance() );
    ui->inLimit->setEnabled( m_account->maximumBalanceEnabled() );
    ui->useOutLimit->setChecked( m_account->minimumBalanceEnabled() );
    ui->outLimit->setValue( m_account->minimumBalance() );
    ui->outLimit->setEnabled( m_account->minimumBalanceEnabled() );

    blockSignals( b );

    emit pageModified();
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
