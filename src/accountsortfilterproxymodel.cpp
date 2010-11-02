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
#include "accountsortfilterproxymodel.h"

#include "preferences.h"
#include "backend/money.h"
#include "backend/posting.h"

#include <QDate>
#include <QDebug>

#define TEST_LESS_THAN_RESULT_TRUE 1
#define TEST_LESS_THAN_RESULT_FALSE 0
#define TEST_LESS_THAN_RESULT_UNKNOWN -1



AccountSortFilterProxyModel::AccountSortFilterProxyModel(QObject *parent)
  : QSortFilterProxyModel( parent )
{
}


QPair<Money, Money> AccountSortFilterProxyModel::amountInMonth(int year, int month)
{
    AccountModel *model = qobject_cast<AccountModel*>( sourceModel() );
    Q_ASSERT( model );

    if( QDate( year, month, 1 ) < model->account()->openingDate() ) {
        return QPair<Money, Money>( 0, 0 );
    }

    int start = findRowByMonth( year, month, 1, rowCount() - 1 );
    if( start < 0 ) {
        return QPair<Money, Money>( 0, 0 );
    }

    int end = findRowByMonth( year, month, rowCount() - 1, start );
    Q_ASSERT( end >= start );

    Q_ASSERT( model->index( 0, AccountModel::AMOUNT ).isValid() );
    Q_ASSERT( mapFromSource( model->index( 0, AccountModel::AMOUNT ) ).isValid() );
    const int column = mapFromSource( model->index( 0, AccountModel::AMOUNT ) ).column();

    Money incoming, outgoing;
    for(int i = start; i <= end; ++i) {
        const QModelIndex idx = createIndex( i, column );
        Q_ASSERT( idx.isValid() );

        Money money = data( idx, Qt::EditRole ).value<Money>();
        if( money > 0.0 ) {
            incoming += money;
        }
        else {
            outgoing += money.abs();
        }
    }

    return QPair<Money, Money>(incoming, outgoing);
}


QVariant AccountSortFilterProxyModel::data(const QModelIndex &idx, int role) const
{
    if( !idx.isValid() ) {
        return QSortFilterProxyModel::data( idx, role );
    }

    if( mapToSource( idx ).column() == AccountModel::BALANCE ) {
        if( role == Qt::ForegroundRole ) {
            Money m = data( idx, Qt::EditRole ).value<Money>();
            if( m >= 0.0 && Preferences::self()->positiveAmountForegroundEnabled() ) {
                return Preferences::self()->positiveAmountForegroundColor();
            }
            else if( m < 0.0 && Preferences::self()->negativeAmountForegroundEnabled() ) {
                return Preferences::self()->negativeAmountForegroundColor();
            }
        }
        else if( role == Qt::TextAlignmentRole ) {
            return static_cast<int>( Qt::AlignRight | Qt::AlignVCenter );
        }
        else if( role == Qt::EditRole || role == Qt::DisplayRole ) {
            Money money = data(
                                index( idx.row(),
                                    mapFromSource(
                                        sourceModel()->index(
                                            mapToSource( idx ).row(),
                                            AccountModel::AMOUNT
                                        )
                                    ).column()
                                ),
                                Qt::EditRole
                            ).value<Money>();

            if( idx.row() <= 0 ) {
                AccountModel *model = qobject_cast<AccountModel*>( sourceModel() );
                Q_ASSERT( model );

                money += model->account()->openingBalance();
            }
            else {
                //TODO Optimize this recursive function calls
                money += data(
                            index( idx.row() - 1, idx.column() ), Qt::EditRole
                        ).value<Money>();
            }

            if( role == Qt::EditRole ) {
                return QVariant::fromValue( money );
            }

            return money.toString();
        }
    }

    return QSortFilterProxyModel::data( idx, role );
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
    //explicit: Qt::DisplayRole, because this role returns an int
    int l_type = sourceModel()->data(
                        createIndex( left.row(), AccountModel::TYPE ),
                        Qt::DisplayRole
                ).toInt();

    int r_type = sourceModel()->data(
                        createIndex( right.row(), AccountModel::TYPE ),
                        Qt::DisplayRole
                ).toInt();

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

    if( Preferences::self()->value<bool>( "TableView", "SortPostingsByMaturity", true ) ) {
        l_primary = l_maturity;
        r_primary = r_maturity;
        l_secondary = l_valuedate;
        r_secondary = r_valuedate;
    }
    else {
        l_primary = l_valuedate;
        r_primary = r_valuedate;
        l_secondary = l_maturity;
        r_secondary = r_maturity;
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


int AccountSortFilterProxyModel::findRowByMonth(int year, int month, int startRow, int endRow) const
{
    //FIXME
    return -1;
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
