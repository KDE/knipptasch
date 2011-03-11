/*
 * Copyright 2010, 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef CATEGORYCOMBOBOX_H
#define CATEGORYCOMBOBOX_H

#include "knipptasch_core_export.h"

#include <KComboBox>
#include <QSet>

class QTreeView;
class QEvent;
class QStandardItem;
class QStandardItemModel;
class QStandardItem;

class Category;
class Account;


/**
 * @class CategoryComboBox
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class KNIPPTASCH_CORE_EXPORT CategoryComboBox : public KComboBox
{
        Q_OBJECT

    public:
        CategoryComboBox( const Account *account, QWidget *parent = 0 );

        const Account *account() const;
        void setAccount( const Account *account );

        const Category *selectedCategory() const;
        Category *selectedCategory();
        void setSelectedCategory( const Category *category );

        bool eventFilter( QObject *object, QEvent *event );

        void showPopup();
        void hidePopup();

    private:
        void addCategory( QStandardItem *parent, const Category *category );
        QIcon renderCategoryIcon( const Category *category );

    private:
        QTreeView *m_view;
        QStandardItemModel *m_model;
        QSet<QStandardItem *> m_items;
        const Account *m_account;
        bool m_skipNextHide;
        int m_selectedCategory;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
