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
#include "categorycombobox.h"

#include "backend/account.h"
#include "backend/category.h"

#include <QMouseEvent>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLineEdit>
#include <QPainter>



CategoryComboBox::CategoryComboBox( const Account *account, QWidget *parent )
    : KComboBox( parent ),
      m_view( new QTreeView( this ) ),
      m_model( new QStandardItemModel( this ) ),
      m_account( 0 ),
      m_skipNextHide( false ),
      m_selectedCategory( -1 )
{
    m_view->header()->hide();
    m_view->setRootIsDecorated( false );
    m_view->setAnimated( true );
    m_view->setUniformRowHeights( true );
    m_view->setEditTriggers( QAbstractItemView::NoEditTriggers );
    m_view->setSelectionBehavior( QAbstractItemView::SelectRows );

    setView( m_view );
    setModel( m_model );

    view()->viewport()->installEventFilter( this );

    setAccount( account );
}


const Account *CategoryComboBox::account() const
{
    return m_account;
}


void CategoryComboBox::setAccount( const Account *account )
{
    Q_ASSERT( m_model );

    m_selectedCategory = -1;
    m_account = account;

    m_items.clear();
    m_model->clear();

    if( !m_account ) {
        return;
    }

    Q_ASSERT( m_account->rootCategory() );

    QStandardItem *item = new QStandardItem( tr( "None" ) );
    item->setData( -1, Qt::UserRole );

    for( int i = 0; i < m_account->rootCategory()->countCategories(); ++i ) {
        addCategory( item, m_account->rootCategory()->category( i ) );
    }

    m_model->appendRow( item );
    m_items.insert( item );

    m_view->expandToDepth( 3 );
}


const Category *CategoryComboBox::selectedCategory() const
{
    Object *object = account()->objectByIdentifier( m_selectedCategory );
    if( object ) {
        Category *category = qobject_cast<Category *>( object );
        Q_ASSERT( category );

        return category;
    }

    return 0;
}


Category *CategoryComboBox::selectedCategory()
{
    Object *object = account()->objectByIdentifier( m_selectedCategory );
    if( object ) {
        Category *category = qobject_cast<Category *>( object );
        Q_ASSERT( category );

        return category;
    }

    return 0;
}


void CategoryComboBox::setSelectedCategory( const Category *category )
{
    m_selectedCategory = account()->identifierByObject( category );
}


bool CategoryComboBox::eventFilter( QObject *object, QEvent *event )
{
    if( event->type() == QEvent::MouseButtonPress ) {
        if( object == view()->viewport() ) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( event );

            QModelIndex index = view()->indexAt( mouseEvent->pos() );
            if( !view()->visualRect( index ).contains( mouseEvent->pos() ) ) {
                m_skipNextHide = true;
            }
        }
    }

    return false;
}


void CategoryComboBox::showPopup()
{
    QModelIndex index;
    if( m_selectedCategory >= 0 ) {
        foreach( QStandardItem * item, m_items ) {
            bool ok;
            int itemid = item->data( Qt::UserRole ).toInt( &ok );
            Q_ASSERT( ok );

            if( itemid == m_selectedCategory ) {
                index = m_model->indexFromItem( item );
                Q_ASSERT( index.isValid() );

                break;
            }
        }
    }

    QComboBox::showPopup();

    m_view->setCurrentIndex( index );
    m_view->scrollTo( index );
}


void CategoryComboBox::hidePopup()
{
    if( m_skipNextHide ) {
        m_skipNextHide = false;
        return;
    }

    if( m_view->currentIndex().isValid() ) {
        bool ok;
        int id = m_model->itemFromIndex( m_view->currentIndex() )
                 ->data( Qt::UserRole ).toInt( &ok );
        Q_ASSERT( ok );

        m_selectedCategory = id;
    }

    QComboBox::hidePopup();
}


void CategoryComboBox::addCategory( QStandardItem *parent, const Category *cat )
{
    Q_ASSERT( parent );
    Q_ASSERT( cat );

    QStandardItem *item = new QStandardItem( renderCategoryIcon( cat ), cat->name() );
    item->setData( account()->identifierByObject( cat ), Qt::UserRole );

    parent->appendRow( item );
    m_items.insert( item );

    for( int i = 0; i < cat->countCategories(); ++i ) {
        addCategory( item, cat->category( i ) );
    }
}


QIcon CategoryComboBox::renderCategoryIcon( const Category *category )
{
    if( category && category->color().isValid() ) {
        QPixmap pix( 128, 128 );

        QPainter painter( &pix );
        painter.setPen( QPen( palette().color( QPalette::Dark ), 10 ) );
        painter.setBrush( category->color() );
        painter.drawRect( pix.rect() );

        return QIcon( pix );
    }

    return QIcon();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
