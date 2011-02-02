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
#include "datedelegate.h"

#include "accountmodel.h"


#include <Knipptasch/DateEdit>
#include <Knipptasch/Preferences>

#include <compat/utils.h>

#include <QPainter>
#include <QDate>
#include <QApplication>
#include <QSortFilterProxyModel>
#include <QFontMetrics>
#include <QDebug>


DateDelegate::DateDelegate(Knipptasch::Preferences *preferences, QObject *parent)
  : QStyledItemDelegate( parent ),
    m_preferences( preferences )
{
}


QWidget* DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    if( !model ) {
        return QStyledItemDelegate::createEditor( parent, option, index );
    }

    DateEdit *input = new DateEdit( m_preferences->userDefinedDateFormat(), parent );
//    input->setCalendarPopup( true );
//    input->setCorrectionMode( DateEdit::CorrectToNearestValue );
    input->setFrame( false );

    return input;
}


void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    DateEdit *input = qobject_cast<DateEdit*>( editor );
    const QAbstractItemModel *model = index.model();

    if( !input || !model ) {
        QStyledItemDelegate::setEditorData( editor, index );
    }
    else {
        QDate dt;
        if( index.isValid() ) {
            const QSortFilterProxyModel *proxy = qobject_cast<const QSortFilterProxyModel*>( model );
            QModelIndex idx = index;

            if( proxy ) {
                QModelIndex idx = proxy->mapToSource( index );
            }

            Q_ASSERT( idx.isValid() );

            dt = model->data( index, Qt::EditRole ).toDate();

            if( !dt.isValid() ) {
                if( idx.column() == AccountModel::MATURITY ) {
                    dt = QDate::currentDate();
                }
                else if( idx.column() == AccountModel::VALUEDATE ) {
                    dt = model->data(
                            model->index( idx.row(), AccountModel::MATURITY ),
                            Qt::EditRole
                        ).toDate();
                }
                else if( idx.column() == AccountModel::WARRANTY ) {
                    dt = model->data(
                            model->index( idx.row(), AccountModel::VALUEDATE ),
                            Qt::EditRole
                        ).toDate();

                    if( dt.isValid() ) {
                        dt = dt.addMonths( m_preferences->defaultLengthOfWarrantyInMonth() );
                    }
                }
            }
        }

        qDebug() << "set editor data:" << ( dt.isValid() ? dt : QDate::currentDate() );
        input->setDate( dt.isValid() ? dt : QDate::currentDate() );
    }
}


void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return;
    }

    DateEdit *input = qobject_cast<DateEdit*>( editor );

    if( !input ) {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
    else {
        qDebug() << "set model data:" << input->date();
        model->setData( index, input->date(), Qt::EditRole );
    }
}


QSize DateDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static int fix = 0;

    if( fix <= 0 ) {
        fix = qApp->fontMetrics()
                        .size(
                                Qt::TextSingleLine,
                                formatShortDate( QDate::currentDate() )
                         ).width();

        fix += 40;
    }

    const QSize size = QStyledItemDelegate::sizeHint( option, index )
                        + QSize( 25, 0 );

    if( size.width() >= fix ) {
        return size;
    }

    return QSize( fix, size.height() );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
