/*
 * Copyright 2007-2010 by Stefan Böhmann <kde@hilefoks.org>
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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accountwidget.h"
#include "preferences.h"

#include "savemodifieddialog.h"
#include "settingsdialog.h"
#include "recentfilemenu.h"

#include "backend/account.h"
#include "backend/storage.h"

#include "compat/iconloader.h"
#include "compat/standardaction.h"
#include "compat/actioncollection.h"

#if defined(HAVE_KDE)
#include <KMessageBox>
#include <kxmlguifactory.h>
#include <kstatusbar.h>
#include <kglobal.h>
#include <kcomponentdata.h>
#include <KAboutData>
#include <KFileDialog>
#else
#include "aboutdialog.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QFileDialog>
#endif

#include <QCloseEvent>
#include <QFile>
#include <QPointer>

#include <KIcon>
#include <KAction>
#include <KUrl>

#include <QDebug>
#include <QToolButton>


MainWindow::MainWindow(QWidget* parent) :
#if defined(HAVE_KDE)
    KXmlGuiWindow( parent ),
#else
    QMainWindow( parent ),
#endif
    ui( new Ui::MainWindow ),
    m_ActionCollection( new ActionCollection( this ) ),
    m_recentFileMenu( new RecentFileMenu( tr( "Open &Recent" ), this ) )
{
    ui->setupUi( this );

    setAttribute( Qt::WA_DeleteOnClose );

    setupActions();

#if defined(HAVE_KDE)
    setupGUI();
#else
    restoreGeometry( Preferences::self()->value( "MainWindow", "geometry" ).toByteArray() );
    restoreState( Preferences::self()->value( "MainWindow", "state" ).toByteArray() );
    setWindowIcon( QIcon(":/oxygen/32x32/apps/knipptasch.png") );
#endif

    ui->tabWidget->clear();

    loadConfig();

    connect( ui->welcomeWidget, SIGNAL( createFileClicked()  ), this, SLOT( onNewFile() ) );
    connect( ui->welcomeWidget, SIGNAL( openFileClicked() ), this, SLOT( onOpenFile() ) );
    connect( ui->welcomeWidget, SIGNAL( openFileClicked(QString) ), this, SLOT( onOpenFile(QString) ) );

    connect( ui->tabWidget, SIGNAL( currentChanged(int) ), this, SLOT( checkActionStates() ) );
    connect( ui->tabWidget, SIGNAL( tabCloseRequested(int) ), this, SLOT( onTabCloseRequest(int) ) );

    checkActionStates();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    if( !SaveModifiedDialog::queryClose( this, allAccountWidgets() ) ) {
        event->ignore();
        return;
    }

    Preferences::self()->sync();

#if defined(HAVE_KDE)
    KXmlGuiWindow::closeEvent( event );
#else
    Preferences::self()->setValue( "MainWindow", "geometry", saveGeometry() );
    Preferences::self()->setValue( "MainWindow", "state", saveState() );

    QMainWindow::closeEvent( event );
#endif
}


void MainWindow::setupActions()
{
    // File Actions
    StandardAction::openNew( this, SLOT( onNewFile() ), actionCollection() );
    StandardAction::open( this, SLOT( onOpenFile() ), actionCollection() );
    QAction *recentFileAction = actionCollection()->addAction( "file_recent_files" );
    recentFileAction->setText( tr( "Open &Recent" ) );
    recentFileAction->setIcon( BarIcon("document-open-recent") );
    recentFileAction->setMenu( m_recentFileMenu );

    StandardAction::save( this, SLOT( onSaveFile() ), actionCollection() );
    StandardAction::saveAs( this, SLOT( onSaveAsFile() ), actionCollection() );
    StandardAction::print( this, SLOT( onPrintFile() ), actionCollection() );
    StandardAction::close( this, SLOT( onCloseFile() ), actionCollection() );
    StandardAction::quit( this, SLOT( close() ), actionCollection() );

    // Edit Actions
    StandardAction::undo( this, SLOT( onUndo() ), actionCollection() );
    StandardAction::redo( this, SLOT( onRedo() ), actionCollection() );
    StandardAction::cut( this, SLOT( onCut() ), actionCollection() );
    StandardAction::copy( this, SLOT( onCopy() ), actionCollection() );
    StandardAction::paste( this, SLOT( onPaste() ), actionCollection() );
    StandardAction::find( this, SLOT( onFind() ), actionCollection() );
    StandardAction::findNext( this, SLOT( onFindNext() ), actionCollection() );
    StandardAction::findPrev( this, SLOT( onFindPrev() ), actionCollection() );

    // Posting Actions
    QAction *postingDeleteAction = actionCollection()->addAction( "posting_delete", this, SLOT( onPostingDelete() ) );
    postingDeleteAction->setText( tr( "&Delete Posting" ) );
    postingDeleteAction->setIcon( BarIcon("edit-delete") );

    QAction *postingCloneAction = actionCollection()->addAction( "posting_clone", this, SLOT( onPostingClone() ) );
    postingCloneAction->setText( tr( "&Clone Posting" ) );
    //postingCloneAction->setIcon( BarIcon("") );

    QAction *postingValueDateToTodayAction = actionCollection()->addAction( "posting_valuedate_to_today", this, SLOT( onPostingValueDateToToday() ) );
    postingValueDateToTodayAction->setText( tr( "Set Value Date to Today" ) );
    postingValueDateToTodayAction->setIcon( BarIcon("view-calendar") );

    QAction *postingValueDateToMaturityAction = actionCollection()->addAction( "posting_valuedate_to_maturity", this, SLOT( onPostingValueDateToMaturity() ) );
    postingValueDateToMaturityAction->setText( tr( "Set Value Date to Maturity" ) );
    postingValueDateToMaturityAction->setIcon( BarIcon("view-calendar") );

    // Settings Actions
    QAction* configureAccountAction = actionCollection()->addAction( "configure_account", this, SLOT( onConfigureAccount() ) );
    configureAccountAction->setText( tr( "Configure &Account..." ) );
    configureAccountAction->setIcon( BarIcon("view-bank-account") );

    StandardAction::preferences( this, SLOT( onConfigure() ), actionCollection() );

#if !defined(HAVE_KDE)
    ui->menuFile->addAction( actionCollection()->action( "file_new" ) );
    ui->menuFile->addAction( actionCollection()->action( "file_open" ) );
    ui->menuFile->addAction( actionCollection()->action( "file_recent_files" ) );
    ui->menuFile->addSeparator();
    ui->menuFile->addAction( actionCollection()->action( "file_save" ) );
    ui->menuFile->addAction( actionCollection()->action( "file_save_as" ) );
    ui->menuFile->addSeparator();
    ui->menuFile->addAction( actionCollection()->action( "file_print" ) );
    ui->menuFile->addSeparator();
    ui->menuFile->addAction( actionCollection()->action( "file_close" ) );
    ui->menuFile->addSeparator();
    ui->menuFile->addAction( actionCollection()->action( "file_quit" ) );

    ui->menuEdit->addAction( actionCollection()->action( "edit_undo" ) );
    ui->menuEdit->addAction( actionCollection()->action( "edit_redo" ) );
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction( actionCollection()->action( "edit_cut" ) );
    ui->menuEdit->addAction( actionCollection()->action( "edit_copy" ) );
    ui->menuEdit->addAction( actionCollection()->action( "edit_paste" ) );
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction( actionCollection()->action( "edit_find" ) );
    ui->menuEdit->addAction( actionCollection()->action( "edit_find_next" ) );
    ui->menuEdit->addAction( actionCollection()->action( "edit_find_prev" ) );

    ui->menuPosting->addAction( actionCollection()->action( "posting_clone" ) );
    ui->menuPosting->addSeparator();
    ui->menuPosting->addAction( actionCollection()->action( "posting_delete" ) );
    ui->menuPosting->addSeparator();
    ui->menuPosting->addAction( actionCollection()->action( "posting_valuedate_to_today" ) );
    ui->menuPosting->addAction( actionCollection()->action( "posting_valuedate_to_maturity" ) );

    KAction* showStatusbarAction = StandardAction::showStatusbar( this, SLOT( onShowStatusbar() ), actionCollection() );
    showStatusbarAction->setChecked( true );

    ui->menuSettings->addAction( actionCollection()->action( "configure_account" ) );
    ui->menuSettings->addSeparator();

//    QMenu* toolbarMenu = ui->menuSettings->addMenu( tr( "Show &Toolbar" ) );
//    toolbarMenu->addAction( ui->mainToolBar->toggleViewAction() );
    ui->menuSettings->addAction( ui->mainToolBar->toggleViewAction() );
    ui->menuSettings->addAction( actionCollection()->action( "options_show_statusbar" ) );
    ui->menuSettings->addSeparator();
    ui->menuSettings->addAction( actionCollection()->action( "options_configure" ) );

    KAction* helpAppAction = StandardAction::help( this, SLOT( onHelp() ), actionCollection() );
    ui->menuHelp->addAction( helpAppAction );

    KAction* aboutAppAction = StandardAction::aboutApp( this, SLOT( onAbout() ), actionCollection() );
    ui->menuHelp->addAction( aboutAppAction );

    ui->mainToolBar->addAction( actionCollection()->action( "file_new" ) );
    ui->mainToolBar->addAction( actionCollection()->action( "file_open" ) );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( actionCollection()->action( "file_save" ) );
    ui->mainToolBar->addAction( actionCollection()->action( "file_save_as" ) );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( actionCollection()->action( "file_print" ) );
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction( actionCollection()->action( "file_close" ) );
#endif
}


void MainWindow::loadConfig()
{
    QList<AccountWidget*> list = allAccountWidgets();
    foreach(AccountWidget *widget, list ) {
        widget->loadConfig();
    }

    ui->tabWidget->setMovable( Preferences::self()->value<bool>( "General", "MovableTabs", true ) );
    ui->tabWidget->setTabsClosable( Preferences::self()->value<bool>( "General", "CloseButtonOnTabs", false ) );

    if( Preferences::self()->value<bool>( "General", "TabCornerCloseButton", true ) ) {

        if( !ui->tabWidget->cornerWidget( Qt::TopRightCorner ) ) {
            QToolButton *button = new QToolButton( ui->tabWidget );
            button->setToolTip( tr( "Close the current tab" ) );
            button->setIcon( BarIcon("tab-close") );
            button->setAutoRaise( true );
            button->setEnabled( ui->stackedWidget->currentIndex() != 0 );

            connect( button, SIGNAL( clicked(bool) ), this, SLOT( onCloseFile() ) );

            ui->tabWidget->setCornerWidget( button, Qt::TopRightCorner );
        }
        else {
            ui->tabWidget->cornerWidget( Qt::TopRightCorner )->setVisible( true );
        }
    }
    else {
        ui->tabWidget->cornerWidget( Qt::TopRightCorner )->setVisible( false );
    }

#if !defined(HAVE_KDE)
    actionCollection()->action( "options_show_statusbar" )->setChecked( Preferences::self()->showStatusbar() );
    onShowStatusbar();
#endif
}


void MainWindow::addAccountWidget(AccountWidget *widget)
{
    static quint32 counter = 0;
    Q_ASSERT( widget );

    QString tabname = widget->fileName().isEmpty()
                            ? tr( "Untitled %1" ).arg( ++counter )
                            : widget->fileName();

    int index = ui->tabWidget->addTab( widget, tabname );
    ui->tabWidget->setCurrentIndex( index );

    connect( widget, SIGNAL( changed() ), this, SLOT( checkActionStates() ) );

    checkActionStates();
}


AccountWidget* MainWindow::currentAccountWidget()
{
    Q_ASSERT( ui->stackedWidget->currentIndex() == 1 );

    QWidget *widget = ui->tabWidget->currentWidget();
    Q_ASSERT( widget );

    AccountWidget *accountWidget = qobject_cast<AccountWidget*>( widget );
    Q_ASSERT( accountWidget );

    return accountWidget;
}


AccountWidget* MainWindow::accountWidget(int index)
{
    Q_ASSERT( ui->stackedWidget->currentIndex() == 1 );
    Q_ASSERT( index < ui->tabWidget->count() );

    index = index < 0 ? ui->tabWidget->currentIndex() : index;
    Q_ASSERT( index >= 0 );

    QWidget *widget = ui->tabWidget->widget( index );
    Q_ASSERT( widget );

    AccountWidget *aWidget = qobject_cast<AccountWidget*>( widget );
    Q_ASSERT( aWidget );

    return aWidget;
}


QList<AccountWidget*> MainWindow::allAccountWidgets()
{
    QList<AccountWidget*> list;

    if( ui->stackedWidget->currentIndex() == 1 ) {
        for(int i = 0; i < ui->tabWidget->count(); ++i) {
            QWidget *widget = ui->tabWidget->currentWidget();
            Q_ASSERT( widget );

            AccountWidget *accountWidget = qobject_cast<AccountWidget*>( widget );
            Q_ASSERT( accountWidget );

            list.append( accountWidget );
        }
    }

    return list;
}


void MainWindow::checkActionStates()
{
    if( ui->tabWidget->count() <= 0 ) {
        ui->stackedWidget->setCurrentIndex( 0 );
    }
    else if( ui->stackedWidget->currentIndex() == 0 ) {
        ui->stackedWidget->setCurrentIndex( 1 );
    }

    if( ui->tabWidget->cornerWidget( Qt::TopRightCorner ) ) {
        ui->tabWidget->cornerWidget( Qt::TopRightCorner )
            ->setEnabled( ui->stackedWidget->currentIndex() != 0 );
    }

    if( ui->stackedWidget->currentIndex() == 0 ) {
        actionCollection()->action("file_save")->setEnabled( false );
        actionCollection()->action("file_save_as")->setEnabled( false );
        actionCollection()->action("file_print")->setEnabled( false );
        actionCollection()->action("file_close")->setEnabled( false );

        actionCollection()->action( "edit_undo" )->setEnabled( false );
        actionCollection()->action( "edit_redo" )->setEnabled( false );
        actionCollection()->action( "edit_cut" )->setEnabled( false );
        actionCollection()->action( "edit_copy" )->setEnabled( false );
        actionCollection()->action( "edit_paste" )->setEnabled( false );
        actionCollection()->action( "edit_find" )->setEnabled( false );
        actionCollection()->action( "edit_find_next" )->setEnabled( false );
        actionCollection()->action( "edit_find_prev" )->setEnabled( false );

        actionCollection()->action( "posting_delete" )->setEnabled( false );
        actionCollection()->action( "posting_clone" )->setEnabled( false );
        actionCollection()->action( "posting_valuedate_to_today" )->setEnabled( false );
        actionCollection()->action( "posting_valuedate_to_maturity" )->setEnabled( false );

        actionCollection()->action( "configure_account" )->setEnabled( false );

#if defined(HAVE_KDE)
        setCaption( "" );
#else
        setWindowTitle( QCoreApplication::applicationName() );
#endif
    }
    else {
        for(int i = 0; i < ui->tabWidget->count(); ++i) {
            QWidget *widget = ui->tabWidget->widget( i );
            Q_ASSERT( widget );

            AccountWidget *acw = qobject_cast<AccountWidget*>( widget );
            Q_ASSERT( acw );

            if( acw->isModified() ) {
                ui->tabWidget->setTabIcon( i, BarIcon("document-save") );
            }
            else {
                ui->tabWidget->setTabIcon( i, KIcon() );
            }

            QString filename = acw->fileName();
            if( filename.isEmpty() ) {
                ui->tabWidget->setTabText( i, tr( "Untitled" ) );
            }
            else {
                QFileInfo finfo( filename );
                ui->tabWidget->setTabText( i, finfo.fileName() );
            }
        }

        Q_ASSERT( currentAccountWidget() );

        currentAccountWidget()->checkActionState();

        QString caption = !currentAccountWidget()->fileName().isEmpty()
                                    ? currentAccountWidget()->fileName()
                                    : tr( "Untitled" );

        if( currentAccountWidget()->isModified() ) {
            caption.append( QString( " [%1]" ).arg( tr( "modified" ) ) );
        }

#if defined(HAVE_KDE)
        setCaption( caption );
#else
        setWindowTitle( QString( "%1 - %2" )
                            .arg( caption )
                            .arg( QCoreApplication::applicationName() )
        );
#endif
    }
}


void MainWindow::onTabCloseRequest(int index)
{
    int i = index >= 0 ? index : ui->tabWidget->currentIndex();

    if( SaveModifiedDialog::queryClose( this, accountWidget( i ) ) ) {
        ui->tabWidget->removeTab( i );
    }

    checkActionStates();
}


void MainWindow::onNewFile()
{
    addAccountWidget( new AccountWidget( new Account(), this ) );

    checkActionStates();
}


void MainWindow::onOpenFile(const QString &str)
{
    QString filename = str.trimmed();
    if( filename.isEmpty() ) {
#if defined(HAVE_KDE)
        filename = KFileDialog::getOpenFileName( KUrl(), "*.ka|" + tr( "All Supported Files" ), this );
#else
        filename = QFileDialog::getOpenFileName( this, // krazy:exclude=qclasses
                     tr( "Open File - %1" ).arg( QCoreApplication::applicationName() ),
                     QString(), tr( "All Supported Files" ) + " (*.ka)"
                   );
#endif

        if( filename.isEmpty() ) {
            return;
        }
    }

    Account *acc = new Account;
    if( Storage::readAccount( this, acc, filename ) != QFile::NoError ) {
        delete acc;
    }
    else {
        AccountWidget *widget = new AccountWidget( acc, this );
        widget->setFileName( filename );
        acc->setModified( false );

        m_recentFileMenu->addFile( filename );

        statusBar()->showMessage( tr( "File '%1' loaded" ).arg( filename ) , 2000 );

        addAccountWidget( widget );
    }

    checkActionStates();
}


void MainWindow::onSaveFile()
{
    Q_ASSERT( currentAccountWidget() );

    if( currentAccountWidget()->onSaveFile() ) {
        statusBar()->showMessage( tr( "File saved" ), 2000 );
    }

    checkActionStates();
}


void MainWindow::onSaveAsFile()
{
    Q_ASSERT( currentAccountWidget() );

    if( currentAccountWidget()->onSaveAsFile() ) {
        QString filename = currentAccountWidget()->fileName();

        m_recentFileMenu->addFile( filename );
        statusBar()->showMessage( tr( "File '%1' saved" ).arg( filename ), 2000 );
    }

    checkActionStates();
}


void MainWindow::onPrintFile()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onPrintFile();
}


void MainWindow::onCloseFile()
{
    Q_ASSERT( currentAccountWidget() );

    onTabCloseRequest( -1 );
}


void MainWindow::onUndo()
{
    /* TODO */
}


void MainWindow::onRedo()
{
    /* TODO */
}


void MainWindow::onCut()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onCut();
}


void MainWindow::onCopy()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onCopy();
}


void MainWindow::onPaste()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onPaste();
}


void MainWindow::onFind()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onFind();
}


void MainWindow::onFindNext()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onFindNext();
}


void MainWindow::onFindPrev()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onFindPrev();
}


void MainWindow::onReplace()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onReplace();
}


void MainWindow::onPostingClone()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onPostingClone();
}


void MainWindow::onPostingDelete()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onPostingDelete();
}


void MainWindow::onPostingValueDateToToday()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onPostingValueDateToToday();
}


void MainWindow::onPostingValueDateToMaturity()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onPostingValueDateToMaturity();
}


void MainWindow::onConfigureAccount()
{
    Q_ASSERT( currentAccountWidget() );

    currentAccountWidget()->onConfigureAccount();
}


void MainWindow::onConfigure()
{
    QPointer<SettingsDialog> dialog = new SettingsDialog( this );

    if( dialog->exec() == QDialog::Accepted ) {
        loadConfig();
    }

    delete dialog;
}


void MainWindow::onShowStatusbar()
{
    bool statusbarState = actionCollection()->action( "options_show_statusbar" )->isChecked();
    statusBar()->setVisible( statusbarState );

    Preferences::self()->setShowStatusbar( statusbarState );
}


#if !defined(HAVE_KDE)

void MainWindow::onAbout()
{
    QDialog *dialog = 0; // krazy:exclude=qclasses

    if( !( dialog = findChild<AboutDialog*>() ) ) {
        dialog = new AboutDialog( this );
    }

    dialog->show();
}


void MainWindow::onHelp()
{
    //TODO

    QMessageBox::information( // krazy:exclude=qclasses
        this,
        tr( "Sorry - %1" ).arg( QCoreApplication::applicationName() ),
        tr( "<qt><b>Sorry!</b><br/>No help content available!</qt>" )
    );
}

#endif



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

