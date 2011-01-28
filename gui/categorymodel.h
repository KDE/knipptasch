/*
 * Copyright 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QAbstractItemModel>

class Account;
class Category;


/**
 * @class CategoryModel
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class CategoryModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        enum Column
        {
            CATEGORY_NAME = 0,
            CATEGORY_COLOR,
            CATEGORY_USAGE,
            ENTRYCOUNT
        };

        explicit CategoryModel(QObject *parent = 0);
        explicit CategoryModel(Account *account, QObject *parent = 0);

        Account* account();
        const Account* account() const;
        void setAccount(Account *account);

        const Category* category(const QModelIndex &index) const;
        Category* category(const QModelIndex &index);

        QModelIndex categoryIndex(const Category *category) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

        bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

        Qt::ItemFlags flags(const QModelIndex &index) const;
        Qt::DropActions supportedDropActions() const;

        QStringList mimeTypes() const;
        QMimeData* mimeData(const QModelIndexList &indexes) const;
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;

    private:
        QModelIndex findCategoryIndex(const QModelIndex &parent, const Category *category) const;

    private:
        Account *m_account;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
