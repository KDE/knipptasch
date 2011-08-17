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
#include "accounttablewidget.h"

#include "preferences.h"
#include "accountmodel.h"
#include "accountsortfilterproxymodel.h"

#include "backend/account.h"

#include "delegate/datedelegate.h"
#include "delegate/postingtextdelegate.h"
#include "delegate/moneydelegate.h"
#include "delegate/categorydelegate.h"

#include <QtGui/QTableView>
#include <QtGui/QHeaderView>

#include <QtGui/QVBoxLayout>

#include <modeltest/modeltest.h>
#include <delegate/datedelegate.h>


namespace Knipptasch
{


    struct AccountTableWidget::Private {
        Private( QWidget *parent )
            : model( new AccountModel( parent ) ),
              proxy( new AccountSortFilterProxyModel( parent ) ),
              view( new QTableView( parent ) ) {
            new ModelTest( model, parent );
            new ModelTest( proxy, parent );
        }


        AccountModel *model;
        AccountSortFilterProxyModel *proxy;
        QTableView *view;
    };



    AccountTableWidget::AccountTableWidget( QWidget *parent )
        : QWidget( parent ),
          d( new AccountTableWidget::Private( this ) )
    {
        d->proxy->setSourceModel( d->model );
        d->proxy->sort( AccountModel::MATURITY, Qt::AscendingOrder );
        d->proxy->setDynamicSortFilter( true );
        d->proxy->setFilterKeyColumn( -1 );
        d->proxy->setFilterCaseSensitivity( Qt::CaseInsensitive );

        d->view->setModel( d->proxy );

        d->view->setItemDelegateForColumn( AccountModel::MATURITY, new DateDelegate( this ) );
        d->view->setItemDelegateForColumn( AccountModel::VALUEDATE, new DateDelegate( this ) );
        d->view->setItemDelegateForColumn( AccountModel::WARRANTY, new DateDelegate( this ) );
        d->view->setItemDelegateForColumn( AccountModel::POSTINGTEXT, new PostingTextDelegate( this ) );
        d->view->setItemDelegateForColumn( AccountModel::AMOUNT, new MoneyDelegate( this ) );
        d->view->setItemDelegateForColumn( AccountModel::BALANCE, new MoneyDelegate( this ) );
        d->view->setItemDelegateForColumn( AccountModel::CATEGORY, new CategoryDelegate( this ) );

        d->view->resizeColumnsToContents();
        d->view->verticalHeader()->hide();
        d->view->setSelectionBehavior( QAbstractItemView::SelectRows );
        d->view->horizontalHeader()->setResizeMode( AccountModel::POSTINGTEXT, QHeaderView::Stretch );

        connect( d->view->selectionModel(), SIGNAL( currentRowChanged( QModelIndex, QModelIndex ) ), this, SLOT( slotCurrentRowChanged() ) );

        QVBoxLayout *layout = new QVBoxLayout;
        layout->setContentsMargins( 0, 0, 0, 0 );
        layout->addWidget( d->view );
        setLayout( layout );
    }



    AccountTableWidget::~AccountTableWidget()
    {
        delete d;
    }


    bool AccountTableWidget::isModified() const
    {
        if( d->model && d->model->account() ) {
            return d->model->account()->isModified();
        }

        return false;
    }


    Account *AccountTableWidget::account()
    {
        return d->model->account();
    }


    const Account *AccountTableWidget::account() const
    {
        return d->model->account();
    }


    void AccountTableWidget::setAccount( Account *acc )
    {
        d->model->setAccount( acc );
    }


    QAbstractItemView *AccountTableWidget::view()
    {
        return d->view;
    }


    const QAbstractItemView *AccountTableWidget::view() const
    {
        return d->view;
    }


    AccountModel *AccountTableWidget::model()
    {
        return d->model;
    }


    const AccountModel *AccountTableWidget::model() const
    {
        return d->model;
    }


    AccountSortFilterProxyModel *AccountTableWidget::proxy()
    {
        return d->proxy;
    }


    const AccountSortFilterProxyModel *AccountTableWidget::proxy() const
    {
        return d->proxy;
    }


    void AccountTableWidget::slotCurrentRowChanged()
    {
        if( Knipptasch::Preferences::self()->resetCurrentIndexWhenCurrentRowChanged() ) {
            d->view->setCurrentIndex( d->view->model()->index( d->view->currentIndex().row(), 0 ) );
        }
    }


} // EndNamspace Knipptasch


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
