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
#ifndef SPLITPOSTINGTABWIDGET_H
#define SPLITPOSTINGTABWIDGET_H

#include "interface/abstractaccounttabwidget.h"


namespace Ui
{
    class SplitPostingTabWidget;
}


class SplitPostingTabWidget : public AbstractAccountTabWidget
{
    Q_OBJECT

    public:
        explicit SplitPostingTabWidget(QWidget *parent = 0);

    protected:
        void accountModelAboutToBeChanged();
        void accountModelChanged(AccountModel *model);

        void currentSelectedIndexAboutToBeChanged();
        void currentSelectedIndexChanged(const QModelIndex &index);

    private:
        Ui::SplitPostingTabWidget *ui;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
