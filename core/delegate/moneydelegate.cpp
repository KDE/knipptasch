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
#include "moneydelegate.h"
#include "backend/money.h"

#include <Knipptasch/Preferences>

#include <QPainter>
#include <QDoubleSpinBox>
#include <QApplication>



MoneyDelegate::MoneyDelegate(Knipptasch::Preferences *preferences, QObject *parent)
  : QStyledItemDelegate( parent ),
    m_preferences( preferences )
{
}


QWidget* MoneyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    if( !model ) {
        return QStyledItemDelegate::createEditor( parent, option, index );
    }

    QDoubleSpinBox *input = new QDoubleSpinBox( parent );
    input->setMaximum( 999999.99 );
    input->setMinimum( -999999.99 );
    input->setDecimals( 2 );
    input->setFrame( false );

    return input;
}


void MoneyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox *input = qobject_cast<QDoubleSpinBox*>( editor );
    const QAbstractItemModel *model = index.model();

    if( !input || !model ) {
        QStyledItemDelegate::setEditorData( editor, index );
    }
    else {
        double value = model->data( index, Qt::EditRole ).value<Money>();
        input->setValue( value );
    }
}


void MoneyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return;
    }

    QDoubleSpinBox *input = qobject_cast<QDoubleSpinBox*>( editor );

    if( !input ) {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
    else {
        model->setData( index, input->value(), Qt::EditRole );
    }
}


QSize MoneyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static int minsize = qApp->fontMetrics().width( "-2000,00" );
    const QSize size = QStyledItemDelegate::sizeHint( option, index );
    int width = size.width() + 25;

    return QSize( qMax( minsize, width ), size.height() );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
