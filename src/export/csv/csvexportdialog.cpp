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
#include "backend/posting.h"

#include "compat/iconloader.h"

#include <QDate>
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QTextCodec>
#include <QPushButton>

#include <KLineEdit>

#if defined(HAVE_KDE)
#include <KFileDialog>
#else
#include <QFileDialog>
#endif

#include <QDebug>
#include <qprogressdialog.h>
#include <unistd.h>
#include <QApplication>
#include <backend/money.h>
#include <QBuffer>


CsvExportDialog::CsvExportDialog(const Account *account, const QList<const Posting*> &selected, QWidget *parent)
  : QDialog( parent ),
    ui( new Ui::CsvExportDialog ),
    m_account( account ),
    m_selectedPostings( selected )
{
    Q_ASSERT( m_account );

    ui->setupUi( this );

    setWindowTitle( tr( "CSV Export - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon("text-csv") );

    ui->allInYearEdit->setDate( QDate::currentDate() );
    ui->allBetweenDateStart->setDate( account->openingDate().isValid()
                                        ? account->openingDate()
                                        : QDate( 1, 1, QDate::currentDate().year() )
                                    );
    ui->allBetweenDateEnd->setDate( QDate::currentDate() );

    if( !m_selectedPostings.isEmpty() ) {
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
    m_delimiter = ui->delimiter->itemData( 3 ).toChar();

    ui->encoding->clear();
    foreach(const QByteArray &name, QTextCodec::availableCodecs() ) {
        ui->encoding->addItem( name, name );
    }
    ui->encoding->setCurrentIndex( ui->encoding->findData( QTextCodec::codecForLocale()->name() ) );

    ui->endOfLine->clear();
    ui->endOfLine->addItem( tr( "Unix" ), "\n" );
    ui->endOfLine->addItem( tr( "Windows/DOS" ), "\r\n" );
    ui->endOfLine->addItem( tr( "Macintosh" ), "\r" );
    ui->endOfLine->setCurrentIndex( 0 );

    connect( ui->delimiter, SIGNAL( activated(int) ), this, SLOT( onDelimiterComboBoxIndexChanged(int) ) );
    connect( ui->delimiter->lineEdit(), SIGNAL( textEdited(QString) ), this, SLOT( onDelimiterComboBoxTextChanged() ) );

    connect( ui->buttonBox->button( QDialogButtonBox::Save ), SIGNAL( clicked() ), this, SLOT( onSave() ) );
}


CsvExportDialog::~CsvExportDialog()
{
    delete ui;
}


void CsvExportDialog::onSave()
{
    hide();

    QString filename;
#if defined(HAVE_KDE)
    filename = KFileDialog::getSaveFileName( KUrl(), "*.csv|" + tr( "All Supported Files" ), this );
#else
    filename = QFileDialog::getSaveFileName( this, // krazy:exclude=qclasses
                     tr( "Export File - %1" ).arg( QCoreApplication::applicationName() ),
                     QString(), tr( "All Supported Files" ) + " (*.csv)"
                   );
#endif

    if( filename.isEmpty() ) {
        rejected();
        return;
    }

    QList<const Posting*> postings = m_account->postings();

    QProgressDialog progress( tr( "Exporting data..." ), tr( "Cancel" ), 0, postings.size(), this );
    progress.setWindowModality( Qt::WindowModal );

    QString eof = ui->endOfLine->currentText();
    QString tq = ui->textquote->currentText();

    QByteArray output;
    QBuffer buffer( &output );
    buffer.open( QIODevice::WriteOnly );

    QTextStream out( &buffer );
    out.setCodec( QTextCodec::codecForName( ui->encoding->itemData( ui->encoding->currentIndex() ).toByteArray() ) );

    for(int i = 0; i < postings.size(); ++i) {
        progress.setValue( i );

        if( progress.wasCanceled() ) {
            break;
        }

        const Posting *p = postings[ i ];

        out << tq << p->valueDate().toString().toUtf8() << tq << m_delimiter;
        out << tq << p->postingText()                   << tq << m_delimiter;
        out << tq << p->maturity().toString()           << tq << m_delimiter;
        out << tq << p->amount().toString()             << tq << m_delimiter;
        out << eof;
    }

    buffer.close();
    progress.setValue( postings.size() );

    qDebug() << output;

    show();
}


void CsvExportDialog::onDelimiterComboBoxIndexChanged(int index)
{
    m_delimiter = ui->delimiter->itemData( index ).toChar();
}


void CsvExportDialog::onDelimiterComboBoxTextChanged()
{
    m_delimiter = ui->delimiter->currentText();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
