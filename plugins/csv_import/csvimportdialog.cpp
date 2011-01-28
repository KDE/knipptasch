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
#include "csvimportdialog.h"
#include "ui_csvimportdialog.h"

#include "backend/account.h"

#include "compat/iconloader.h"

#include <QDate>
#include <QCoreApplication>
#include <QTextCodec>

#include <KLineEdit>

#include <QDebug>


CsvImportDialog::CsvImportDialog(QWidget *parent)
  : QDialog( parent ),
    ui( new Ui::CsvImportDialog )
{
    ui->setupUi( this );

    setWindowTitle( tr( "CSV Import - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon("text-csv") );

    ui->fileButton->setIcon( BarIcon("document-open") );

    ui->delimiter->clear();
    ui->delimiter->addItem( "" );
    ui->delimiter->addItem( tr( "Comma" ), ',' );
    ui->delimiter->addItem( tr( "Tabulator" ), '\t' );
    ui->delimiter->addItem( tr( "Semicolon" ), ';' );
    ui->delimiter->addItem( tr( "Space" ), ' ' );
    ui->delimiter->setCurrentIndex( 3 );
    m_delimiter = ui->delimiter->itemData( 3 ).toChar();

    ui->encoding->clear();
    foreach(const QByteArray &name, QTextCodec::availableCodecs() ) {
        ui->encoding->addItem( name, name );
    }
    ui->encoding->setCurrentIndex( ui->encoding->findData( QTextCodec::codecForLocale()->name() ) );

    ui->endOfLine->clear();
    ui->endOfLine->addItem( tr( "Unix" ) );
    ui->endOfLine->addItem( tr( "Windows/DOS" ) );
    ui->endOfLine->addItem( tr( "Macintosh" ) );

#if defined( Q_WS_WIN )
    ui->endOfLine->setCurrentIndex( 1 );
#elif defined( Q_WS_MAC )
    ui->endOfLine->setCurrentIndex( 2 );
#else
    ui->endOfLine->setCurrentIndex( 0 );
#endif

    connect( ui->delimiter, SIGNAL( activated(int) ), this, SLOT( onDelimiterComboBoxIndexChanged(int) ) );
    connect( ui->delimiter->lineEdit(), SIGNAL( textEdited(QString) ), this, SLOT( onDelimiterComboBoxTextChanged() ) );
}


CsvImportDialog::~CsvImportDialog()
{
    delete ui;
}


Account* CsvImportDialog::account() const
{
    return 0;
}


void CsvImportDialog::onDelimiterComboBoxIndexChanged(int index)
{
    m_delimiter = ui->delimiter->itemData( index ).toChar();
}


void CsvImportDialog::onDelimiterComboBoxTextChanged()
{
    m_delimiter = ui->delimiter->currentText();
}




// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
