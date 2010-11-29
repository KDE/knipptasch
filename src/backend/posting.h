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
#ifndef POSTING_H
#define POSTING_H

#include "object.h"

#include <QDate>

typedef QDate Maturity;
typedef QDate ValueDate;
class Money;

class QDataStream;


class Posting : public Object
{
    public:
        Posting();
        ~Posting();

        bool isModified() const;
        void setModified(bool state = true);

        QString postingText() const;
        void setPostingText(const QString &str);

        Maturity maturity() const;
        void setMaturity(const Maturity &date);

        ValueDate valueDate() const;
        void setValueDate(const ValueDate &date);

        Money amount() const;
        void setAmount(const Money &m);

        int page() const;
        void setPage(int p);

        QString description() const;
        void setDescription(const QString &str);

        QString voucher() const;
        void setVoucher(const QString &str);

        QDate warranty() const;
        void setWarranty(const QDate &date);

        QString methodOfPayment() const;
        void setMethodOfPayment(const QString &str);

        QString category() const;
        void setCategory(const QString &str);

        QString payee() const;
        void setPayee(const QString &str);

    protected:
        friend QDataStream& operator<<(QDataStream &stream, const Posting &acc);
        friend QDataStream& operator>>(QDataStream &stream, Posting &acc);

        virtual QDataStream& serialize(QDataStream &stream) const;
        virtual QDataStream& deserialize(QDataStream &stream);

    private:
        class Private;
        Private *d;
};

extern QDataStream& operator<<(QDataStream &stream, const Posting &posting);
extern QDataStream& operator>>(QDataStream &stream, Posting &posting);


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
