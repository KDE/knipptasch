/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef ACCOUNTSETTINGSDIALOG_H
#define ACCOUNTSETTINGSDIALOG_H

#include <QDialog>

class QString;
class QModelIndex;

class Account;
class Category;
class PasswordWidget;
class CategoryModel;


namespace Ui {
    class AccountSettingsDialog;
}


/**
 * @class AccountSettingsDialog
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class AccountSettingsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit AccountSettingsDialog(Account *account, QWidget* parent = 0);
        ~AccountSettingsDialog();

        Account* account();
        const Account* account() const;
        void setAccount(Account *account);

    private slots:
        void onValueChanged();
        void onApplyChanges();
        void onCategoryChanged();

        void onContextMenu(const QPoint &point);

        void addCategoryClicked();
        void addSubCategoryClicked();
        void importCategoryClicked();
        void exportCategoryClicked();
        void removeCategoryClicked();

        void selectNewCategory(const QModelIndex&, int);

    private:
        void addCategory(bool belowOfCurrent);

    private:
        Ui::AccountSettingsDialog *ui;
        PasswordWidget *m_passwordWidget;

        CategoryModel *m_model;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
