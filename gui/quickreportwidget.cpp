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

#include "backend/account.h"
#include "backend/money.h"
#include "backend/posting.h"

#include "accountsortfilterproxymodel.h"

#include "preferences.h"

#include "compat/utils.h"
#include "compat/iconloader.h"
#if defined(HAVE_KDE)
#include <KGlobal>
#include <KLocale>
#include <KCalendarSystem>
#endif

#include <QLocale>
#include <QShowEvent>


bool doSortPostings( const Posting *p1, const Posting *p2 )
{
    if( !p1->maturity().isValid() ) {
        return true;
    }
    if( !p2->maturity().isValid() ) {
        return false;
    }

    return p1->maturity() < p2->maturity();
}



QuickReportWidget::QuickReportWidget( AccountSortFilterProxyModel *proxy, QWidget *parent )
    : QWidget( parent ),
      ui( new Ui::QuickReportWidget ),
      m_proxy( proxy ),
      m_date( QDate::currentDate().year(), QDate::currentDate().month(), 1 )
{
    Q_ASSERT( m_proxy );

    ui->setupUi( this );

    ui->closeButton->setVisible( false );

    ui->closeButton->setIcon( BarIcon( "dialog-close" ) );
    ui->nextButton->setIcon( BarIcon( "arrow-right" ) );
    ui->previousButton->setIcon( BarIcon( "arrow-left" ) );

    installEventFilter( this );

    connect( ui->nextButton, SIGNAL( clicked( bool ) ), this, SLOT( slotNextMonth() ) );
    connect( ui->previousButton, SIGNAL( clicked( bool ) ), this, SLOT( slotPreviousMonth() ) );
    connect( ui->closeButton, SIGNAL( clicked( bool ) ), this, SIGNAL( closeRequested() ) );
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


void QuickReportWidget::showEvent( QShowEvent *event )
{
    if( event->type() == QEvent::Show ) {
        updateView();
    }
}


bool QuickReportWidget::eventFilter( QObject *obj, QEvent *event )
{
    if( event->type() == QEvent::Wheel ) {
        QWheelEvent *we = static_cast<QWheelEvent *>( event );

        if( we->delta() > 0 ) {
            slotNextMonth();

            return true;
        } else if( we->delta() < 0 ) {
            slotPreviousMonth();

            return true;
        }
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
    Q_ASSERT( m_proxy );
    AccountModel *model = qobject_cast<AccountModel *>( m_proxy->sourceModel() );
    Q_ASSERT( model );

    if( !model->account() ) {
        return;
    }

    ui->titleLabel->setText( formatMonth( m_date ) );

    Money income;
    Money expense;

    QList<const Posting *> list = model->account()->postings();
    qSort( list.begin(), list.end(), doSortPostings );

    foreach( const Posting * p, list ) {
        if( p->maturity().year() == m_date.year() ) {
            if( p->maturity().month() == m_date.month() ) {
                if( p->amount() < 0.0 ) {
                    expense -= p->amount().abs();
                } else {
                    income += p->amount();
                }
            } else if( p->maturity().month() > m_date.month() ) {
                break;
            }
        } else if( p->maturity().year() > m_date.year() ) {
            break;
        }
    }

    ui->incomeValue->setText( formatMoney( income ) );
    ui->expenseValue->setText( formatMoney( expense ) );
    ui->diffValue->setText( formatMoney( income - expense.abs() ) );

    if( Knipptasch::Preferences::self()->positiveAmountForegroundEnabled()
            && Knipptasch::Preferences::self()->positiveAmountForegroundColor().isValid() ) {
        QString style = QString::fromLatin1( "color: %1;" ).arg(
                            Knipptasch::Preferences::self()->positiveAmountForegroundColor().name() );

        ui->incomeValue->setStyleSheet( style );

        if(( income - expense.abs() ) >= 0.0 ) {
            ui->diffValue->setStyleSheet( style );
        }
    }

    if( Knipptasch::Preferences::self()->negativeAmountForegroundEnabled()
            && Knipptasch::Preferences::self()->negativeAmountForegroundColor().isValid() ) {
        QString style = QString::fromLatin1( "color: %1;" ).arg(
                            Knipptasch::Preferences::self()->negativeAmountForegroundColor().name() );

        ui->expenseValue->setStyleSheet( style );

        if(( income - expense.abs() ) < 0.0 ) {
            ui->diffValue->setStyleSheet( style );
        }
    }

    ui->nextButton->setEnabled( m_date < QDate::currentDate().addMonths( 3 ) );
    ui->previousButton->setEnabled( m_date > model->account()->openingDate() );
}


QDate QuickReportWidget::currentDate() const
{
    return m_date;
}


void QuickReportWidget::setCurrentDate( const QDate &date )
{
    m_date = QDate( date.year(), date.month(), 1 );
    updateView();
}


bool QuickReportWidget::closeButtonEnabled() const
{
    return ui->closeButton->isVisible();
}


void QuickReportWidget::setCloseButtonEnabled( bool state )
{
    ui->closeButton->setVisible( state );
}


QString QuickReportWidget::formatMonth( const QDate &date )
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

    return tr( "%1 %2" )
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
