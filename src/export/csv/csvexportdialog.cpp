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
#include "backend/money.h"

#include "compat/iconloader.h"

#include <QDate>
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QTextCodec>
#include <QPushButton>

#include <KLineEdit>

#if defined(HAVE_KDE)
#include <KFileDialog>
#include <KMessageBox>
#else
#include <QFileDialog>
#include <QMessageBox>
#endif

#include <QApplication>
#include <QBuffer>
#include <QTimer>

#include <QDebug>

#include <unistd.h>



CsvExportDialog::CsvExportDialog(const Account *account, const QList<const Posting*> &selected, QWidget *parent)
  : QDialog( parent ),
    ui( new Ui::CsvExportDialog ),
    m_account( account ),
    m_selectedPostings( selected ),
    m_updateTimer( new QTimer( this ) )
{
    Q_ASSERT( m_account );

    ui->setupUi( this );

    setWindowTitle( tr( "CSV Export - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon("text-csv") );

    m_updateTimer->setInterval( 300 );

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

    const QLocale l;

    ui->decimalSymbol->clear();
    ui->decimalSymbol->addItem( "" );
    ui->decimalSymbol->addItem( "," );
    ui->decimalSymbol->addItem( "." );
    if( ui->decimalSymbol->findText( l.decimalPoint() ) < 0 ) {
        ui->decimalSymbol->addItem( l.decimalPoint() );
    }
    Q_ASSERT( ui->decimalSymbol->findText( l.decimalPoint() ) >= 0 );
    ui->decimalSymbol->setCurrentIndex( ui->decimalSymbol->findText( l.decimalPoint() ) );

    ui->decimalSymbol->setEnabled( false ); //TODO

    ui->thousandsSeparator->clear();
    ui->thousandsSeparator->addItem( "" );
    ui->thousandsSeparator->addItem( "." );
    ui->thousandsSeparator->setCurrentIndex( 0 );
    ui->thousandsSeparator->setEnabled( false ); //TODO

    ui->textquote->clear();
    ui->textquote->addItem( "" );
    ui->textquote->addItem( "\"" );
    ui->textquote->addItem( "'" );
    ui->textquote->addItem( "`" );
    ui->textquote->setCurrentIndex( 1 );

    ui->delimiter->clear();
    ui->delimiter->addItem( "" );
    ui->delimiter->addItem( tr( "Comma" ), "," );
    ui->delimiter->addItem( tr( "Tabulator" ), "\t" );
    ui->delimiter->addItem( tr( "Semicolon" ), ";" );
    ui->delimiter->addItem( tr( "Space" ), " " );
    ui->delimiter->setCurrentIndex( 3 );

    QString df = tr(
        "<p>These expressions may be used for the date format:</p>"
        "<table>"
        "<tr><th>Expression</th><th>Output</th></tr>"
        "<tr><td>d</td><td>The day as a number without a leading zero (1 to 31)</td></tr>"
        "<tr><td>dd</td><td>The day as a number with a leading zero (01 to 31)</td></tr>"
        "<tr><td>ddd</td><td>The abbreviated localized day name (e.g. 'Mon' to 'Sun')</td></tr>"
        "<tr><td>dddd</td><td>The long localized day name (e.g. 'Monday' to 'Sunday')</td></tr>"
        "<tr><td>M</td><td>The month as a number without a leading zero (1 to 12)</td></tr>"
        "<tr><td>MM</td><td>The month as a number with a leading zero (01 to 12)</td></tr>"
        "<tr><td>MMM</td><td>The abbreviated localized month name (e.g. 'Jan' to 'Dec')</td></tr>"
        "<tr><td>MMMM</td><td>The long localized month name (e.g. 'January' to 'December')</td></tr>"
        "<tr><td>yy</td><td>The year as two digit number (00 to 99)</td></tr>"
        "<tr><td>yyyy</td><td>The year as four digit number</td></tr>"
        "</table>"
        "<table>"
        "<tr><th>Expression</th><th>Output</th></tr>"
        "<tr><td>Text Date</td><td>%1</td></tr>"
        "<tr><td>ISO Date</td><td>%2</td></tr>"
        "<tr><td>Short Date</td><td>%3</td></tr>"
        "<tr><td>Long Date</td><td>%4</td></tr>"
    )
        .arg( "ddd MMM d yyyy" )
        .arg( "yyyy-MM-dd" )
        .arg( l.dateFormat( QLocale::ShortFormat ) )
        .arg( l.dateFormat( QLocale::LongFormat ) );

    ui->dateFormat->setToolTip( df );
    ui->dateFormat->setWhatsThis( df );

    ui->dateFormat->clear();
    ui->dateFormat->addItem( "" );
    ui->dateFormat->addItem( tr( "Text Date" ), "ddd MMM d yyyy" );
    ui->dateFormat->addItem( tr( "ISO Date" ), "yyyy-MM-dd" );
    ui->dateFormat->addItem( tr( "Short Date" ), l.dateFormat( QLocale::ShortFormat ) );
    ui->dateFormat->addItem( tr( "Long Date" ), l.dateFormat( QLocale::LongFormat ) );
    ui->dateFormat->setCurrentIndex( 2 );

    ui->encoding->clear();
    foreach(const QByteArray &name, QTextCodec::availableCodecs() ) {
        ui->encoding->addItem( name, name );
    }
    ui->encoding->setCurrentIndex( ui->encoding->findData(
                                    QTextCodec::codecForLocale()->name() ) );

    ui->endOfLine->clear();
    ui->endOfLine->addItem( tr( "Unix" ), "\n" );
    ui->endOfLine->addItem( tr( "Windows/DOS" ), "\r\n" );
    ui->endOfLine->addItem( tr( "Macintosh" ), "\r" );

#if defined( Q_WS_WIN )
    ui->endOfLine->setCurrentIndex( 1 );
#elif defined( Q_WS_MAC )
    ui->endOfLine->setCurrentIndex( 2 );
#else
    ui->endOfLine->setCurrentIndex( 0 );
#endif

    ui->buttonBox->button( QDialogButtonBox::Save )->setEnabled( false );

    connect( ui->delimiter, SIGNAL( activated(int) ),
             this, SLOT( onDelimiterComboBoxIndexChanged(int) ) );
    connect( ui->delimiter->lineEdit(), SIGNAL( textEdited(QString) ),
             this, SLOT( onDelimiterComboBoxTextChanged() ) );

    connect( ui->dateFormat, SIGNAL( activated(int) ),
             this, SLOT( onDateFormatComboBoxIndexChanged(int) ) );
    connect( ui->dateFormat->lineEdit(), SIGNAL( textEdited(QString) ),
             this, SLOT( onDateFormatComboBoxTextChanged() ) );

    connect( ui->all, SIGNAL( toggled(bool) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->selected, SIGNAL( toggled(bool) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->allInYear, SIGNAL( toggled(bool) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->allBetween, SIGNAL( toggled(bool) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->allInYearEdit, SIGNAL( dateChanged(QDate) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->allBetweenDateStart, SIGNAL( dateChanged(QDate) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->allBetweenDateEnd, SIGNAL( dateChanged(QDate) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->textquote, SIGNAL( editTextChanged(QString) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->decimalSymbol, SIGNAL( editTextChanged(QString) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->thousandsSeparator, SIGNAL( editTextChanged(QString) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->encoding, SIGNAL( activated(int) ),
             this, SLOT( onStartUpdateTimer() ) );
    connect( ui->endOfLine, SIGNAL( activated(int) ),
             this, SLOT( onStartUpdateTimer() ) );

    connect( ui->buttonBox->button( QDialogButtonBox::Save ), SIGNAL( clicked() ),
             this, SLOT( onSave() ) );

    connect( m_updateTimer, SIGNAL( timeout() ),
             this, SLOT( onUpdatePreview() ) );

    onDateFormatComboBoxIndexChanged( ui->dateFormat->currentIndex() );
    onDelimiterComboBoxIndexChanged( ui->delimiter->currentIndex() );
}


CsvExportDialog::~CsvExportDialog()
{
    delete ui;
}


void CsvExportDialog::onStartUpdateTimer()
{
    Q_ASSERT( ui->buttonBox->button( QDialogButtonBox::Save ) );

    ui->buttonBox->button( QDialogButtonBox::Save )->setEnabled( false );

    m_updateTimer->start();
}


void CsvExportDialog::onUpdatePreview()
{
    ui->preview->clear();
    ui->rowCountLabel->setText( QString::number( 0 ) );
    ui->columnCountLabel->setText( QString::number( 0 ) );
    ui->characterCountLabel->setText( QString::number( 0 ) );

    m_result.clear();

    QList<const Posting*> postings;

    if( ui->all->isChecked() ) {
        postings = m_account->postings();
    }
    else if( ui->selected->isChecked() ) {
        postings = m_selectedPostings;
    }
    else if( ui->allInYear->isChecked() ) {
        QList<const Posting*> plist = m_account->postings();
        int year = ui->allInYearEdit->date().year();

        foreach(const Posting *p, plist) {
            if( ( p->maturity().isValid() && p->maturity().year() == year ) ||
                ( p->valueDate().isValid() && p->valueDate().year() == year ) ) {
                     postings.append( p );
                }
        }
    }
    else if( ui->allBetween->isChecked() ) {
        QList<const Posting*> plist = m_account->postings();

        QDate start = ui->allBetweenDateStart->date();
        QDate end = ui->allBetweenDateEnd->date() > start
                    ? ui->allBetweenDateEnd->date()
                    : start;

        Q_ASSERT( start.isValid() );
        Q_ASSERT( end.isValid() );

        foreach(const Posting *p, plist) {
            if( ( p->maturity().isValid() && p->maturity() >= start && p->maturity() <= end ) ||
                ( p->valueDate().isValid() && p->valueDate() >= start && p->valueDate() <= end ) ) {
                     postings.append( p );
                }
        }
    }
    else {
        Q_ASSERT( false );
    }

    QString eof = ui->endOfLine->itemData( ui->endOfLine->currentIndex() ).toString();
    QString tq = ui->textquote->currentText();

    QBuffer buffer( &m_result );
    buffer.open( QIODevice::WriteOnly );

    QTextStream out( &buffer );
    out.setCodec( QTextCodec::codecForName(
                    ui->encoding->itemData( ui->encoding->currentIndex() )
                        .toByteArray()
                ) );

    for(int i = 0; i < postings.size(); ++i) {
        const Posting *p = postings[ i ];

        out << tq << p->valueDate().toString( m_dateFormat ) << tq << m_delimiter;
        out << tq << p->postingText()                        << tq << m_delimiter;
        out << tq << p->maturity().toString( m_dateFormat )  << tq << m_delimiter;
        out << tq << p->amount().toString()                  << tq << m_delimiter;
        out << eof;
    }

    buffer.close();

    ui->rowCountLabel->setText( QString::number( postings.size() ) );
    ui->columnCountLabel->setText( QString::number( postings.size() * 4 ) );
    ui->characterCountLabel->setText( QString::number( m_result.size() ) );

    ui->preview->setPlainText( m_result );

    Q_ASSERT( ui->buttonBox->button( QDialogButtonBox::Save ) );
    ui->buttonBox->button( QDialogButtonBox::Save )
                        ->setEnabled( !m_result.isEmpty() );
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

    QFile file( filename );
    if( !file.open( QIODevice::WriteOnly ) ) {
        QString message = tr( "The file given could not be written; "
                              "check whether it exists or is writeable "
                              "for the current user." );
#if defined(HAVE_KDE)
        KMessageBox::error( parentWidget(), message );
#else
        QMessageBox::warning( parentWidget(),        // krazy:exclude=qclasses
                              tr( "Error - %1" )
                               .arg( QCoreApplication::applicationName() ),
                              message );
#endif
        reject();
        return;
    }

    file.write( m_result );
    file.close();

    accept();
}


void CsvExportDialog::onDelimiterComboBoxIndexChanged(int index)
{
    m_delimiter = ui->delimiter->itemData( index ).toString();
    onStartUpdateTimer();
}


void CsvExportDialog::onDelimiterComboBoxTextChanged()
{
    m_delimiter = ui->delimiter->currentText();
    onStartUpdateTimer();
}


void CsvExportDialog::onDateFormatComboBoxIndexChanged(int index)
{
    m_dateFormat = ui->dateFormat->itemData( index ).toString();
    onStartUpdateTimer();
}


void CsvExportDialog::onDateFormatComboBoxTextChanged()
{
    m_dateFormat = ui->dateFormat->currentText();
    onStartUpdateTimer();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
