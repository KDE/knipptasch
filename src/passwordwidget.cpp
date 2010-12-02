/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
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
#include "passwordwidget.h"
#include "ui_passwordwidget.h"

#include "compat/iconloader.h"
#include "backend/account.h"


PasswordWidget::PasswordWidget(QWidget *parent)
  : QWidget( parent ),
    ui( new Ui::PasswordWidget ),
    m_account( 0 )
{
    ui->setupUi( this );

    ui->iconLabel->setPixmap( DesktopIcon("dialog-password") );
    ui->messageIcon->setPixmap( DesktopIcon("dialog-warning") );

    ui->level->clear();
    ui->level->addItem( tr( "High" ), static_cast<int>( Account::High ) );
    ui->level->addItem( tr( "Average" ), static_cast<int>( Account::Average ) );
    ui->level->addItem( tr( "Low" ), static_cast<int>( Account::Low ) );
    ui->level->setCurrentIndex( 2 );

    connect( ui->usePassword, SIGNAL( toggled(bool) ), this, SLOT( onValueChanged() ) );
    connect( ui->level, SIGNAL( currentIndexChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->pw1, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->pw2, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
}


PasswordWidget::~PasswordWidget()
{
}


bool PasswordWidget::isValid() const
{
    if( !m_account ) {
        return true;
    }

    if( !ui->usePassword->isChecked() ) {
        return true;
    }

    if( !ui->pw1->text().isEmpty() && ( ui->pw1->text() == ui->pw2->text() ) ) {
        return true;
    }

    return false;
}


bool PasswordWidget::isModified() const
{
    if( !m_account ) {
        return false;
    }

    if( m_account->isPasswordEnabled() != ui->usePassword->isChecked() ) {
        return true;
    }

    if( m_account->password() != ui->pw1->text().toUtf8() ) {
        return true;
    }

    return false;
}


void PasswordWidget::onApplyChanges()
{
    m_account->setPasswordEnabled( ui->usePassword->isChecked() );
    m_account->setPassword( ui->pw1->text().toUtf8() );
    m_account->setSecurityLevel(
        static_cast<Account::SecurityLevel>( ui->level->itemData( ui->level->currentIndex() ).toInt() )
    );
}


void PasswordWidget::setAccount(Account *account)
{
    m_account = account;

    bool block = blockSignals( true );

    ui->level->setCurrentIndex( ui->level->findData( account->securityLevel() ) );
    ui->pw1->setText( account->password() );
    ui->pw2->setText( account->password() );
    ui->usePassword->setChecked( account->isPasswordEnabled() );

    blockSignals( block );

    emit valueChanged();
}


void PasswordWidget::onValueChanged()
{
    ui->messageWidget->setVisible( false );

    if( ui->usePassword->isChecked() ) {
        if( ui->pw1->text().isEmpty() ) {
            ui->messageLabel->setText( tr( "The required field Password is empty!" ) );
            ui->messageWidget->setVisible( true );
        }
        else if( ui->pw2->text().isEmpty() || ui->pw1->text() != ui->pw2->text() ) {
            ui->messageLabel->setText( tr( "The passwords you entered do not match." ) );
            ui->messageWidget->setVisible( true );
        }
    }

    emit valueChanged();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
