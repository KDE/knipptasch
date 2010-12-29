/*
 * Copyright 2007-2010 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include "backend/account.h"

#include <QAbstractTableModel>


/**
 * @class AccountModel
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class AccountModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        enum Column
        {
            MATURITY = 0,
            POSTINGTEXT,
            AMOUNT,
            VALUEDATE,
            CATEGORY,
            BALANCE,
            PAYEE,
            STATEMENT,
            VOUCHER,
            WARRANTY,
            PAYMENT,
            DESCRIPTION,
            ENTRYCOUNT
        };

        enum PostingTypeFlag
        {
            Scheduled = 1,
            SplitPosting = 2,
            WithAttachments = 4,
            Incomplete = 8,
            Invalid = 16,
            Current = 32,
            Future = 64
        };

        Q_DECLARE_FLAGS(PostingTypeFlags, PostingTypeFlag)

        explicit AccountModel(QObject *parent = 0);
        ~AccountModel();

        Account* account() { return m_account; }
        const Account* account() const { return m_account; }
        void setAccount(Account *account);

        const Posting* posting(const QModelIndex &index) const;
        const Posting* posting(int row) const;
        Posting* posting(const QModelIndex &index);
        Posting* posting(int row);

        PostingTypeFlags postingType(int row) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
        Qt::ItemFlags flags(const QModelIndex &index) const;

        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    private:
        QVariant backgroundRoleData(const QModelIndex &index) const;
        QVariant foregroundRoleData(const QModelIndex &index) const;
        QVariant textAlignmentRoleData(const QModelIndex &index) const;

        void initDemoAccountAddPosting(const QDate &date, const QString &postingText, const Money &amount, bool valueDate = true);

        PostingTypeFlags postingType(const Posting *p) const;
        static bool postingIsValid(const Posting *p);

    private:
        Account *m_account;
        Posting *m_posting;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( AccountModel::PostingTypeFlags )
Q_DECLARE_METATYPE( AccountModel::PostingTypeFlags )


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
