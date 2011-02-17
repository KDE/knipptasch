/*
 * Copyright 2008-2011  Stefan Böhmann <kde@hilefoks.org>
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
#include "accountsortfilterproxymodel.h"

#include "backend/money.h"
#include "backend/account.h"
#include "backend/posting.h"

#include "preferences.h"

#include <QDate>
#include <QTimer>
#include <QColor>


#define TEST_LESS_THAN_RESULT_TRUE 1
#define TEST_LESS_THAN_RESULT_FALSE 0
#define TEST_LESS_THAN_RESULT_UNKNOWN -1



AccountSortFilterProxyModel::AccountSortFilterProxyModel(QObject *parent)
  : QSortFilterProxyModel( parent )
{
    connect( this, SIGNAL( modelReset() ), this, SLOT( updateCache() ) );

    connect( this, SIGNAL( rowsInserted(const QModelIndex &, int, int) ),
             this, SLOT( updateCache() ) );
    connect( this, SIGNAL( rowsRemoved(const QModelIndex &, int, int) ),
             this, SLOT( updateCache() ) );

    QTimer::singleShot( 0, this, SLOT( updateCache() ) );
}


Account* AccountSortFilterProxyModel::account()
{
    AccountModel *model = qobject_cast<AccountModel*>( sourceModel() );
    Q_ASSERT( model );

    return model->account();
}


const Account* AccountSortFilterProxyModel::account() const
{
    const AccountModel *model = qobject_cast<const AccountModel*>( sourceModel() );
    Q_ASSERT( model );

    return model->account();
}


QVariant AccountSortFilterProxyModel::data(const QModelIndex &idx, int role) const
{
    if( !idx.isValid() ) {
        return QSortFilterProxyModel::data( idx, role );
    }

    if( mapToSource( idx ).column() == AccountModel::BALANCE ) {
        if( role == Qt::TextAlignmentRole ) {
            return static_cast<int>( Qt::AlignRight | Qt::AlignVCenter );
        }
        else if( role == Qt::ForegroundRole ) {
            if( !m_cache.contains( idx.row() ) ) { // Value not cached
                return QVariant();
            }

            const AccountModel *model = qobject_cast<const AccountModel*>( sourceModel() );
            Q_ASSERT( model );

            Money m = data( idx, Qt::EditRole ).value<Money>();
            if( m >= 0.0 && Knipptasch::Preferences::self()->positiveAmountForegroundEnabled() && Knipptasch::Preferences::self()->positiveAmountForegroundColor().isValid() ) {
                return Knipptasch::Preferences::self()->positiveAmountForegroundColor();
            }
            else if( m < 0.0 && Knipptasch::Preferences::self()->negativeAmountForegroundEnabled() && Knipptasch::Preferences::self()->negativeAmountForegroundColor().isValid() ) {
                return Knipptasch::Preferences::self()->negativeAmountForegroundColor();
            }
        }
        else if( role == Qt::EditRole ) {
            // Although this column is not editable, this value is still needed (!
            return QVariant::fromValue( m_cache.value( idx.row() ) );
        }
        else if( role == Qt::DisplayRole ) {
            return m_cache.contains( idx.row() )
                      ? m_cache.value( idx.row() ).toString()
                      : "-";
        }
    }

    return QSortFilterProxyModel::data( idx, role );
}


bool AccountSortFilterProxyModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    if( !QSortFilterProxyModel::setData( idx, value, role ) ) {
        return false;
    }

    if( mapToSource( idx ).column() == AccountModel::AMOUNT ) {
        updateCache( idx.row() );
    }

    return true;
}


void AccountSortFilterProxyModel::updateCache(int firstRow)
{
    const int amountColumnView = mapFromSource( sourceModel()->index( 0, AccountModel::AMOUNT ) ).column();
    const int balanceColumnView = mapFromSource( sourceModel()->index( 0, AccountModel::BALANCE ) ).column();

    if( firstRow <= 0 ) {
        m_cache.clear();
    }

    for(int viewIndex = qMax( firstRow, 0 ); viewIndex < rowCount(); ++viewIndex) {
        Money money = data( index( viewIndex, amountColumnView ), Qt::EditRole ).value<Money>();

        if( viewIndex <= 0 ) {
            money += account()->openingBalance();
        }
        else {
            money += data( index( viewIndex - 1, balanceColumnView ), Qt::EditRole ).value<Money>();
        }

        m_cache.insert( viewIndex, money );
    }
}


bool AccountSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    int result = lessThanByType( left, right );

    if( result > TEST_LESS_THAN_RESULT_UNKNOWN ) {
        return result == TEST_LESS_THAN_RESULT_TRUE ? true : false;
    }

    result = lessThanDateBased( left, right );
    if( result > TEST_LESS_THAN_RESULT_UNKNOWN ) {
        return result == TEST_LESS_THAN_RESULT_TRUE ? true : false;
    }

    QString lstr = sourceModel()->data(
        createIndex( left.row(), AccountModel::POSTINGTEXT ),
        Qt::EditRole
    ).toString().trimmed();

    QString rstr = sourceModel()->data(
        createIndex( right.row(), AccountModel::POSTINGTEXT ),
        Qt::EditRole
    ).toString().trimmed();

    Money lamount = sourceModel()->data(
        createIndex( left.row(), AccountModel::AMOUNT ),
        Qt::EditRole
    ).value<Money>();

    Money ramount = sourceModel()->data(
        createIndex( right.row(), AccountModel::AMOUNT ),
        Qt::EditRole
    ).value<Money>();

    if( !( lstr.isEmpty() && rstr.isEmpty() ) ) {
        return lstr < rstr;
    }
    else if( lamount != ramount ) {
        return lamount < ramount;
    }

    return left.row() < right.row();
}


int AccountSortFilterProxyModel::lessThanByType(const QModelIndex &left, const QModelIndex &right) const
{
    const AccountModel *model = qobject_cast<const AccountModel*>( sourceModel() );
    Q_ASSERT( model );

    int l_type = model->postingType( left.row() );
    int r_type = model->postingType( right.row() );

    if( l_type < r_type ) {
        return TEST_LESS_THAN_RESULT_TRUE;
    }
    else if( l_type > r_type ) {
        return TEST_LESS_THAN_RESULT_FALSE;
    }

    return TEST_LESS_THAN_RESULT_UNKNOWN;
}


int AccountSortFilterProxyModel::lessThanDateBased(const QModelIndex &left, const QModelIndex &right) const
{
    const QDate l_maturity = sourceModel()->data(
                    createIndex( left.row(), AccountModel::MATURITY ),
                    Qt::EditRole ).value<QDate>();

    const QDate r_maturity = sourceModel()->data(
                    createIndex( right.row(), AccountModel::MATURITY ),
                    Qt::EditRole ).value<QDate>();

    const QDate l_valuedate = sourceModel()->data(
                    createIndex( left.row(), AccountModel::VALUEDATE ),
                    Qt::EditRole ).value<QDate>();

    const QDate r_valuedate = sourceModel()->data(
                    createIndex( right.row(), AccountModel::VALUEDATE ),
                    Qt::EditRole ).value<QDate>();

    QDate l_primary, r_primary, l_secondary, r_secondary;

    switch( Knipptasch::Preferences::self()->postingSortOrder() ) {
        case AccountSortFilterProxyModel::Maturity:
            l_primary = l_maturity;
            r_primary = r_maturity;
            l_secondary = l_valuedate;
            r_secondary = r_valuedate;
            break;

        case AccountSortFilterProxyModel::ValueDate:
            l_primary = l_valuedate;
            r_primary = r_valuedate;
            l_secondary = l_maturity;
            r_secondary = r_maturity;

        default:
            Q_ASSERT_X( false, Q_FUNC_INFO, "Unhandled 'postingSortOrder' configuration value in AccountSortFilterProxyModel" );
    }

    if( l_primary.isValid() && !r_primary.isValid() ) {
        return TEST_LESS_THAN_RESULT_TRUE;
    }
    else if( !l_primary.isValid() && r_primary.isValid() ) {
        return TEST_LESS_THAN_RESULT_FALSE;
    }
    else if( l_primary.isValid() && r_primary.isValid() ) {
        if( l_primary != r_primary ) {
            return l_primary < r_primary
                        ? TEST_LESS_THAN_RESULT_TRUE
                        : TEST_LESS_THAN_RESULT_FALSE;
        }
    }

    Q_ASSERT( ( l_primary.isValid() && l_primary == r_primary )
                || ( !l_primary.isValid() && !r_primary.isValid() ) );


    if( l_secondary.isValid() && !r_secondary.isValid() ) {
        return TEST_LESS_THAN_RESULT_TRUE;
    }
    else if( !l_secondary.isValid() && r_secondary.isValid() ) {
        return TEST_LESS_THAN_RESULT_FALSE;
    }
    else if( l_secondary.isValid() && r_secondary.isValid() ) {
        if( l_secondary != r_secondary ) {
            return l_secondary < r_secondary
                        ? TEST_LESS_THAN_RESULT_TRUE
                        : TEST_LESS_THAN_RESULT_FALSE;
        }
    }
    Q_ASSERT( ( l_secondary.isValid() && l_secondary == r_secondary )
                || ( !l_secondary.isValid() && !r_secondary.isValid() ) );

    return TEST_LESS_THAN_RESULT_UNKNOWN;
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
