/*
 * Copyright 2011  Stefan Böhmann <kde@hilefoks.org>
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
#include "categorymodel.h"

#include "backend/account.h"
#include "backend/category.h"

#include <qmimedata.h>


CategoryModel::CategoryModel( QObject *parent )
    : QAbstractItemModel( parent ),
      m_account( 0 )
{
}



CategoryModel::CategoryModel( Account *account, QObject *parent )
    : QAbstractItemModel( parent ),
      m_account( 0 )
{
    setAccount( account );
}


Account *CategoryModel::account()
{
    return m_account;
}


const Account *CategoryModel::account() const
{
    return m_account;
}


void CategoryModel::setAccount( Account *account )
{
#if QT_VERSION >= 0x040600
    beginResetModel();
#endif

    m_account = account;

#if QT_VERSION >= 0x040600
    endResetModel();
#else
    reset();
#endif
}


const Category *CategoryModel::category( const QModelIndex &index ) const
{
    if( !index.isValid() ) {
        return 0;
    }

    return static_cast<Category *>( index.internalPointer() );
}


Category *CategoryModel::category( const QModelIndex &index )
{
    if( !index.isValid() ) {
        return m_account->rootCategory();
    }

    return static_cast<Category *>( index.internalPointer() );
}


QModelIndex CategoryModel::categoryIndex( const Category *category ) const
{
    if( !category ) {
        return QModelIndex();
    }

    for( int i = 0; i < rowCount(); ++i ) {
        QModelIndex idx = findCategoryIndex( index( i, 0 ), category );
        if( idx.isValid() ) {
            return idx;
        }
    }

    return QModelIndex();
}


int CategoryModel::rowCount( const QModelIndex &parent ) const
{
    if( !m_account ) {
        return 0;
    }

    if( parent.column() > 0 ) {
        return 0;
    }

    Category *parentCategory = 0;
    if( !parent.isValid() ) {
        parentCategory = m_account->rootCategory();
    } else {
        parentCategory = static_cast<Category *>( parent.internalPointer() );
    }

    return parentCategory->countCategories();
}


int CategoryModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );

    if( !m_account ) {
        return 0;
    }

    return CategoryModel::ENTRYCOUNT;
}


QVariant CategoryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch( section ) {
            case CATEGORY_NAME:
                return tr( "Name" );

            case CATEGORY_COLOR:
                return tr( "Color" );

            case CATEGORY_USAGE:
                //return tr( "Usage Count" );
                return QString();

            default:
                break;
        }
    }

    return QAbstractItemModel::headerData( section, orientation, role );
}


QVariant CategoryModel::data( const QModelIndex &index, int role ) const
{
    if( !m_account ) {
        return QVariant();
    }

    if( !index.isValid() ) {
        return QVariant();
    }

    if( !( role == Qt::EditRole || role == Qt::DisplayRole ) ) {
        return QVariant();
    }

    Category *category = static_cast<Category *>( index.internalPointer() );
    Q_ASSERT( category );

    switch( index.column() ) {
        case CATEGORY_NAME:
            return category->name();

        case CATEGORY_COLOR:
            if( role == Qt::EditRole ) {
                return QVariant::fromValue<QColor>( category->color() );
            }
            break;

        default:
            break;
    }

    return QVariant();
}


bool CategoryModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( !m_account ) {
        return false;
    }

    if( !index.isValid() ) {
        return false;
    }

    if( !( role == Qt::EditRole || role == Qt::DisplayRole ) ) {
        return false;
    }

    Category *category = static_cast<Category *>( index.internalPointer() );
    Q_ASSERT( category );

    switch( index.column() ) {
        case CATEGORY_NAME: {
                QString str = value.toString().trimmed();
                category->setName( str.isEmpty() ? tr( "Unnamed Category" ) : str );
                break;
            }

        case CATEGORY_COLOR:
            category->setColor( value.value<QColor>() );
            break;

        default:
            return false;
    }

    emit dataChanged( index, index );
    return true;
}


bool CategoryModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    Q_UNUSED( position );

    Category *parentCategory = 0;
    if( !parent.isValid() ) {
        parentCategory = m_account->rootCategory();
    } else {
        parentCategory = static_cast<Category *>( parent.internalPointer() );
    }
    Q_ASSERT( parentCategory );

    beginInsertRows( parent, parentCategory->countCategories(),
                     parentCategory->countCategories() + rows - 1 );

    for( int row = 0; row < rows; row++ ) {
        parentCategory->addCategory( new Category( tr( "Unnamed Category" ), m_account ) );
    }

    endInsertRows();

    return true;
}


bool CategoryModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    Category *parentCategory = 0;
    if( !parent.isValid() ) {
        parentCategory = m_account->rootCategory();
    } else {
        parentCategory = static_cast<Category *>( parent.internalPointer() );
    }
    Q_ASSERT( parentCategory );
    Q_ASSERT(( position + rows - 1 ) < parentCategory->countCategories() );

    beginRemoveRows( parent, position, position + rows - 1 );

    for( int row = 0; row < rows; ++row ) {
        delete parentCategory->takeCategory( position );
    }

    endRemoveRows();

    return true;
}


Qt::ItemFlags CategoryModel::flags( const QModelIndex &index ) const
{
    if( !m_account ) {
        return 0;
    }

    if( !index.isValid() ) {
        return QAbstractItemModel::flags( index )
               /* | Qt::ItemIsDropEnabled */;  //<< TODO implement Drag & Drop
    }

    return QAbstractItemModel::flags( index )
           | Qt::ItemIsEditable
           | Qt::ItemIsDragEnabled
           /* | Qt::ItemIsDropEnabled */;  //<< TODO implement Drag & Drop
}


Qt::DropActions CategoryModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}


QStringList CategoryModel::mimeTypes() const
{
    QStringList list( QAbstractItemModel::mimeTypes() );

    //TODO implement Drag & Drop

    return list;
}


QMimeData *CategoryModel::mimeData( const QModelIndexList &indexes ) const
{
    QMimeData *mimeData = QAbstractItemModel::mimeData( indexes );

    //TODO implement Drag & Drop

    return mimeData;
}


bool CategoryModel::dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent )
{
    //TODO implement Drag & Drop
    return QAbstractItemModel::dropMimeData( data, action, row, column, parent );
}


QModelIndex CategoryModel::index( int row, int column, const QModelIndex &parent ) const
{
    if( !m_account ) {
        return QModelIndex();
    }

    if( !hasIndex( row, column, parent ) ) {
        return QModelIndex();
    }

    Category *parentCategory;

    if( !parent.isValid() ) {
        parentCategory =  m_account->rootCategory();
    } else {
        parentCategory = static_cast<Category *>( parent.internalPointer() );
    }


    if( row >= 0 && row < parentCategory->countCategories() ) {
        Category *childCategory = parentCategory->category( row );
        Q_ASSERT( childCategory );

        return createIndex( row, column, childCategory );
    } else {
        return QModelIndex();
    }
}


QModelIndex CategoryModel::parent( const QModelIndex &index ) const
{
    if( !m_account ) {
        return QModelIndex();
    }

    if( !index.isValid() ) {
        return QModelIndex();
    }

    Category *childCategory = static_cast<Category *>( index.internalPointer() );
    Category *parentCategory = m_account->findCategoryParent( childCategory );

    if( parentCategory == m_account->rootCategory() ) {
        return QModelIndex();
    }

    Category *grandparents = m_account->findCategoryParent( parentCategory );
    Q_ASSERT( grandparents );

    int row = 0;
    for( int i = 0; i < grandparents->countCategories(); ++i ) {
        if( grandparents->category( i ) == parentCategory ) {
            row = i;
            break;
        }
    }

    return createIndex( row, 0, parentCategory );
}


QModelIndex CategoryModel::findCategoryIndex( const QModelIndex &parent, const Category *category ) const
{
    if( !parent.isValid() ) {
        return QModelIndex();
    }

    if( static_cast<Category *>( parent.internalPointer() ) == category ) {
        return parent;
    }

    for( int i = 0; i < rowCount( parent ); ++i ) {
        QModelIndex idx = findCategoryIndex( parent.child( i, parent.column() ), category );
        if( idx.isValid() ) {
            return idx;
        }
    }

    return QModelIndex();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
