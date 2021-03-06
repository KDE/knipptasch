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
#include "passworddialog.h"
#include "ui_passworddialog.h"

#include "compat/iconloader.h"

#include <QDialogButtonBox>
#include <QPushButton>


PasswordDialog::PasswordDialog( const QString &filename, QWidget *parent )
    : QDialog( parent ),
      ui( new Ui::PasswordDialog )
{
    ui->setupUi( this );

    if( !filename.isEmpty() ) {
        ui->label->setText(
            tr( "Enter password to open the file \"%1\"." ).arg( filename )
        );
    }

    setWindowTitle( tr( "Password - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon( "dialog-password" ) );

    connect( ui->password, SIGNAL( textChanged( QString ) ), this, SLOT( checkState() ) );

    checkState();
}

QByteArray PasswordDialog::password() const
{
    return ui->password->text().toUtf8();
}


void PasswordDialog::checkState()
{
    ui->buttonBox->button( QDialogButtonBox::Ok )
    ->setEnabled( !ui->password->text().isEmpty() );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
