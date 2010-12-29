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
#ifndef ACCOUNTSORTFILTERPROXYMODEL_H
#define ACCOUNTSORTFILTERPROXYMODEL_H

#include "accountmodel.h"
#include "backend/money.h"

#include <QSortFilterProxyModel>
#include <QMap>


/**
 * @class AccountSortFilterProxyModel
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class AccountSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:
        explicit AccountSortFilterProxyModel(QObject *parent = 0);

        Account* account();
        const Account* account() const;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    public slots:
        void updateCache(int firstRow = 0);

    protected:
        bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    private:
        int lessThanByType(const QModelIndex &left, const QModelIndex &right) const;
        int lessThanDateBased(const QModelIndex &left, const QModelIndex &right) const;

    private:
        mutable QMap<int, Money> m_cache;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
