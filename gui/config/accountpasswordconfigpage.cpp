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
#include "accountpasswordconfigpage.h"

#include "ui_accountpasswordconfigpage.h"

#include "compat/iconloader.h"

#include <Knipptasch/Account>

#include <QDebug>

#if defined( WITH_QCA2 )
#include <QtCrypto>
#endif


AccountPasswordConfigPage::AccountPasswordConfigPage( Account *account, ConfigWidget *parent )
    : AbstractConfigPage( tr( "Password" ), tr( "Password Options" ), DesktopIcon( "preferences-desktop-cryptography" ), parent ),
      ui( new Ui::AccountPasswordConfigPage ),
      m_account( account )
{
    ui->setupUi( this );

    ui->iconLabel->setPixmap( DesktopIcon( "dialog-password" ) );
    ui->messageIcon->setPixmap( DesktopIcon( "dialog-warning" ) );

    connect( ui->usePassword, SIGNAL( toggled( bool ) ), this, SLOT( onValueChanged() ) );
    connect( ui->pw1, SIGNAL( textChanged( QString ) ), this, SLOT( onValueChanged() ) );
    connect( ui->pw2, SIGNAL( textChanged( QString ) ), this, SLOT( onValueChanged() ) );

#if defined( WITH_QCA2 )
    ui->usePassword->setEnabled( QCA::isSupported( "aes256-cbc-pkcs7" ) );
#else
    ui->usePassword->setEnabled( false );
#endif

    revert();
}


AccountPasswordConfigPage::~AccountPasswordConfigPage()
{
    delete ui;
}


bool AccountPasswordConfigPage::isModified() const
{
    Q_ASSERT( m_account );

    if( m_account->isPasswordEnabled() != ui->usePassword->isChecked() ) {
        return true;
    }

    if( m_account->password() != ui->pw1->text().toUtf8() ) {
        return true;
    }

    return false;
}


bool AccountPasswordConfigPage::isValid() const
{
    Q_ASSERT( m_account );

    if( !ui->usePassword->isChecked() ) {
        return true;
    }

    if( !ui->pw1->text().isEmpty() && ( ui->pw1->text() == ui->pw2->text() ) ) {
        return true;
    }

    return false;
}


bool AccountPasswordConfigPage::commit()
{
    Q_ASSERT( m_account );

    m_account->setPasswordEnabled( ui->usePassword->isChecked() );
    m_account->setPassword( ui->pw1->text().toUtf8() );

    return true;
}


void AccountPasswordConfigPage::revert()
{
    Q_ASSERT( m_account );

    bool block = blockSignals( true );

    ui->pw1->setText( m_account->password() );
    ui->pw2->setText( m_account->password() );
    ui->usePassword->setChecked( m_account->isPasswordEnabled() );

    blockSignals( block );

    onValueChanged();
}


void AccountPasswordConfigPage::onValueChanged()
{
    ui->messageWidget->setVisible( false );

    if( ui->usePassword->isChecked() ) {
        if( ui->pw1->text().isEmpty() ) {
            ui->messageLabel->setText( tr( "The required field Password is empty!" ) );
            ui->messageWidget->setVisible( true );
        } else if( ui->pw2->text().isEmpty() || ui->pw1->text() != ui->pw2->text() ) {
            ui->messageLabel->setText( tr( "The passwords you entered do not match." ) );
            ui->messageWidget->setVisible( true );
        }
    }

    emit pageModified();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
