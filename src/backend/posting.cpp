/*
 * Copyright 2008  Stefan Böhmann <kde@hilefoks.org>
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
#include "posting.h"
#include "money.h"

#include <QDataStream>


struct Posting::Private
{
    Private()
      : page(0),
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

    QString category;
    QString payee;

    mutable bool modified;
};




Posting::Posting()
  : d( new Posting::Private )
{
}


Posting::~Posting()
{
    delete d;
}


bool Posting::isModified() const
{
    return d->modified;
}


void Posting::setModified(bool b)
{
    d->modified = b;
}



QString Posting::postingText() const
{
    return d->postingtext;
}


void Posting::setPostingText(const QString &str)
{
    d->postingtext = str;
    setModified();
}


Maturity Posting::maturity() const
{
    return d->maturity;
}


void Posting::setMaturity(const Maturity &date)
{
    d->maturity = date;
    setModified();
}


ValueDate Posting::valueDate() const
{
    return d->valuedate;
}


void Posting::setValueDate(const ValueDate &date)
{
    d->valuedate = date;
    setModified();
}


Money Posting::amount() const
{
    return d->amount;
}


void Posting::setAmount(const Money &m)
{
    d->amount = m;
    setModified();
}


int Posting::page() const
{
    return d->page;
}


void Posting::setPage(int p)
{
    d->page = p;
    setModified();
}


QString Posting::description() const
{
    return d->description;
}


void Posting::setDescription(const QString &str)
{
    d->description = str;
    setModified();
}


QString Posting::voucher() const
{
    return d->voucher;
}


void Posting::setVoucher(const QString &str)
{
    d->voucher = str;
    setModified();
}


QDate Posting::warranty() const
{
    return d->warranty;
}


void Posting::setWarranty(const QDate &date)
{
    d->warranty = date;
    setModified();
}


QString Posting::methodOfPayment() const
{
    return d->methodOfPayment;
}


void Posting::setMethodOfPayment(const QString &str)
{
    d->methodOfPayment = str;
    setModified();
}


QString Posting::category() const
{
    return d->category;
}


void Posting::setCategory(const QString &str)
{
    d->category = str;
    setModified();
}


QString Posting::payee() const
{
    return d->payee;
}


void Posting::setPayee(const QString &str)
{
    d->payee = str;
    setModified();
}


QDataStream& operator<<(QDataStream &stream, const Posting &posting)
{
    stream << posting.amount();
    stream << posting.postingText();
    stream << posting.description();
    stream << posting.voucher();
    stream << posting.methodOfPayment();
    stream << posting.maturity();
    stream << posting.valueDate();
    stream << posting.warranty();
    stream << posting.page();
    stream << posting.category();
    stream << posting.payee();

    return stream;
}


QDataStream& operator>>(QDataStream &stream, Posting &posting)
{
    int integer;
    QDate date;
    QString str;
    Money money;

    stream >> money;
    posting.setAmount( money );

    stream >> str;
    posting.setPostingText( str );

    stream >> str;
    posting.setDescription( str );

    stream >> str;
    posting.setVoucher( str );

    stream >> str;
    posting.setMethodOfPayment( str );

    stream >> date;
    posting.setMaturity( date );

    stream >> date;
    posting.setValueDate( date );

    stream >> date;
    posting.setWarranty( date );

    stream >> integer;
    posting.setPage( integer );

    stream >> str;
    posting.setCategory( str );

    stream >> str;
    posting.setPayee( str );

    return stream;
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
