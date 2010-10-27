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
#include "csvexportdialog.h"
#include "ui_csvexportdialog.h"

#include "backend/account.h"

#include "compat/iconloader.h"

#include <QDate>
#include <QCoreApplication>
#include <QTextCodec>



CsvExportDialog::CsvExportDialog(const Account *account, const QList<const Account*> &selected, QWidget *parent)
  : QDialog( parent ),
    ui( new Ui::CsvExportDialog )
{
    ui->setupUi( this );

    setWindowTitle( tr( "CSV Export - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon("text-csv") );

    ui->allInYearEdit->setDate( QDate::currentDate() );
    ui->allBetweenDateStart->setDate( account->openingDate().isValid()
                                        ? account->openingDate()
                                        : QDate( 1, 1, QDate::currentDate().year() )
                                    );
    ui->allBetweenDateEnd->setDate( QDate::currentDate() );

    if( !selected.isEmpty() ) {
        ui->selected->setEnabled( true );
        ui->selected->setChecked( true );
    }
    else {
        ui->selected->setEnabled( false );
        ui->all->setChecked( true );
    }
    
    ui->delimiter->clear();
    ui->delimiter->addItem( "" );
    ui->delimiter->addItem( tr( "Comma" ), ',' );
    ui->delimiter->addItem( tr( "Tabulator" ), '\t' );
    ui->delimiter->addItem( tr( "Semicolon" ), ';' );
    ui->delimiter->addItem( tr( "Space" ), ' ' );
    ui->delimiter->setCurrentIndex( 3 );

    ui->textquote->clear();
    ui->textquote->addItem( "\"" );
    ui->textquote->addItem( "'" );
    ui->textquote->addItem( "`" );
    ui->textquote->setCurrentIndex( 0 );

    ui->dateFormat->clear();
    ui->dateFormat->addItem( "Y-M-D" );
    ui->dateFormat->setCurrentIndex( 0 );
    
    ui->decimalSymbol->clear();
    ui->decimalSymbol->addItem( "." );
    ui->decimalSymbol->addItem( "," );

    ui->thousandsSeparator->clear();
    ui->thousandsSeparator->addItem( "" );
    ui->thousandsSeparator->addItem( "." );

    ui->currencySign->clear(); 
    ui->currencySign->addItem( "" );

    ui->encoding->clear();
    foreach(const QByteArray &name, QTextCodec::availableCodecs() ) {
        ui->encoding->addItem( name, name );
    }
    ui->encoding->setCurrentIndex( ui->encoding->findData( QTextCodec::codecForLocale()->name() ) );

    ui->endOfLine->clear();
    ui->endOfLine->addItem( tr( "Unix" ) );
    ui->endOfLine->addItem( tr( "Windows/DOS" ) );
    ui->endOfLine->addItem( tr( "Macintosh" ) );
    ui->endOfLine->setCurrentIndex( 0 );
}


CsvExportDialog::~CsvExportDialog()
{
    delete ui;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
