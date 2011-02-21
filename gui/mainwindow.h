/*
 * Copyright 2007-2011 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "compat/actioncollection.h"

#include <QMainWindow>


#if defined(HAVE_KDE)
#include <KXmlGuiWindow>
#endif

#include <KAction>

class QUrl;
class QMenu;
class AccountWidget;
class ActionCollection;
class RecentFileMenu;
class Account;


namespace Knipptasch 
{        
    class Preferences;
    class ImportPlugin;
    class ExportPlugin;
}


namespace Ui
{
    class MainWindow;
}


/**
 * @class MainWindow
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class MainWindow :
#if defined(HAVE_KDE)
    public KXmlGuiWindow
#else
    public QMainWindow
#endif
{
    Q_OBJECT
    Q_DISABLE_COPY( MainWindow )

    public:
        explicit MainWindow(QWidget* parent = 0);
        ~MainWindow();
        
#if defined(HAVE_KDE)
        KActionCollection* mainWindowActionCollection() { return actionCollection(); }
#else
        ActionCollection* mainWindowActionCollection() { return actionCollection(); }
#endif

        void openFiles(const QList<QUrl> &urlList);

    protected:
        void closeEvent(QCloseEvent* event);

        void setupActions();

        void addAccountWidget(Account *acc, const QString &filename = QString() );

        AccountWidget* currentAccountWidget();
        AccountWidget* accountWidget(int index);
        QList<AccountWidget*> allAccountWidgets();

#if !defined(HAVE_KDE)
        ActionCollection* actionCollection() { return m_ActionCollection; }
#endif

    protected slots:
        void checkActionStates();
        void onTabCloseRequest(int index);

        void onLoadConfig();

        void onNewFile();
        void onOpenFile(const QString &str = QString());
        void onOpenFile(const QUrl &url);
        void onSaveFile();
        void onSaveAsFile();
        void onPrintFile();
        void onCloseFile();

        void onUndo();
        void onRedo();
        void onCut();
        void onCopy();
        void onPaste();
        void onFind();

        void onPostingClone();
        void onPostingDelete();
        void onPostingValueDateToToday();
        void onPostingValueDateToMaturity();

        void onConfigureAccount();
        void onConfigure();

        void onShowStatusbar();

        void onExportPluginClicked(QAction *action);
        void onImportPluginClicked(QAction *action);

        void onTabContextMenuRequest(QWidget*,const QPoint&);

#if !defined(HAVE_KDE)
        void onAbout();
        void onHelp();
#endif

        void loadPlugins();
        void unloadPlugins();

    private:
        Ui::MainWindow *ui;
        
        ActionCollection *m_ActionCollection;
        RecentFileMenu *m_recentFileMenu;

        QList<Knipptasch::ExportPlugin*> m_exportPlugins;
        QActionGroup *m_exportPluginActionGroup;

        QList<Knipptasch::ImportPlugin*> m_importPlugins;
        QActionGroup *m_importPluginActionGroup;

#if !defined(HAVE_KDE)
        QMenu *m_exportMenu;
        QMenu *m_importMenu;
#endif
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
