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
#ifndef ACCOUNT_H
#define ACCOUNT_H

class Money;
class Posting;
class QString;
class QDate;
class QDataStream;


class Account
{
    public:
        Account();
        ~Account();

        bool isModified() const;
        void setModified(bool state = true);

        QString name() const;
        void setName(const QString &name);

        QString number() const;
        void setNumber(const QString &number) ;

        QString description() const;
        void setDescription(const QString &desc);

        QDate openingDate() const;
        void setOpeningDate(const QDate &date);

        Money openingBalance() const;
        void setOpeningBalance(Money money);

        bool minimumBalanceEnabled() const;
        void setMinimumBalanceEnabled(bool b);

        Money minimumBalance() const;
        void setMinimumBalance(Money money);

        bool maximumBalanceEnabled() const;
        void setMaximumBalanceEnabled(bool b);

        Money maximumBalance() const;
        void setMaximumBalance(Money money);

        QString iban() const;
        void setIban(const QString &iban);

        QString owner() const;
        void setOwner(const QString &owner);

        QString institution() const;
        void setInstitution(const QString &str);

        QString bic() const;
        void setBic(const QString &str);

        int countPostings() const;
        void addPosting(Posting *ptr);
        Posting* posting(int index);
        const Posting* posting(int index) const;
        Posting* takePosting(int index);
        void removePosting(int index);
        void deletePosting(int index);

        static Account* demoAccount();

    private:
        class Private;
        Private *d;

};

extern QDataStream& operator<<(QDataStream &stream, const Account &acc);
extern QDataStream& operator>>(QDataStream &stream, Account &acc);

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
