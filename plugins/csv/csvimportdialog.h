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
#ifndef CSVIMPORTDIALOG_H
#define CSVIMPORTDIALOG_H

#include <QDialog>

class Account;

namespace Ui
{
    class CsvImportDialog;
}


/**
 * @class CsvImportDialog
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class CsvImportDialog : public QDialog
{
    Q_OBJECT

    public:
        CsvImportDialog(QWidget *parent = 0);
        ~CsvImportDialog();

        Account* account() const;

    private slots:
        void onDelimiterComboBoxIndexChanged(int);
        void onDelimiterComboBoxTextChanged();

    private:
        QString m_delimiter;
        Ui::CsvImportDialog *ui;
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
