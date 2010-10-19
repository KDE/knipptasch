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
#include "postingtextdelegate.h"

#include <QPainter>
#include <QApplication>

#include <KLineEdit>
#include <QCompleter>


PostingTextDelegate::PostingTextDelegate(QObject *parent)
  : QStyledItemDelegate( parent )
{
}


QWidget* PostingTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QAbstractItemModel *model = const_cast<QAbstractItemModel*>( index.model() );

    if( !model ) {
        return QStyledItemDelegate::createEditor( parent, option, index );
    }

    KLineEdit *input = new KLineEdit( parent );
    input->setFrame( false );

    QCompleter *completer = new QCompleter( input );
    completer->setCaseSensitivity( Qt::CaseInsensitive );
    completer->setModel( model );
    completer->setCompletionColumn( index.column() );
    input->setCompleter( completer );

#if defined(HAVE_KDE)
    input->setClearButtonShown( true );
#endif

    return input;
}


void PostingTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    KLineEdit *input = qobject_cast<KLineEdit*>( editor );
    const QAbstractItemModel *model = index.model();

    if( !input || !model ) {
        QStyledItemDelegate::setEditorData( editor, index );
    }
    else {
        input->setText( model->data(index, Qt::EditRole).toString() );
    }
}


void PostingTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return;
    }

    KLineEdit *input = qobject_cast<KLineEdit*>( editor );

    if( !input ) {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
    else {
        model->setData( index, input->text(), Qt::EditRole );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
