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

#include "backend/account.h"
#include "backend/posting.h"
#include "backend/category.h"
#include "backend/money.h"

#include "compat/utils.h"

#include "preferences.h"

#include <QVariant>
#include <QBrush>
#include <QStringList>
#include <QDate>
#include <QFontMetrics>
#include <QFlags>
#include <QDebug>



struct AccountModel::Private {
    Private()
        : account( 0 ),
          posting( new Posting ) {
    }


    ~Private() {
        delete posting;
        delete account;
    }


    Account *account;
    Posting *posting;
};



AccountModel::AccountModel( QObject *parent )
    : QAbstractTableModel( parent ),
      d( new AccountModel::Private )
{
}


AccountModel::~AccountModel()
{
    delete d;
}


Account *AccountModel::account()
{
    return d->account;
}


const Account *AccountModel::account() const
{
    return d->account;
}


void AccountModel::setAccount( Account *account )
{
#if QT_VERSION >= 0x040600
    beginResetModel();
#endif

    d->account = account;
    delete d->posting;
    d->posting = new Posting;

#if QT_VERSION >= 0x040600
    endResetModel();
#else
    reset();
#endif
}


const Posting *AccountModel::posting( const QModelIndex &index ) const
{
    if( index.isValid() ) {
        return posting( index.row() );
    }

    return 0;
}


const Posting *AccountModel::posting( int row ) const
{
    Q_ASSERT( row >= 0 );

    if( row < account()->countPostings() ) {
        return account()->posting( row );
    }

    return 0;
}


Posting *AccountModel::posting( const QModelIndex &index )
{
    if( index.isValid() ) {
        return posting( index.row() );
    }

    return 0;
}


Posting *AccountModel::posting( int row )
{
    Q_ASSERT( row >= 0 );

    if( row < account()->countPostings() ) {
        return account()->posting( row );
    }

    return 0;
}


AccountModel::PostingTypeFlags AccountModel::postingType( int row ) const
{
    if( row < d->account->countPostings() ) {
        return postingType( d->account->posting( row ) );
    }

    return postingType( d->posting );
}


int AccountModel::rowCount( const QModelIndex &parent ) const
{
    if( !d->account ) {
        return 0;
    }

    if( parent.isValid() ) {
        return 0;
    }

    return d->account->countPostings() + 1;
}


int AccountModel::columnCount( const QModelIndex &parent ) const
{
    if( parent.isValid() ) {
        return 0;
    }

    return AccountModel::ENTRYCOUNT;
}


QVariant AccountModel::data( const QModelIndex &index, int role ) const
{
    if( !index.isValid() ) {
        return QVariant();
    }

    if( !d->account ) {
        return QVariant();
    }

    Q_ASSERT( index.row() >= 0 );
    Q_ASSERT( index.row() <= d->account->countPostings() );

    switch( role ) {
        case Qt::TextAlignmentRole:
            return textAlignmentRoleData( index );

        case Qt::BackgroundRole:
            return backgroundRoleData( index );

        case Qt::ForegroundRole:
            return foregroundRoleData( index );

        case Qt::DecorationRole:
            return decorationRoleData( index );

        case Qt::EditRole:
        case Qt::DisplayRole:
            break;

        default: // Can't handle this role...
            return QVariant();
    }

    Posting *entry = 0;
    if( index.row() < d->account->countPostings() ) {
        entry = d->account->posting( index.row() );
    } else {
        entry = d->posting;
    }

    Q_ASSERT( entry );

    switch( index.column() ) {
        case MATURITY: {
                QDate dt = entry->maturity();
                if( role == Qt::EditRole ) {
                    return dt;
                }
                if( !Knipptasch::Preferences::self()->userDefinedDateFormat().isEmpty() ) {
                    return dt.toString( Knipptasch::Preferences::self()->userDefinedDateFormat() );
                }
                return formatShortDate( dt );
            }
        case POSTINGTEXT:
            return entry->postingText();

        case AMOUNT:
            if( role == Qt::EditRole ) {
                return QVariant::fromValue( entry->amount() );
            } else if( entry->amount().cents() != 0 ) {
                return entry->amount().toString();
            }
            return QVariant();

        case BALANCE:
            break;

        case VALUEDATE: {
                QDate dt = entry->valueDate();
                if( role == Qt::EditRole ) {
                    return dt;
                }
                if( !Knipptasch::Preferences::self()->userDefinedDateFormat().isEmpty() ) {
                    return dt.toString( Knipptasch::Preferences::self()->userDefinedDateFormat() );
                }
                return formatShortDate( dt );
            }
        case CATEGORY:
            if( role == Qt::EditRole ) {
                return account()->identifierByObject( entry->category() );
            } else if( entry->category() ) {
                return entry->category()->name();
            }

            return QVariant();

        case PAYEE:
            return entry->payee();

        case STATEMENT:
            if( role == Qt::EditRole || entry->page() > 0 ) {
                return entry->page();
            }
            break;

        case VOUCHER:
            entry->voucher();

        case WARRANTY: {
                QDate dt = entry->warranty();
                if( role == Qt::EditRole ) {
                    return dt;
                }
                if( !Knipptasch::Preferences::self()->userDefinedDateFormat().isEmpty() ) {
                    return dt.toString( Knipptasch::Preferences::self()->userDefinedDateFormat() );
                }
                return formatShortDate( dt );
            }
        case PAYMENT:
            return entry->methodOfPayment();

        case DESCRIPTION:
            return entry->description();
    }

    return QVariant();
}


QVariant AccountModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole && section < AccountModel::ENTRYCOUNT ) {
        switch( section ) {
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


bool AccountModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( !index.isValid() ) {
        return false;
    }

    if( !d->account ) {
        return false;
    }

    if( !( role == Qt::EditRole || role == Qt::DisplayRole ) ) {
        return false;
    }

    if( index.column() == BALANCE ) {
        return false;
    }

    Posting *entry = 0;
    if( index.row() < d->account->countPostings() ) {
        entry = d->account->posting( index.row() );
    } else {
        Q_ASSERT( index.row() == d->account->countPostings() );
        entry = d->posting;
    }

    Q_ASSERT( entry );

    switch( index.column() ) {
        case MATURITY:
            entry->setMaturity( value.toDate() );
            break;

        case POSTINGTEXT:
            entry->setPostingText( value.toString().trimmed() );
            break;

        case AMOUNT: {
                bool ok;
                double amount = value.toDouble( &ok );
                if( !ok ) {
                    qDebug() << "invalid amount value";
                } else {
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
            if( role == Qt::EditRole ) {
                bool ok;
                int v = value.toInt( &ok );
                Q_ASSERT( ok );

                Object *object = account()->objectByIdentifier( v );
                Category *category = 0;

                if( object ) {
                    category = qobject_cast<Category *>( object );
                    Q_ASSERT( category );
                }

                entry->setCategory( category );
            }
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

    if( index.row() >= d->account->countPostings() ) {
        if( postingIsValid( d->posting ) ) {
            beginInsertRows( QModelIndex(), rowCount(), rowCount() );
            d->account->addPosting( entry );
            endInsertRows();

            d->posting = new Posting;
            emit dataChanged( createIndex( rowCount() - 1, 0 ),
                              createIndex( rowCount(), columnCount() ) );
        }
    } else {
        emit dataChanged( createIndex( index.row(), index.column() ),
                          createIndex( index.row(), index.column() ) );
    }

    return true;
}


Qt::ItemFlags AccountModel::flags( const QModelIndex &index ) const
{
    if( !index.isValid() ) {
        return 0;
    }

    if( !d->account ) {
        return 0;
    }

    if( index.column() == BALANCE ) {
        return QAbstractItemModel::flags( index );
    }

    return QAbstractTableModel::flags( index ) | Qt::ItemIsEditable;
}


bool AccountModel::removeRows( int row, int count, const QModelIndex &parent )
{
    if( !d->account ) {
        return false;
    }

    bool re = false;

    if(( row + count - 1 ) == d->account->countPostings() ) {
        count -= 1;
        d->posting = new Posting;
        re = true;
    }

    if(( row + count - 1 ) < d->account->countPostings() ) {
        beginRemoveRows( parent, row, row + count - 1 );

        for( int i = 0; i < count; ++i ) {
            d->account->removePosting( row );
        }

        endRemoveRows();
        re = true;
    }

    return re;
}


AccountModel::PostingTypeFlags AccountModel::postingType( const Posting *ptr ) const
{
    Q_ASSERT( d->posting );
    Q_ASSERT( ptr );

    PostingTypeFlags type = 0;

    if( ptr == d->posting ) {
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


bool AccountModel::postingIsValid( const Posting *ptr )
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


QVariant AccountModel::backgroundRoleData( const QModelIndex &index ) const
{
    AccountModel::PostingTypeFlags type = postingType( index.row() );

    if( type & AccountModel::Current && Knipptasch::Preferences::self()->currentPostingBackgroundEnabled() && Knipptasch::Preferences::self()->currentPostingBackgroundColor().isValid() ) {
        return Knipptasch::Preferences::self()->currentPostingBackgroundColor();
    } else if( type & AccountModel::Future && Knipptasch::Preferences::self()->futurePostingBackgroundEnabled() && Knipptasch::Preferences::self()->futurePostingBackgroundColor().isValid() ) {
        return Knipptasch::Preferences::self()->futurePostingBackgroundColor();
    } else if( type & AccountModel::Incomplete && Knipptasch::Preferences::self()->incompletePostingBackgroundEnabled() && Knipptasch::Preferences::self()->incompletePostingBackgroundColor().isValid() ) {
        return Knipptasch::Preferences::self()->incompletePostingBackgroundColor();
    } else if( Knipptasch::Preferences::self()->defaultPostingBackgroundEnabled() && Knipptasch::Preferences::self()->defaultPostingBackgroundColor().isValid() ) {
        return Knipptasch::Preferences::self()->defaultPostingBackgroundColor();
    }

    return QVariant();
}


QVariant AccountModel::foregroundRoleData( const QModelIndex &index ) const
{
    QColor c;

    switch( index.column() ) {
        case AccountModel::AMOUNT:
            if( data( index, Qt::EditRole ).value<Money>() >= 0.0 ) {
                if( Knipptasch::Preferences::self()->positiveAmountForegroundEnabled() ) {
                    c = Knipptasch::Preferences::self()->positiveAmountForegroundColor();
                }
            } else if( Knipptasch::Preferences::self()->negativeAmountForegroundEnabled() ) {
                c = Knipptasch::Preferences::self()->negativeAmountForegroundColor();
            }
            break;

        case AccountModel::WARRANTY:
            if( data( index, Qt::EditRole ).value<QDate>() < QDate::currentDate() ) {
                if( Knipptasch::Preferences::self()->availableWarrantyForegroundEnabled() ) {
                    c = Knipptasch::Preferences::self()->availableWarrantyForegroundColor();
                }
            } else if( Knipptasch::Preferences::self()->expiredWarrantyForegroundEnabled() ) {
                c = Knipptasch::Preferences::self()->expiredWarrantyForegroundColor();
            }
            break;

        default:
            break;
    }

    return c.isValid() ? c : QVariant();
}


QVariant AccountModel::textAlignmentRoleData( const QModelIndex &index ) const
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


QVariant AccountModel::decorationRoleData( const QModelIndex &index ) const
{
    switch( index.column() ) {
        case AccountModel::CATEGORY: {
                Posting *entry = 0;
                if( index.row() < d->account->countPostings() ) {
                    entry = d->account->posting( index.row() );
                } else {
                    entry = d->posting;
                }

                Q_ASSERT( entry );

                if( entry->category() && entry->category()->color().isValid() ) {
                    return entry->category()->color();
                }
            }

        default:
            break;
    }

    return QVariant();
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
