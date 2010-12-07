/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
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
#ifndef CATEGORY_H
#define CATEGORY_H

#include "object.h"
#include <QColor>

class Money;


class Category : public Object
{
    Q_OBJECT

    public:
        enum ForwardRule
        {
            Daily = 0,      /**< */
            Weekly = 1,     /**< */
            Monthly = 2,    /**< */
            Quarterly = 3,  /**< */
            Yearly = 4      /**< */
        };

        Category(QObject *parent = 0);
        Category(const QString &name, QObject *parent = 0);
        ~Category();

        bool isModified() const;
        void setModified(bool state = true);

        QString name() const;
        void setName(const QString &name);

        QColor color() const;
        void setColor(const QColor &color = QColor());

        bool maximumLimitEnabled() const;
        void setMaximumLimitEnabled(bool b);
        Money maximumLimit() const;
        void setMaximumLimit(const Money &m);
        ForwardRule maximumUnit() const;
        void setMaximumUnit(ForwardRule rule);

        bool minimumLimitEnabled() const;
        void setMinimumLimitEnabled(bool b);
        Money minimumLimit() const;
        void setMinimumLimit(const Money &m);
        ForwardRule minimumUnit() const;
        void setMinimumUnit(ForwardRule rule);

        bool hasCategories() const;
        int countCategories() const;
        const Category* category(int index) const;
        Category* category(int index);
        void addCategory(Category *p);
        Category* addCategory(const QString &str);
        Category* takeCategory(int index);
        void removeCategory(int index);
        void clearCategories();

        Category* findCategoryByHash(const QByteArray &hash);
        const Category* findCategoryByHash(const QByteArray &hash) const;

        virtual QDataStream& serialize(QDataStream &stream) const;
        virtual QDataStream& deserialize(const Account *account, QDataStream &stream);

    signals:
        void valueChanged();
        void categoryChanged();

    private:
        class Private;
        Private * const d;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
