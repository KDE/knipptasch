/*
 * Copyright 2008-2010  Stefan Böhmann <kde@hilefoks.org>
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
#include "baseposting.h"
#include "money.h"
#include "category.h"
#include "account.h"

#include <QDataStream>
#include <QDebug>


struct BasePosting::Private
{
    Private()
      : page( 0 ),
        category( 0 ),
        modified( false )
    {
    }

    Money amount;
    QString postingtext;
    QString description;
    QString voucher;
    QString methodOfPayment;

    Maturity maturity;
    ValueDate valuedate;
    QDate warranty;
    int page;

    Category *category;
    QString payee;

    bool modified;
};




BasePosting::BasePosting()
  : d( new BasePosting::Private )
{
}


BasePosting::~BasePosting()
{
    delete d;
}


bool BasePosting::isModified() const
{
    if( d->modified || ( d->category && d->category->isModified() ) || Object::isModified() ) {
        return true;
    }

    return false;
}


void BasePosting::setModified(bool state)
{
    if( state ) {
        d->modified = true;
    }
    else {
        Object::setModified( false );

        if( d->category ) {
            d->category->setModified( false );
        }

        d->modified = false;
    }
}


QString BasePosting::postingText() const
{
    return d->postingtext;
}


void BasePosting::setPostingText(const QString &str)
{
    d->postingtext = str;
    setModified();
}


Maturity BasePosting::maturity() const
{
    return d->maturity;
}


void BasePosting::setMaturity(const Maturity &date)
{
    d->maturity = date;
    setModified();
}


ValueDate BasePosting::valueDate() const
{
    return d->valuedate;
}


void BasePosting::setValueDate(const ValueDate &date)
{
    d->valuedate = date;
    setModified();
}


Money BasePosting::amount() const
{
    return d->amount;
}


void BasePosting::setAmount(const Money &m)
{
    d->amount = m;
    setModified();
}


int BasePosting::page() const
{
    return d->page;
}


void BasePosting::setPage(int p)
{
    d->page = p;
    setModified();
}


QString BasePosting::description() const
{
    return d->description;
}


void BasePosting::setDescription(const QString &str)
{
    d->description = str;
    setModified();
}


QString BasePosting::voucher() const
{
    return d->voucher;
}


void BasePosting::setVoucher(const QString &str)
{
    d->voucher = str;
    setModified();
}


QDate BasePosting::warranty() const
{
    return d->warranty;
}


void BasePosting::setWarranty(const QDate &date)
{
    d->warranty = date;
    setModified();
}


QString BasePosting::methodOfPayment() const
{
    return d->methodOfPayment;
}


void BasePosting::setMethodOfPayment(const QString &str)
{
    d->methodOfPayment = str;
    setModified();
}


Category* BasePosting::category()
{
    return d->category;
}


const Category* BasePosting::category() const
{
    return d->category;
}


void BasePosting::setCategory(Category *category)
{
    if( d->category != category ) {
        d->category = category;
        setModified();
    }
}


void BasePosting::clearCategory()
{
    if( d->category ) {
        d->category = 0;
        setModified();
    }
}


QString BasePosting::payee() const
{
    return d->payee;
}


void BasePosting::setPayee(const QString &str)
{
    d->payee = str;
    setModified();
}


QDataStream& BasePosting::serialize(QDataStream &stream) const
{
    Object::serialize( stream );

    stream << d->amount;
    stream << d->postingtext;
    stream << d->description;
    stream << d->voucher;
    stream << d->methodOfPayment;
    stream << d->maturity;
    stream << d->valuedate;
    stream << d->warranty;
    stream << d->page;
    stream << d->payee;
    stream << ( d->category ? d->category->hash() : QByteArray() );

    return stream;
}


QDataStream& BasePosting::deserialize(const Account *account, QDataStream &stream)
{
    Object::deserialize( account, stream );

    stream >> d->amount;
    stream >> d->postingtext;
    stream >> d->description;
    stream >> d->voucher;
    stream >> d->methodOfPayment;
    stream >> d->maturity;
    stream >> d->valuedate;
    stream >> d->warranty;
    stream >> d->page;
    stream >> d->payee;

    QByteArray hash;
    stream >> hash;

    if( !hash.isEmpty() ) {
        const Category *c = account->rootCategory()->findCategoryByHash( hash );

        if( c ) {
            d->category = const_cast<Category*>( c );
        }
        else {
            qDebug() << "No category for hash" << hash << "found!";
        }
    }

    return stream;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
