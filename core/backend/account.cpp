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
#include "category.h"

#include <QDataStream>
#include <QDebug>
#include <QPointer>



const Category* doFindCategoryParent(const Category *current, const Category *target)
{
    Q_ASSERT( current );
    Q_ASSERT( target );
    Q_ASSERT( current != target );

    for(int i = 0; i < current->countCategories(); ++i) {
        const Category *c = current->category( i );
        Q_ASSERT( c );

        if( c == target ) {
            return current;
        }

        const Category *child = doFindCategoryParent( c, target );
        if( child ) {
            return child;
        }
    }

    return 0;
}



Category* doFindCategoryParent(Category *current, const Category *target)
{
    Q_ASSERT( current );
    Q_ASSERT( target );
    Q_ASSERT( current != target );

    for(int i = 0; i < current->countCategories(); ++i) {
        Category *c = current->category( i );
        Q_ASSERT( c );

        if( c == target ) {
            return current;
        }

        Category *child = doFindCategoryParent( c, target );
        if( child ) {
            return child;
        }
    }

    return 0;
}




struct Account::Private
{
    Private()
      : openingdate( QDate( QDate::currentDate().year(), 1, 1 ) ),
        minbalanceenabled( false ),
        maxbalanceenabled( false ),
        usePassword( false ),
        level( Average ),
        category( new Category ),
        modified( false )
    {
    }

    ~Private()
    {
        qDeleteAll( postings );
        delete category;
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
    QByteArray password;
    SecurityLevel level;

    QList<Posting*> postings;
    Category *category;

    mutable QVector<QPointer<Object> > objects;

    bool modified;
};



Account::Account(QObject *parent)
  : Object( parent ),
    d( new Account::Private )
{
    connect( d->category, SIGNAL( valueChanged() ), this, SIGNAL( categoryChanged() ) );
    connect( d->category, SIGNAL( categoryChanged() ), this, SIGNAL( categoryChanged() ) );
}


Account::~Account()
{
    delete d;
}


bool Account::isModified() const
{
    if( d->modified || Object::isModified() ) {
        return true;
    }

    foreach(const Posting *p, d->postings) {
        Q_ASSERT( p );
        if( p->isModified() ) {
            d->modified = true;
            return true;
        }
    }

    return false;
}


void Account::setModified(bool state)
{
    if( !state ) {
        foreach(Posting *p, d->postings) {
            p->setModified( false );
        }
    }

    d->modified = state;
    Object::setModified( state );
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

        emit valueChanged();
    }
}


QByteArray Account::password() const
{
    return d->password;
}


void  Account::setPassword(const QByteArray &pw)
{
    if( pw != d->password ) {
        d->password = pw;
        setModified();

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
        emit openingBalanceChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
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

        emit valueChanged();
    }
}


int Account::countPostings() const
{
    return d->postings.size();
}


void Account::addPosting(Posting *ptr)
{
    Q_ASSERT( ptr );

    connect( ptr, SIGNAL( valueChanged() ), this, SIGNAL( postingChanged() ) );
    d->postings.append( ptr );
    setModified();

    emit postingChanged();
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
        Q_ASSERT( p );
        list.append( p );
    }

    return list;
}


Posting* Account::takePosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    Posting *posting = d->postings.takeAt( index );
    disconnect( posting, 0, this, 0 );

    setModified();
    emit postingChanged();

    return posting;
}


void Account::removePosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    Posting *posting = d->postings.takeAt( index );
    disconnect( posting, 0, this, 0 );

    setModified();
    emit postingChanged();
}


void Account::deletePosting(int index)
{
    delete takePosting( index );
}


Category* Account::rootCategory()
{
    Q_ASSERT( d );
    Q_ASSERT( d->category );

    return d->category;
}


const Category* Account::rootCategory() const
{
    Q_ASSERT( d );
    Q_ASSERT( d->category );

    return d->category;
}


Category* Account::findCategoryParent(const Category *child)
{
    if( !child ) {
        return 0;
    }

    if( child == rootCategory() ) {
        return 0;
    }

    return doFindCategoryParent( rootCategory(), child );
}


const Category* Account::findCategoryParent(const Category *child) const
{
    if( !child ) {
        return 0;
    }

    if( child == rootCategory() ) {
        return 0;
    }

    return doFindCategoryParent( rootCategory(), child );
}


int Account::identifierByObject(const Object *object) const
{
    if( !object ) {
        return -1;
    }

    Object *obj = const_cast<Object*>( object );

    if( !d->objects.contains( obj ) ) {
        d->objects.append( obj );
    }

    Q_ASSERT( d->objects.lastIndexOf( obj ) >= 0 );
    return d->objects.lastIndexOf( obj );
}


Object* Account::objectByIdentifier(int id) const
{
    return d->objects.value( id );
}


void Account::clearObjectIdentifier()
{
    d->objects.clear();
}


QDataStream& Account::serialize(QDataStream &stream) const
{
    Object::serialize( stream );

    stream << d->name;
    stream << d->number;
    stream << d->description;
    stream << d->openingdate;
    stream << d->openingbalance;
    stream << d->minbalanceenabled;
    stream << d->minbalance;
    stream << d->maxbalanceenabled;
    stream << d->maxbalance;
    stream << d->iban;
    stream << d->owner;
    stream << d->institution;
    stream << d->bic;
    d->category->serialize( stream );

    stream << static_cast<quint32>( d->postings.size() );
    foreach(const Posting *p, d->postings) {
        p->serialize( stream );
    }

    return stream;
}


QDataStream& Account::deserialize(const Account *account, QDataStream &stream)
{
    Object::deserialize( account, stream );

    stream >> d->name;
    stream >> d->number;
    stream >> d->description;
    stream >> d->openingdate;
    stream >> d->openingbalance;
    stream >> d->minbalanceenabled;
    stream >> d->minbalance;
    stream >> d->maxbalanceenabled;
    stream >> d->maxbalance;
    stream >> d->iban;
    stream >> d->owner;
    stream >> d->institution;
    stream >> d->bic;
    d->category->deserialize( account, stream );

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i) {
        Posting *p = new Posting;
        p->deserialize( account, stream );
        addPosting( p );
    }

    return stream;
}


QDataStream& operator<<(QDataStream &stream, const Account &acc)
{
    return acc.serialize( stream );
}


QDataStream& operator>>(QDataStream &stream, Account &acc)
{
    return acc.deserialize( &acc, stream );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
