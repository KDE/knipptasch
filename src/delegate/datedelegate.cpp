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

#include <QPainter>
#include <QDateEdit>
#include <QApplication>



DateDelegate::DateDelegate(QObject *parent)
  : QStyledItemDelegate( parent )
{
}


QWidget* DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    if( !model ) {
        return QStyledItemDelegate::createEditor( parent, option, index );
    }

    QDateEdit *input = new QDateEdit( parent );
    input->setCalendarPopup( true );
    input->setDate( QDate::currentDate() );
    input->setCorrectionMode( QDateEdit::CorrectToNearestValue );
    input->setFrame( false );

    return input;
}


void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateEdit *input = qobject_cast<QDateEdit*>( editor );
    const QAbstractItemModel *model = index.model();

    if( !input || !model ) {
        QStyledItemDelegate::setEditorData( editor, index );
    }
    else {
        input->setDate( model->data(index, Qt::EditRole).toDate() );
    }
}


void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return;
    }

    QDateEdit *input = qobject_cast<QDateEdit*>( editor );

    if( !input ) {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
    else {
        model->setData( index, input->date(), Qt::EditRole );
    }
}


QSize DateDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit edit;

    return QSize(
        qMax(
            QStyledItemDelegate::sizeHint( option, index ).width(),
            edit.sizeHint().width() - 10
        ),
        QStyledItemDelegate::sizeHint( option, index ).height()
    );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
