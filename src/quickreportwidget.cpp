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
#include "quickreportwidget.h"
#include "ui_quickreportwidget.h"

#include "compat/utils.h"

#if defined(HAVE_KDE)
#include <KGlobal>
#include <KLocale>
#include <KCalendarSystem>
#endif

#include <QLocale>
#include <QShowEvent>



QuickReportWidget::QuickReportWidget(QWidget *parent)
  : QWidget( parent ),
    ui( new Ui::QuickReportWidget ),
    m_date( QDate::currentDate() )
{
    ui->setupUi( this );

    installEventFilter( this );

    connect( ui->nextButton,      SIGNAL( clicked(bool) ), this, SLOT( slotNextMonth()     ) );
    connect( ui->previousButton,  SIGNAL( clicked(bool) ), this, SLOT( slotPreviousMonth() ) );
}


QuickReportWidget::~QuickReportWidget()
{
}


QSize QuickReportWidget::sizeHint() const
{
    const int width = fontMetrics().width( formatMoney( 100000.0 ) );

    return QSize(
            QWidget::sizeHint().width()
                - ui->incomeValue->sizeHint().width()
                + width,
            QWidget::sizeHint().height()
    );
}


void QuickReportWidget::showEvent(QShowEvent *event)
{
    if( event->type() == QEvent::Show ) {
        updateView();
    }
}


bool QuickReportWidget::eventFilter(QObject *obj, QEvent *event)
{
    if( event->type() == QEvent::Wheel ) {
        QWheelEvent *we = static_cast<QWheelEvent*>( event );

        if( we->delta() > 0 ) {
            slotNextMonth();

            return true;
        }
        else if( we->delta() < 0 ) {
            slotPreviousMonth();

            return true;
        }
    }
    else if( event->type() == QEvent::MouseButtonDblClick ) {
        m_date = QDate::currentDate();
        updateView();

        return true;
    }

    return QWidget::eventFilter( obj, event );
}


void QuickReportWidget::slotNextMonth()
{
    Q_ASSERT( m_date.isValid() );

    if( ui->nextButton->isEnabled() ) {
        m_date = m_date.addMonths( 1 );
        updateView();
    }
}


void QuickReportWidget::slotPreviousMonth()
{
    Q_ASSERT( m_date.isValid() );

    if( ui->previousButton->isEnabled() ) {
        m_date = m_date.addMonths( -1 );
        updateView();
    }
}


void QuickReportWidget::updateView()
{
    ui->titleLabel->setText( formatMonth( m_date ) );

    if( false /*m_account*/ ) {
        setEnabled( true );

    }
    else {
        setEnabled( false );

        ui->incomeValue->setText( tr( "Unknown" ) );
        ui->expenseValue->setText( tr( "Unknown" ) );
        ui->diffValue->setText( tr( "Unknown" ) );
    }
}



QString QuickReportWidget::formatMonth(const QDate &date)
{
    Q_ASSERT( date.isValid() );

    const QDate &current = QDate(
                                QDate::currentDate().year(),
                                QDate::currentDate().month(),
                                date.day()
                            );

    if( date == current ) {
        return tr( "current month" );
    }

    if( date == current.addMonths( -1 ) ) {
        return tr( "last month" );
    }

    if( date == current.addMonths( 1 ) ) {
        return tr( "next month" );
    }

    if( date == current.addYears( 1 ) ) {
        return tr( "next year" );
    }

    if( date == current.addYears( -1 ) ) {
        return tr( "last year" );
    }

    return tr( "monthname year", "%1 %2" )
#if defined(HAVE_KDE)
                .arg( KGlobal::locale()->calendar()->monthName( date ) )
                .arg( KGlobal::locale()->calendar()->yearString( date ) );
#else
                .arg( QLocale().monthName( date.month() ) )
                .arg( QString::number( date.year() ) );
#endif
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
