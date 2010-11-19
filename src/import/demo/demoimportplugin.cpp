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

#include <compat/iconloader.h>

#include <QPixmap>
#include <QInputDialog>

#include <cstdlib>
#include <QDate>


void initDemoAccountAddPosting(Account *acc, const QDate &date, const QString &postingText, const Money &amount, bool valueDate = true)
{
    Q_ASSERT( acc );

    Posting *p = new Posting;
    p->setMaturity( date );
    p->setPostingText( postingText );
    p->setAmount( amount );
    p->setValueDate( valueDate ? date.addDays( std::rand() % 8 ) : QDate() );

    acc->addPosting( p );
}



DemoImportPlugin::DemoImportPlugin()
{

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
            v << -10 << -15 << -20 << -25 << -30 << -35 << -40 << -50
                << -60 << -70 << -80 << -90 << -100 << -120 << -150 << -200 << -500;

        Account *acc = new Account;

        acc->setName( QObject::tr( "Example Account" ) );
        acc->setNumber( "105626320" );
        acc->setOpeningBalance( 42.21 );
        acc->setOpeningDate( QDate::currentDate().addMonths( -count + 1 ) );

        for(int i = -count; i < 3; ++i) {
            QDate date(
                QDate::currentDate().year(),
                QDate::currentDate().addMonths( i ).month(),
                1
            );

            initDemoAccountAddPosting( acc, date.addDays( std::rand() % 6 ),
                                    QObject::tr( "Rent" ) , -548.50, i < 0 );

            if( i < 0 ) {
                initDemoAccountAddPosting(
                    acc,
                    date.addDays( std::rand() % 6 ),
                    QObject::tr( "Phone and Internet" ),
                    -25 - ( ( std::rand() % 5000 + 100 ) / 100.00 )
                );
                initDemoAccountAddPosting(
                    acc,
                    date.addDays( std::rand() % 6 ),
                    QObject::tr( "Salary" ),
                    1000.00 + ( ( std::rand() % 50000 + 100 ) / 100.00 )
                );

                for(int i = ( std::rand() % 15 + 1 ) ; i >= 0; --i) {
                    initDemoAccountAddPosting(
                        acc,
                        date.addDays( std::rand() % date.daysInMonth() + 1 ),
                        QObject::tr( "ATM" ),
                        v[ std::rand() % v.size() ]
                    );
                }
            }
            else {
                initDemoAccountAddPosting(
                    acc,
                    date.addDays( std::rand() % 6 ),
                    QObject::tr( "Phone and Internet" ),
                    Money(),
                    false
                );

                initDemoAccountAddPosting(
                    acc,
                    date.addDays( std::rand() % 6 ),
                    QObject::tr( "Salary" ),
                    Money(),
                    false
                );
            }
        }

        acc->setModified( false );

        return acc;
    }

    return 0;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
