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
#include "accountsettingsdialog.h"
#include "ui_accountsettingsdialog.h"

#include "compat/iconloader.h"

#include "backend/account.h"
#include "backend/money.h"

#include <QPushButton>



AccountSettingsDialog::AccountSettingsDialog(Account *account, QWidget* parent)
  : QDialog( parent ),
    ui( new Ui::AccountSettingsDialog ),
    m_account( 0 )
{
    ui->setupUi( this );

    setWindowTitle( tr( "Configure Account - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon("view-bank-account") );

    setAccount( account );

    connect( ui->name, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->institution, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->bic, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->owner, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->accountNumber, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->iban, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->openingDate, SIGNAL( dateChanged(QDate) ), this, SLOT( onValueChanged() ) );
    connect( ui->openingBalance, SIGNAL( valueChanged(double) ), this, SLOT( onValueChanged() ) );
    connect( ui->useInLimit, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->inLimit, SIGNAL( valueChanged(double) ), this, SLOT( onValueChanged() ) );
    connect( ui->useOutLimit, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->outLimit, SIGNAL( valueChanged(double) ), this, SLOT( onValueChanged() ) );

    connect( ui->buttonBox, SIGNAL( accepted() ), this, SLOT( onApplyChanges() ) );
    connect( ui->buttonBox->button( QDialogButtonBox::Apply ), SIGNAL( clicked(bool) ), this, SLOT( onApplyChanges() ) );

    onValueChanged();
}


AccountSettingsDialog::~AccountSettingsDialog()
{
    delete ui;
}


Account* AccountSettingsDialog::account()
{
    return m_account;
}


const Account* AccountSettingsDialog::account() const
{
    return m_account;
}


void AccountSettingsDialog::setAccount(Account *account)
{
    m_account = account;

    ui->name->setEnabled( m_account );
    ui->institution->setEnabled( m_account );
    ui->bic->setEnabled( m_account );
    ui->owner->setEnabled( m_account );
    ui->accountNumber->setEnabled( m_account );
    ui->iban->setEnabled( m_account );
    ui->openingDate->setEnabled( m_account );
    ui->openingBalance->setEnabled( m_account );
    ui->useInLimit->setEnabled( m_account );
    ui->inLimit->setEnabled( m_account );
    ui->useOutLimit->setEnabled( m_account );
    ui->outLimit->setEnabled( m_account );

    if( m_account ) {
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
    }
}


void AccountSettingsDialog::onValueChanged()
{
    Q_ASSERT( m_account );

    ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( true );

    do {
        if( ui->name->text().trimmed() != m_account->name().trimmed() ) {
            break;
        }

        if( ui->institution->text().trimmed() != m_account->institution().trimmed() ) {
            break;
        }

        if( ui->bic->text().trimmed() != m_account->bic().trimmed() ) {
            break;
        }

        if( ui->owner->text().trimmed() != m_account->owner().trimmed() ) {
            break;
        }

        if( ui->accountNumber->text().trimmed() != m_account->number().trimmed() ) {
            break;
        }

        if( ui->iban->text().trimmed() != m_account->iban().trimmed() ) {
            break;
        }

        if( ui->openingDate->date() != m_account->openingDate() ) {
            break;
        }

        if( ui->openingBalance->value() != m_account->openingBalance() ) {
            break;
        }

        if( ui->useInLimit->isChecked() != m_account->maximumBalanceEnabled() ) {
            break;
        }

        if( ui->inLimit->value() != m_account->maximumBalance() ) {
            break;
        }

        if( ui->useOutLimit->isChecked() != m_account->minimumBalanceEnabled() ) {
            break;
        }

        if( ui->outLimit->value() != m_account->minimumBalance() ) {
            break;
        }

        ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    } while( false );
}


void AccountSettingsDialog::onApplyChanges()
{
    if( m_account ) {
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
    }
    onValueChanged();
}

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
