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
#include "splitpostingtabwidget.h"

#include "ui_splitpostingtabwidget.h"

#include "compat/iconloader.h"
#include "backend/account.h"


SplitPostingTabWidget::SplitPostingTabWidget(QWidget *parent)
  : AbstractAccountTabWidget( tr( "Split Posting" ), BarIcon( "view-calendar-list" ), parent ),
    ui( new Ui::SplitPostingTabWidget )
{
    ui->setupUi( this );
}


void SplitPostingTabWidget::accountModelAboutToBeChanged()
{
    //submit
}


void SplitPostingTabWidget::accountModelChanged(AccountModel *model)
{
    Q_UNUSED( model )
}


void SplitPostingTabWidget::currentSelectedIndexAboutToBeChanged()
{
    //submit
}


void SplitPostingTabWidget::currentSelectedIndexChanged(const QModelIndex &index)
{
    Q_UNUSED( index );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
