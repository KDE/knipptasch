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
#include <QDebug>


struct Account::Private
{
    Private()
      : openingdate( QDate( QDate::currentDate().year(), 1, 1 ) ),
        minbalanceenabled( false ),
        maxbalanceenabled( false ),
        usePassword( false ),
        level( Average ),
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

    bool usePassword;
    QCA::SecureArray password;
    SecurityLevel level;

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


bool Account::isPasswordEnabled() const
{
    return d->usePassword;
}


void Account::setPasswordEnabled(bool state)
{
    if( state != d->usePassword ) {
        d->usePassword = state;
        setModified();
    }
}


QByteArray Account::password() const
{
#if defined( WITH_QCA2 )
    return d->password.toByteArray();
#else
    return d->password;
#endif
}


void  Account::setPassword(const QCA::SecureArray &pw)
{
    if( pw != d->password ) {
        d->password = pw;
        setModified();
    }
}


Account::SecurityLevel Account::securityLevel() const
{
    return d->level;
}


void Account::setSecurityLevel(Account::SecurityLevel level)
{
    if( level != d->level ) {
        d->level = level;
        setModified();
    }
}


QString Account::name() const
{
    return d->name;
}


void Account::setName(const QString &name)
{
    if( name != d->name ) {
        d->name = name;
        setModified();
    }
}


QString Account::number() const
{
    return d->number;
}


void Account::setNumber(const QString &number)
{
    if( number != d->number ) {
        d->number = number;
        setModified();
    }
}


QString Account::description() const
{
    return d->description;
}


void Account::setDescription(const QString &desc)
{
    if( desc != d->description ) {
        d->description = desc;
        setModified();
    }
}


QDate Account::openingDate() const
{
    return d->openingdate;
}


void Account::setOpeningDate(const QDate &date)
{
    if( date != d->openingdate ) {
        d->openingdate = date;
        setModified();
    }
}


Money Account::openingBalance() const
{
    return d->openingbalance;
}


void Account::setOpeningBalance(Money money)
{
    if( money.cents() != d->openingbalance.cents() ) {
        d->openingbalance = money;
        setModified();
    }
}


bool Account::minimumBalanceEnabled() const
{
    return d->minbalanceenabled;
}


void Account::setMinimumBalanceEnabled(bool b)
{
    if( b != d->minbalanceenabled ) {
        d->minbalanceenabled = b;
        setModified();
    }
}


Money Account::minimumBalance() const
{
    return d->minbalance;
}


void Account::setMinimumBalance(Money money)
{
    if( money.cents() != d->minbalance.cents() ) {
        d->minbalance = money;
        setModified();
    }
}


bool Account::maximumBalanceEnabled() const
{
    return d->maxbalanceenabled;
}


void Account::setMaximumBalanceEnabled(bool b)
{
    if( b != d->maxbalanceenabled ) {
        d->maxbalanceenabled = b;
        setModified();
    }
}


Money Account::maximumBalance() const
{
    return d->maxbalance;
}


void Account::setMaximumBalance(Money money)
{
    if( money.cents() != d->maxbalance.cents() ) {
        d->maxbalance = money;
        setModified();
    }
}


QString Account::iban() const
{
    return d->iban;
}


void Account::setIban(const QString &iban)
{
    if( iban != d->iban ) {
        d->iban = iban;
        setModified();
    }
}


QString Account::owner() const
{
    return d->owner;
}


void Account::setOwner(const QString &owner)
{
    if( owner != d->owner ) {
        d->owner = owner;
        setModified();
    }
}


QString Account::institution() const
{
    return d->institution;
}


void Account::setInstitution(const QString &institution)
{
    if( institution != d->institution ) {
        d->institution = institution;
        setModified();
    }
}


QString Account::bic() const
{
    return d->bic;
}


void Account::setBic(const QString &bic)
{
    if( bic != d->bic ) {
        d->bic = bic;
        setModified();
    }
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


QList<const Posting*> Account::postings() const
{
    QList<const Posting*> list;
    foreach(Posting *p, d->postings) {
        list.append( p );
    }

    return list;
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


QDataStream& operator<<(QDataStream &stream, const Account &acc)
{
    qDebug() << "Serialize Account" << acc.name();

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
    stream << acc.bic();

    quint32 count = acc.countPostings();
    stream << count;

    qDebug() << "  Serialize" << count << "elements";

    for(quint32 index = 0; index < count; ++index) {
        const Posting *p = acc.posting( index );
        stream << *p;
    }

    return stream;
}


QDataStream& operator>>(QDataStream &stream, Account &acc)
{
    qDebug() << "Deserialize Account";

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

    stream >> str;
    acc.setBic( str );

    quint32 count;
    stream >> count;

    qDebug() << "  Deserialize" << count << "elements";

    for(quint32 i = 0; i < count; ++i) {
        Posting *p = new Posting;
        stream >> *p;

        acc.addPosting( p );
    }

    return stream;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
