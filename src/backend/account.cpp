/*
 * Copyright 2007-2010 Stefan Böhmann <kde@hilefoks.org>
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
#include "account.h"

#include "posting.h"
#include "money.h"

#include <QDataStream>
#include <cstdlib>



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



struct Account::Private
{
    Private()
      : openingdate( QDate( QDate::currentDate().year(), 1, 1 ) ),
        minbalanceenabled( false ),
        maxbalanceenabled( false ),
        modified( false )
    {
    }

    ~Private()
    {
        qDeleteAll( postings );
    }

    QString name;
    QString number;
    QString description;
    QString iban;
    QString owner;
    QString institution;
    QString bic;
    QDate openingdate;
    Money openingbalance;
    bool minbalanceenabled;
    bool maxbalanceenabled;
    Money minbalance;
    Money maxbalance;

    QList<Posting*> postings;
    mutable bool modified;
};



Account::Account()
  : d( new Account::Private )
{
}


Account::~Account()
{
    delete d;
}


bool Account::isModified() const
{
    if( d->modified ) {
        return true;
    }

    foreach(Posting *p, d->postings) {
        if( p && p->isModified() ) {
            d->modified = true;
            return true;
        }
    }

    return false;
}


void Account::setModified(bool state)
{
    if( state == false ) {
        foreach(Posting *p, d->postings) {
            p->setModified( false );
        }
    }

    d->modified = state;
}


QString Account::name() const
{
    return d->name;
}


void Account::setName(const QString &name)
{
    d->name = name;
    setModified();
}


QString Account::number() const
{
    return d->number;
}


void Account::setNumber(const QString &number)
{
    d->number = number;
    setModified();
}


QString Account::description() const
{
    return d->description;
}


void Account::setDescription(const QString &desc)
{
    d->description = desc;
    setModified();
}


QDate Account::openingDate() const
{
    return d->openingdate;
}


void Account::setOpeningDate(const QDate &date)
{
    d->openingdate = date;
    setModified();
}


Money Account::openingBalance() const
{
    return d->openingbalance;
}


void Account::setOpeningBalance(Money money)
{
    d->openingbalance = money;
    setModified();
}


bool Account::minimumBalanceEnabled() const
{
    return d->minbalanceenabled;
}


void Account::setMinimumBalanceEnabled(bool b)
{
    d->minbalanceenabled = b;
    setModified();
}


Money Account::minimumBalance() const
{
    return d->minbalance;
}


void Account::setMinimumBalance(Money money)
{
    d->minbalance = money.abs();
    setModified();
}


bool Account::maximumBalanceEnabled() const
{
    return d->maxbalanceenabled;
}


void Account::setMaximumBalanceEnabled(bool b)
{
    d->maxbalanceenabled = b;
    setModified();
}


Money Account::maximumBalance() const
{
    return d->maxbalance;
}


void Account::setMaximumBalance(Money money)
{
    d->maxbalance = money.abs();
    setModified();
}


QString Account::iban() const
{
    return d->iban;
}


void Account::setIban(const QString &iban)
{
    d->iban = iban;
    setModified();
}


QString Account::owner() const
{
    return d->owner;
}


void Account::setOwner(const QString &owner)
{
    d->owner = owner;
    setModified();
}


QString Account::institution() const
{
    return d->institution;
}


void Account::setInstitution(const QString &str)
{
    d->institution = str;
}


QString Account::bic() const
{
    return d->bic;
}


void Account::setBic(const QString &str)
{
    d->bic = str;
}


int Account::countPostings() const
{
    return d->postings.size();
}


void Account::addPosting(Posting *ptr)
{
    Q_ASSERT( ptr );

    d->postings.append( ptr );
    setModified();
}


Posting* Account::posting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    return d->postings.at( index );
}


const Posting* Account::posting(int index) const
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    return d->postings.at( index );
}


Posting* Account::takePosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    Posting *posting = d->postings.takeAt( index );
    setModified();

    return posting;
}


void Account::removePosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    d->postings.removeAt( index );
    setModified();
}


void Account::deletePosting(int index)
{
    delete takePosting( index );
}


Account* Account::demoAccount()
{
    QList<int> v;
    v << -10 << -15 << -20 << -25 << -30 << -35 << -40 << -50
        << -60 << -70 << -80 << -100 << -120 << -150 << -200;

    Account *acc = new Account;

    acc->setNumber( "105626300" );
    acc->setOpeningBalance( 2000 );

    for(int i = -15; i < 3; ++i) {
        QDate date(
            QDate::currentDate().year(),
            QDate::currentDate().addMonths( i ).month(),
            1
        );

        initDemoAccountAddPosting( acc, date.addDays( std::rand() % 6 ), "Rent", -548.50, i < 0 );

        if( i < 0 ) {
            initDemoAccountAddPosting(
                acc,
                date.addDays( std::rand() % 6 ),
                "Phone and Internet",
                -25 - ( ( std::rand() % 5000 + 100 ) / 100.00 )
            );
            initDemoAccountAddPosting(
                acc,
                date.addDays( std::rand() % 6 ),
                "Salary",
                1800.00 + ( ( std::rand() % 50000 + 100 ) / 100.00 )
            );

            for(int i = ( std::rand() % 5 + 1 ) ; i >= 0; --i) {
                initDemoAccountAddPosting(
                    acc,
                    date.addDays( std::rand() % date.daysInMonth() + 1 ),
                    "ATM",
                    v[ std::rand() % v.size() ]
                );
            }
        }
        else {
            initDemoAccountAddPosting(
                acc,
                date.addDays( std::rand() % 6 ),
                "Phone and Internet",
                Money(),
                false
            );

            initDemoAccountAddPosting(
                acc,
                date.addDays( std::rand() % 6 ),
                "Salary",
                Money(),
                false
            );
        }
    }

    return acc;
}


QDataStream& operator<<(QDataStream &stream, const Account &acc)
{
    stream << acc.name();
    stream << acc.number();
    stream << acc.description();
    stream << acc.openingDate();
    stream << acc.openingBalance();
    stream << acc.minimumBalanceEnabled();
    stream << acc.minimumBalance();
    stream << acc.maximumBalanceEnabled();
    stream << acc.maximumBalance();
    stream << acc.iban();
    stream << acc.owner();
    stream << acc.institution();

    int count = acc.countPostings();
    stream << count;
    for(int index = 0; index < count; ++index) {
        const Posting *p = acc.posting( index );
        stream << (*p);
    }

    return stream;
}


QDataStream& operator>>(QDataStream &stream, Account &acc)
{
    QString str;
    QDate date;
    Money money;
    bool b;

    stream >> str;
    acc.setName( str );

    stream >> str;
    acc.setNumber( str );

    stream >> str;
    acc.setDescription( str );

    stream >> date;
    acc.setOpeningDate( date );

    stream >> money;
    acc.setOpeningBalance( money );

    stream >> b;
    acc.setMinimumBalanceEnabled( b );

    stream >> money;
    acc.setMinimumBalance( money );

    stream >> b;
    acc.setMaximumBalanceEnabled( b );

    stream >> money;
    acc.setMaximumBalance( money );

    stream >> str;
    acc.setIban( str );

    stream >> str;
    acc.setOwner( str );

    stream >> str;
    acc.setInstitution( str );

    int count;
    stream >> count;

    for(int i = 0; i < count; ++i) {
        Posting *p = new Posting;
        stream >> (*p);
    }

    return stream;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
