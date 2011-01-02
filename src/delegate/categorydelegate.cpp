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
#include "categorydelegate.h"
#include "categorycombobox.h"

#include "accountsortfilterproxymodel.h"
#include "backend/posting.h"
#include "backend/category.h"

#include <QPainter>
#include <QApplication>
#include <QStandardItemModel>



CategoryDelegate::CategoryDelegate(QObject *parent)
  : QStyledItemDelegate( parent )
{
}


QWidget* CategoryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const AccountSortFilterProxyModel *model =
            qobject_cast<const AccountSortFilterProxyModel*>( index.model() );

    if( !model ) {
        return QStyledItemDelegate::createEditor( parent, option, index );
    }

    CategoryComboBox *input = new CategoryComboBox( model->account(), parent );
    input->setFrame( false );

    return input;
}


void CategoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    CategoryComboBox *input = qobject_cast<CategoryComboBox*>( editor );

    const AccountSortFilterProxyModel *model =
            qobject_cast<const AccountSortFilterProxyModel*>( index.model() );

    if( !input || !model ) {
        QStyledItemDelegate::setEditorData( editor, index );
    }
    else {
        bool ok;
        int id = model->data( index, Qt::EditRole ).toInt( &ok );

        Category *category = 0;

        if( ok ) {
            Object *object = model->account()->objectByIdentifier( id );
            if( object ) {
                category = qobject_cast<Category*>( object );
                Q_ASSERT( category );
            }
        }

        input->setSelectedCategory( category );
    }
}


void CategoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *m, const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return;
    }

    CategoryComboBox *input = qobject_cast<CategoryComboBox*>( editor );

    AccountSortFilterProxyModel *model =
            qobject_cast<AccountSortFilterProxyModel*>( m );

    if( !input || !model ) {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
    else {
        int id = model->account()->identifierByObject( input->selectedCategory() );
        model->setData( index, id, Qt::EditRole );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
