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
#include "categorycombobox.h"

#include "backend/account.h"
#include "backend/category.h"

#include <QMouseEvent>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>



CategoryComboBox::CategoryComboBox(const Account *account, QWidget* parent)
  : KComboBox( parent ),
    m_view( new QTreeView( this ) ),
    m_account( 0 ),
    m_skipNextHide( false )
{
    m_view->header()->hide();
    m_view->setMinimumSize( 200, 250 );
    setView( m_view );

    QStandardItemModel *m = new QStandardItemModel;
    setModel( m );

    view()->viewport()->installEventFilter( this );

    setAccount( account );
}


const Account* CategoryComboBox::account() const
{
    return m_account;
}


void CategoryComboBox::setAccount(const Account *account)
{
    m_account = account;

    QStandardItemModel *m = qobject_cast<QStandardItemModel*>( model() );
    Q_ASSERT( m );

    m->clear();

    if( !m_account ) {
        return;
    }

    Q_ASSERT( m_account->rootCategory() );
    for(int i = 0; i < m_account->rootCategory()->countCategories(); ++i) {
        const Category *c = m_account->rootCategory()->category( i );

        QStandardItem *item = new QStandardItem( c->name() );
        item->setData( m_account->identifierByObject( c ), Qt::UserRole );

        for(int i = 0; i < c->countCategories(); ++i) {
            addCategory( item, c->category( i ) );
        }

        m->appendRow( item );
    }
}


const Category* CategoryComboBox::selectedCategory() const
{
    if( currentIndex() >= 0 ) {
        bool ok;
        int id = itemData( currentIndex(), Qt::UserRole ).toInt( &ok );
        Q_ASSERT( ok );

        Object *object = account()->objectByIdentifier( id );
        if( object ) {
            Category *category = qobject_cast<Category*>( object );
            Q_ASSERT( category );

            return category;
        }
    }

    return 0;
}


Category* CategoryComboBox::selectedCategory()
{
    if( currentIndex() >= 0 ) {
        bool ok;
        int id = itemData( currentIndex(), Qt::UserRole ).toInt( &ok );
        Q_ASSERT( ok );

        Object *object = account()->objectByIdentifier( id );
        if( object ) {
            Category *category = qobject_cast<Category*>( object );
            Q_ASSERT( category );

            return category;
        }
    }

    return 0;
}


void CategoryComboBox::setSelectedCategory(const Category *category)
{
    int id = account()->identifierByObject( category );

    int index = findData( id, Qt::UserRole );
    if( index >= 0 ) {
        setCurrentIndex( index );
    }
}


bool CategoryComboBox::eventFilter(QObject* object, QEvent* event)
{
    if( event->type() == QEvent::MouseButtonPress && object == view()->viewport() ) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>( event );

        QModelIndex index = view()->indexAt( mouseEvent->pos() );
        if( !view()->visualRect( index ).contains( mouseEvent->pos() ) ) {
            m_skipNextHide = true;
        }
    }

    return false;
}


void CategoryComboBox::showPopup()
{
    setRootModelIndex( QModelIndex() );
    QComboBox::showPopup();
}


void CategoryComboBox::hidePopup()
{
    //setRootModelIndex(view()->currentIndex().parent());
    //setCurrentIndex(view()->currentIndex().row());
    if( m_skipNextHide ) {
        m_skipNextHide = false;
    }
    else {
        QComboBox::hidePopup();
    }
}


void CategoryComboBox::addCategory(QStandardItem *parent, const Category *category)
{
    Q_ASSERT( parent );
    Q_ASSERT( category );

    QStandardItem *item = new QStandardItem( category->name() );
    item->setData( account()->identifierByObject( category ) );

    parent->appendRow( item );

    for(int i = 0; i < category->countCategories(); ++i) {
        addCategory( item, category->category( i ) );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
