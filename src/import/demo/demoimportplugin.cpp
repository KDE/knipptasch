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
#include "demoimportplugin.h"

#include "backend/account.h"
#include "backend/posting.h"
#include "backend/money.h"
#include "backend/category.h"

#include "compat/iconloader.h"

#include <QPixmap>
#include <QInputDialog>
#include <QDate>

#include <QDebug>



Money initDemoAccountAddPosting(Account *acc, const QDate &date, const QString &postingText, const Money &amount, bool valueDate = true)
{
    Q_ASSERT( acc );

    Posting *p = new Posting;
    p->setMaturity( date );
    p->setPostingText( postingText );
    p->setAmount( amount );
    p->setValueDate( valueDate ? date.addDays( qrand() % 8 ) : QDate() );

    acc->addPosting( p );

    return p->amount();
}

Money initRent(Account *acc, const QDate &date)
{
    return initDemoAccountAddPosting( acc, date.addDays( qrand() % 6 ),
                                      QObject::tr( "Rent" ) , -348.50,
                                      date < QDate::currentDate() );
}



DemoImportPlugin::DemoImportPlugin()
{
    qsrand( QTime::currentTime().msec() );
}


QString DemoImportPlugin::importActionName() const
{
    return QObject::tr( "Import &Example File" );
}


QPixmap DemoImportPlugin::importActionIcon() const
{
    return BarIcon("applications-education-miscellaneous");
}


Account* DemoImportPlugin::importAccount(QWidget *parent) const
{
    bool ok;
    int count = QInputDialog::getInt( parent,  // krazy:exclude=qclasses
                                      QString(),
                                      QObject::tr("Past month:"),
                                      15, 1, 120, 1, &ok );
    if( ok ) {
        QList<int> v;
        v << -5 << -10 << -15 << -20 << -25 << -30 << -35 << -40 << -50
          << -60 << -70 << -80 << -90 << -100 << -120 << -130 << -150
          << -200 << -250 << -300;

        QStringList cost;
        cost << QObject::tr( "ATM" );
        cost << QObject::tr( "Food" );
        cost << QObject::tr( "Clothes" );
        cost << QObject::tr( "Reading material" );
        cost << QObject::tr( "Repairs" );
        cost << QObject::tr( "Fuel" );
        cost << QObject::tr( "Books" );
        cost << QObject::tr( "Dining Out" );
        cost << QObject::tr( "Gifts" );
        cost << QObject::tr( "Tax" );
        cost << QObject::tr( "Insurance" );
        cost << QObject::tr( "Office Supplies" );

        Account *acc = new Account;

        acc->setName( QObject::tr( "Example Account" ) );
        acc->setNumber( "105626320" );
        acc->setOpeningBalance( 542.20 );
        acc->setOpeningDate( QDate::currentDate().addMonths( -count ) );

        {
            Category *p = acc->rootCategory()->addCategory( QObject::tr( "Auto" ) );
            p->setColor( Qt::blue );

            p->addCategory( QObject::tr( "Fuel" ) )->setColor( Qt::red );
            p->addCategory( QObject::tr( "Insurance" ) )->setColor( Qt::green );
            p->addCategory( QObject::tr( "Tax" ) )->setColor( Qt::gray );
            p->addCategory( QObject::tr( "Service" ) )->setColor( Qt::darkMagenta );
        }
        {
            Category *p = acc->rootCategory()->addCategory( QObject::tr( "Food" ) );
            p->setColor( Qt::cyan );
            p->addCategory( QObject::tr( "Dining Out" ) )->setColor( Qt::darkBlue );
        }
        {
            Category *p = acc->rootCategory()->addCategory( QObject::tr( "Recreation" ) );
            p->setColor( Qt::yellow );

            p->addCategory( QObject::tr( "Books" ) )->setColor( Qt::darkRed );
            p->addCategory( QObject::tr( "Photo" ) )->setColor( Qt::black );
            p->addCategory( QObject::tr( "Sport" ) );
            Category *p1 = p->addCategory( QObject::tr( "Culture" ) );
            p1->setColor( Qt::darkGreen );
            p1->addCategory( QObject::tr( "Theater" ) )->setColor( Qt::lightGray );
            p1->addCategory( QObject::tr( "Stage" ) );
            p1->addCategory( QObject::tr( "Exposition" ) );
            p1->addCategory( QObject::tr( "Classical Music" ) );
            p1->addCategory( QObject::tr( "Museum" ) );
            p1->addCategory( QObject::tr( "Art" ) );
            p->addCategory( QObject::tr( "Entertainment" ) )->setColor( Qt::darkYellow );
        }

        Money total = acc->openingBalance();
        for( QDate d = acc->openingDate(); d < QDate::currentDate().addMonths( 4 ); d = d.addMonths( 1 ) ) {
            Money month;
            QDate date( d.year(), d.month(), 1 );

            month += initRent( acc, date );

            if( date < QDate::currentDate() ) {
                month += initDemoAccountAddPosting(
                    acc,
                    date.addDays( qrand() % 6 ),
                    QObject::tr( "Phone and Internet" ),
                    -20 - ( ( qrand() % 5000 + 100 ) / 100.00 )
                );

                month += initDemoAccountAddPosting(
                    acc,
                    date.addDays( qrand() % 6 ),
                    QObject::tr( "Salary" ),
                    1000.00 + ( ( qrand() % 50000 + 100 ) / 100.00 )
                );

                if( total > 800.00 ) {
                    month += initDemoAccountAddPosting(
                        acc,
                        date.addDays( qrand() % date.daysInMonth() + 1 ),
                        QObject::tr( "Travel" ),
                        -1700.00 + ( ( qrand() % 50000 + 100 ) / 100.00 )
                    );
                }

                while( month > 200.0 ) {
                    month += initDemoAccountAddPosting(
                        acc,
                        date.addDays( qrand() % date.daysInMonth() + 1 ),
                        cost.at( qrand() % cost.size() ),
                        v[ qrand() % v.size() ]
                    );
                }

                if( month < -50.0 ) {
                    month += initDemoAccountAddPosting(
                        acc,
                        date.addDays( qrand() % date.daysInMonth() + 1 ),
                        QObject::tr( "Dividend" ),
                        Money( v[ qrand() % v.size() ] ).abs()
                    );
                }
            }
            else {
                month += initDemoAccountAddPosting(
                    acc,
                    date.addDays( qrand() % 6 ),
                    QObject::tr( "Phone and Internet" ),
                    Money(),
                    false
                );

                month += initDemoAccountAddPosting(
                    acc,
                    date.addDays( qrand() % 6 ),
                    QObject::tr( "Salary" ),
                    Money(),
                    false
                );
            }

            total += month;
        }

        acc->setModified( false );

        return acc;
    }

    return 0;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
