/*
 * Copyright 2007-2010 by Stefan Böhmann <kde@hilefoks.org>
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
#include "accountmodel.h"

#include "preferences.h"

#include "backend/account.h"
#include "backend/posting.h"
#include "backend/money.h"

#include <QVariant>
#include <QBrush>
#include <QStringList>
#include <QDate>
#include <QFontMetrics>
#include <QFlags>

#include <QDebug>
#include <QApplication>

#include <cstdlib>
#include <time.h>



AccountModel::AccountModel(QObject *parent)
  : QAbstractTableModel( parent ),
    m_account( 0 ),
    m_posting( new Posting )
{
}


AccountModel::AccountModel(Account *acc, QObject *parent)
  : QAbstractTableModel( parent ),
    m_account( acc ),
    m_posting( new Posting )
{
}


AccountModel::~AccountModel()
{
    delete m_posting;
    delete m_account;
}


void AccountModel::setAccount(Account *account)
{
    m_account = account;
}


const Posting* AccountModel::posting(const QModelIndex &index) const
{
    if( index.isValid() ) {
        return posting( index.row() );
    }

    return 0;
}


const Posting* AccountModel::posting(int row) const
{
    Q_ASSERT( row >= 0 );

    if( row < account()->countPostings() ) {
        return account()->posting( row );
    }

    return 0;
}


Posting* AccountModel::posting(const QModelIndex &index)
{
    if( index.isValid() ) {
        return posting( index.row() );
    }

    return 0;
}


Posting* AccountModel::posting(int row)
{
    Q_ASSERT( row >= 0 );

    if( row < account()->countPostings() ) {
        return account()->posting( row );
    }

    return 0;
}


int AccountModel::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() ) {
        return 0;
    }

    if( !m_account ) {
        return 0;
    }

    return m_account->countPostings() + 1;
}


int AccountModel::columnCount(const QModelIndex &parent) const
{
    if( parent.isValid() ) {
        return 0;
    }

    return AccountModel::ENTRYCOUNT;
}


QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() ) {
        return QVariant();
    }

    if( !m_account ) {
        return QVariant();
    }

    Q_ASSERT( index.row() >= 0 );
    Q_ASSERT( index.row() <= m_account->countPostings() );

    switch( role ) {
        case Qt::TextAlignmentRole:
            return textAlignmentRoleData( index );

        case Qt::BackgroundRole:
            return backgroundRoleData( index );

        case Qt::ForegroundRole:
            return foregroundRoleData(index );

        case Qt::EditRole:
        case Qt::DisplayRole:
            break;

        default: // Can't handle this role...
            return QVariant();
    }


    Posting *entry = 0;
    if( index.row() < m_account->countPostings() ) {
        entry = m_account->posting( index.row() );
    }
    else {
        entry = m_posting;
    }

    Q_ASSERT( entry );

    switch( index.column() )
    {
        case TYPE:
        {
            PostingTypeFlags flags = postingType( entry );
            if( role == Qt::EditRole ) {
                return QVariant::fromValue<PostingTypeFlags>( flags );
            }

            return static_cast<int>( flags );
        }

        case MATURITY:
            return entry->maturity();

        case POSTINGTEXT:
            return entry->postingText();

        case AMOUNT:
            if( role == Qt::EditRole ) {
                return QVariant::fromValue( entry->amount() );
            }
            else if( entry->amount().cents() != 0 ) {
                return entry->amount().toString();
            }
            return QVariant();

        case BALANCE:
            break;

        case VALUEDATE:
            return entry->valueDate();

        case CATEGORY:
            return entry->category();

        case PAYEE:
            return entry->payee();

        case STATEMENT:
            if( role == Qt::EditRole || entry->page() > 0 ) {
                return entry->page();
            }
            break;

        case VOUCHER:
            entry->voucher();

        case WARRANTY:
            return entry->warranty();

        case PAYMENT:
            return entry->methodOfPayment();

        case DESCRIPTION:
            return entry->description();
    }

    return QVariant();
}


QVariant AccountModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole && section < AccountModel::ENTRYCOUNT ) {
        switch( section ) {
            case TYPE:
                return tr( "Type" );

            case MATURITY:
                return tr( "Maturity" );

            case POSTINGTEXT:
                return tr( "Posting Text" );

            case AMOUNT:
                return tr( "Amount" );

            case BALANCE:
                return tr( "Balance" );

            case CATEGORY:
                return tr( "Category" );

            case PAYEE:
                return tr( "Payee" );

            case STATEMENT:
                return tr( "Statement" );

            case VOUCHER:
                return tr( "Voucher" );

            case WARRANTY:
                return tr( "Warranty" );

            case PAYMENT:
                return tr( "Method of payment" );

            case VALUEDATE:
                return tr( "Value Date" );

            case DESCRIPTION:
                return tr( "Description" );

            default:
                break;
        }
    }

    return QAbstractTableModel::headerData( section, orientation, role );
}


bool AccountModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() ) {
        return false;
    }

    if( !m_account ) {
        return false;
    }

    if( !(role == Qt::EditRole || role == Qt::DisplayRole) ) {
        return false;
    }

    if( index.column() == TYPE || index.column() == BALANCE ) {
        return false;
    }

    Posting *entry = 0;
    if( index.row() < m_account->countPostings() ) {
        entry = m_account->posting( index.row() );
    }
    else {
        Q_ASSERT( index.row() == m_account->countPostings() );
        entry = m_posting;
    }

    Q_ASSERT( entry );

    switch( index.column() ) {
        case TYPE:
            return false;

        case MATURITY:
            entry->setMaturity( value.toDate() );
            break;

        case POSTINGTEXT:
            entry->setPostingText( value.toString().trimmed() );
            break;

        case AMOUNT:
            {
                bool ok;
                double amount = value.toDouble( &ok );
                if( !ok ) {
                    qDebug() << "invalid amount value";
                }
                else {
                    entry->setAmount( amount );
                }
            }
            break;

        case BALANCE:
            return false;

        case VALUEDATE:
            entry->setValueDate( value.toDate() );
            break;

        case CATEGORY:
            entry->setCategory( value.toString() );
            break;

        case PAYEE:
            entry->setPayee( value.toString() );
            break;

        case STATEMENT:
            entry->setPage( value.toInt() );
            break;

        case VOUCHER:
            entry->setVoucher( value.toString() );
            break;

        case DESCRIPTION:
            entry->setDescription( value.toString() );
            break;
    }

    if( index.row() >= m_account->countPostings() ) {
        if( postingIsValid( m_posting ) ) {
            beginInsertRows( QModelIndex(), rowCount(), rowCount() );
            m_account->addPosting( entry );
            endInsertRows();

            m_posting = new Posting;
            emit dataChanged( createIndex( rowCount()-1, 0 ), createIndex( rowCount(), columnCount() ) );
        }
    }
    else {
        emit dataChanged( createIndex( index.row(), 0 ), createIndex( index.row(), columnCount() ) );
    }

    return true;
}


Qt::ItemFlags AccountModel::flags(const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return 0;
    }

    if( !m_account ) {
        return 0;
    }

    if( index.column() == TYPE || index.column() == BALANCE ) {
        return QAbstractItemModel::flags( index );
    }

    return QAbstractItemModel::flags( index ) | Qt::ItemIsEditable;
}


bool AccountModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if( !m_account ) {
        return false;
    }

    bool re = false;

    if( ( row + count - 1 ) == m_account->countPostings() ) {
        count -= 1;
        m_posting = new Posting;
        re = true;
    }

    if( ( row + count - 1 ) < m_account->countPostings() ) {
        beginRemoveRows( parent, row, row + count - 1 );

        for(int i = 0; i < count; ++i) {
            m_account->removePosting( row );
        }

        endRemoveRows();
        re = true;
    }

    return re;
}


AccountModel::PostingTypeFlags AccountModel::postingType(const Posting *ptr) const
{
    Q_ASSERT( m_posting );
    Q_ASSERT( ptr );

    PostingTypeFlags type = 0;

    if( ptr == m_posting ) {
        type |= Current;
    }

    if( !ptr->valueDate().isValid() ) {
        type |= Incomplete;
    }

    if( !postingIsValid( ptr ) ) {
        type |= Invalid;
    }

    if( ptr->maturity().isValid() && ptr->maturity() > QDate::currentDate() ) {
        type |= Future;
    }

    return type;
}


bool AccountModel::postingIsValid(const Posting *ptr)
{
    Q_ASSERT( ptr );

    if( !ptr->maturity().isValid() ) {
        return false;
    }

    if( ptr->amount() == 0.0 ) {
        return false;
    }

    if( ptr->postingText().isEmpty() ) {
        return false;
    }

    return true;
}


QVariant AccountModel::backgroundRoleData(const QModelIndex &index) const
{
    AccountModel::PostingTypeFlags type =
       data( createIndex( index.row(), AccountModel::TYPE ), Qt::EditRole )
        .value<AccountModel::PostingTypeFlags>();

    if( type & AccountModel::Current && Preferences::self()->currentPostingBackgroundEnabled() ) {
        return Preferences::self()->currentPostingBackgroundColor();
    }
    else if( type & AccountModel::Future && Preferences::self()->futurePostingBackgroundEnabled() ) {
        return Preferences::self()->futurePostingBackgroundColor();
    }
    else if( type & AccountModel::Incomplete && Preferences::self()->incompletePostingBackgroundEnabled() ) {
        return Preferences::self()->incompletePostingBackgroundColor();
    }

    if( Preferences::self()->defaultPostingBackgroundEnabled() ) {
        return Preferences::self()->defaultPostingBackgroundColor();
    }

    return QVariant();
}


QVariant AccountModel::foregroundRoleData(const QModelIndex &index) const
{
    switch( index.column() ) {
        case AccountModel::AMOUNT:
        {
            Money m = data( index, Qt::EditRole ).value<Money>();
            if( m >= 0.0 && Preferences::self()->positiveAmountForegroundEnabled() ) {
                return Preferences::self()->positiveAmountForegroundColor();
            }
            else if( m < 0.0 && Preferences::self()->negativeAmountForegroundEnabled() ) {
                return Preferences::self()->negativeAmountForegroundColor();
            }
        }
        break;

        case AccountModel::WARRANTY:
        {
            QDate d = data( index, Qt::EditRole ).value<QDate>();
            if( d < QDate::currentDate() && Preferences::self()->availableWarrantyForegroundEnabled() ) {
                return Preferences::self()->availableWarrantyForegroundColor();
            }
            else if( Preferences::self()->expiredWarrantyForegroundEnabled() ) {
                return Preferences::self()->expiredWarrantyForegroundColor();
            }
        }
        break;

        default:
            break;
    }

    return QVariant();
}


QVariant AccountModel::textAlignmentRoleData(const QModelIndex &index) const
{
    switch( index.column() ) {
        case AccountModel::VALUEDATE:
        case AccountModel::MATURITY:
        case AccountModel::WARRANTY:
        case AccountModel::AMOUNT:
            return static_cast<int>( Qt::AlignRight | Qt::AlignVCenter );
        default:
            break;
    }

    return QVariant();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
