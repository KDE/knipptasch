/*
 * Copyright 2008-2010 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef ACCOUNTWIDGET_H
#define ACCOUNTWIDGET_H

#include <QWidget>
#include <QUrl>


#include "compat/actioncollection.h"

class AbstractAccountTabWidget;

namespace Ui
{
    class AccountWidget;
}

class Posting;
class Account;
class AccountModel;
class AccountSortFilterProxyModel;
class QUndoStack;
class QModelIndex;
class MainWindow;


class AccountWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit AccountWidget(MainWindow *mainWindow);
        ~AccountWidget();

        bool isModified() const;

        QString fileName() const;
        void setFileName(const QString &name);

        Account* account();
        const Account* account() const;
        void setAccount(Account *acc);

        void loadConfig();
        void saveConfig();

        void checkActionState();

        bool selectionContainsCurrentRow() const;
        bool hasSelectedRows() const;
        bool hasOneSelectedRow() const;
        bool hasSeveralSelectedRows() const;
        int countSelectedRows() const;

        QList<const Posting*> selectedPostings() const;

    public slots:
        void selectCurrentPosting();

    public slots:
        bool onSaveFile();
        bool onSaveAsFile(const QString &str = QString());
        void onPrintFile();

        void onRedo();
        void onUndo();
        void onCut();
        void onCopy();
        void onPaste();
        void onFind();

        void onPostingClone();
        void onPostingDelete();
        void onPostingValueDateToToday();
        void onPostingValueDateToMaturity();

        void onConfigureAccount();

        void onResizeColumnToContents(int);

    signals:
        void changed();

    protected:
        bool eventFilter(QObject *obj, QEvent *event);

    private slots:
        void slotSetIndexToCurrentInput();
        void slotUpdateAccountInfo();
        void slotCurrentRowChanged();
        
        void slotUpdateAccountTabWidget(AbstractAccountTabWidget *widget);

        void closeSearchWidget();

    private:
#if defined(HAVE_KDE)
        KActionCollection* mainWindowActionCollection();
#else
        ActionCollection* mainWindowActionCollection();
#endif

        void showHeaderMenu(QContextMenuEvent *e, const QModelIndex &index);
        void showTableMenu(QContextMenuEvent *e, const QModelIndex &index);

    private:
        void loadAccountTabWidgetPlugins();

    private:
        Ui::AccountWidget *ui;
        AccountModel *m_model;
        AccountSortFilterProxyModel *m_proxy;
        MainWindow *m_mainWindow;

        QString m_filename;

        QList<AbstractAccountTabWidget*> m_tabwidgets;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
