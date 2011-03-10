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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accountwidget.h"

#include "savemodifieddialog.h"
#include "recentfilemenu.h"

#include "config/preferencesconfigdialog.h"

#include "compat/iconloader.h"
#include "compat/standardaction.h"
#include "compat/actioncollection.h"

#include <Knipptasch/ImportPlugin>
#include <Knipptasch/ExportPlugin>
#include <Knipptasch/Account>
#include <Knipptasch/Storage>
#include <Knipptasch/StorageException>
#include <Knipptasch/Preferences>

#if defined(HAVE_KDE)
#include <KMessageBox>
#include <kxmlguifactory.h>
#include <kstatusbar.h>
#include <kglobal.h>
#include <kcomponentdata.h>
#include <KAboutData>
#include <KFileDialog>
#include <KIO/NetAccess>
#else
#include "aboutdialog.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QFileDialog>
#endif

#include <QCloseEvent>
#include <QFile>
#include <QCursor>
#include <QPointer>
#include <QToolButton>
#include <QToolBar>

#include <KIcon>
#include <KAction>
#include <KUrl>
#include <KMenu>

#include <QDebug>
#include "passworddialog.h"
#include "compat/utils.h"
#include <QTimer>
#include <plugin/pluginmanager.h>



#define APPLICATION_WAIT_CURSOR                                                                    \
    struct _application_wait_cursor {                                                              \
      _application_wait_cursor() { QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) ); } \
      ~_application_wait_cursor() { QApplication::restoreOverrideCursor(); }                       \
    };                                                                                             \
    _application_wait_cursor _my_application_wait_cursor


MainWindow::MainWindow(QWidget* parent) :
#if defined(HAVE_KDE)
    KXmlGuiWindow( parent ),
#else
    QMainWindow( parent ),
#endif
    ui( new Ui::MainWindow ),
    m_ActionCollection( new ActionCollection( this ) ),
    m_recentFileMenu( new RecentFileMenu( tr( "Open &Recent" ), this ) ),
    m_exportPluginActionGroup( new QActionGroup( this ) ),
    m_importPluginActionGroup( new QActionGroup( this ) )
{
    ui->setupUi( this );

    setAttribute( Qt::WA_DeleteOnClose );

    setupActions();

#if defined(HAVE_KDE)
    setupGUI();
#else
    restoreGeometry( QByteArray::fromBase64( Knipptasch::Preferences::self()->windowGeometry().toAscii() ) );
    restoreState( QByteArray::fromBase64( Knipptasch::Preferences::self()->windowState().toAscii() ) );
    setWindowIcon( QIcon(":/oxygen/32x32/apps/knipptasch.png") );
#endif

    ui->tabWidget->clear();

    onLoadConfig();

    Knipptasch::PluginManager *manager = Knipptasch::PluginManager::self();
    
    loadPlugins();

    connect( manager, SIGNAL( pluginLoaded(QByteArray) ), this, SLOT( loadPlugins() ) );
    connect( manager, SIGNAL( pluginUnloaded(QByteArray) ), this, SLOT( loadPlugins() ) );
    connect( manager, SIGNAL( pluginDisabled(QByteArray) ), this, SLOT( loadPlugins() ) );
    connect( manager, SIGNAL( pluginEnabled(QByteArray) ), this, SLOT( loadPlugins() ) );
    
    connect( ui->welcomeWidget, SIGNAL( createFileClicked()  ),
             this, SLOT( onNewFile() ) );
    connect( ui->welcomeWidget, SIGNAL( openFileClicked() ),
             this, SLOT( onOpenFile() ) );
    connect( ui->welcomeWidget, SIGNAL( openFileClicked(QString) ),
             this, SLOT( onOpenFile(QString) ) );
    connect( m_recentFileMenu, SIGNAL( openFile(QString) ),
             this, SLOT( onOpenFile(QString) ) );

    connect( ui->tabWidget, SIGNAL( currentChanged(int) ),
             this, SLOT( checkActionStates() ) );
    connect( ui->tabWidget, SIGNAL( tabCloseRequested(int) ),
             this, SLOT( onTabCloseRequest(int) ) );

    connect( m_exportPluginActionGroup, SIGNAL( triggered(QAction*) ),
             this, SLOT( onExportPluginClicked(QAction*) ) );
    connect( m_importPluginActionGroup, SIGNAL( triggered(QAction*) ),
             this, SLOT( onImportPluginClicked(QAction*) ) );

#if defined(HAVE_KDE)
    connect( ui->tabWidget, SIGNAL( contextMenu(QWidget*,const QPoint&) ),
             this, SLOT( onTabContextMenuRequest(QWidget*,const QPoint&) ) );
#endif

    checkActionStates();

    QApplication::processEvents();

    switch( Knipptasch::Preferences::self()->onStartupAction() ) {
        case Knipptasch::Preferences::WelcomePage:
            break;

        case Knipptasch::Preferences::BlankFile:
            onNewFile();
            break;

        case Knipptasch::Preferences::LastFile:
            if( !Knipptasch::Preferences::self()->recentFilesList().isEmpty() ) {
                onOpenFile( Knipptasch::Preferences::self()->recentFilesList().first() );
            }
            break;

        case Knipptasch::Preferences::DefaultFile:
            if( !Knipptasch::Preferences::self()->onStartupActionDefaultFile().isEmpty() ) {
                onOpenFile( Knipptasch::Preferences::self()->onStartupActionDefaultFile() );
            }
            break;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    
    Knipptasch::PluginManager::self()->aboutToQuit();
}


void MainWindow::openFiles(const QList<QUrl> &urlList)
{
    foreach(const QUrl &url, urlList) {
        onOpenFile( url );
    }
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    if( !SaveModifiedDialog::queryClose( this, allAccountWidgets() ) ) {
        event->ignore();
        return;
    }

#if defined(HAVE_KDE)
    KXmlGuiWindow::closeEvent( event );
#else
    Knipptasch::Preferences::self()->setWindowGeometry( saveGeometry().toBase64() );
    Knipptasch::Preferences::self()->setWindowState( saveState().toBase64() );

    QMainWindow::closeEvent( event );
#endif
}


void MainWindow::setupActions()
{
    // File Actions
    StandardAction::openNew( this, SLOT( onNewFile() ), actionCollection() );
    StandardAction::open( this, SLOT( onOpenFile() ), actionCollection() );
    KAction *recentFileAction = actionCollection()->addAction( "file_recent_files" );
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

    // Posting Actions
    KAction *postingDeleteAction = actionCollection()->addAction( "posting_delete", this, SLOT( onPostingDelete() ) );
    postingDeleteAction->setText( tr( "&Delete Posting" ) );
    postingDeleteAction->setIcon( BarIcon("edit-delete") );

    KAction *postingCloneAction = actionCollection()->addAction( "posting_clone", this, SLOT( onPostingClone() ) );
    postingCloneAction->setText( tr( "&Clone Posting" ) );
    //postingCloneAction->setIcon( BarIcon("") );

    KAction *postingValueDateToTodayAction = actionCollection()->addAction( "posting_valuedate_to_today", this, SLOT( onPostingValueDateToToday() ) );
    postingValueDateToTodayAction->setText( tr( "Set Value Date to Today" ) );
    postingValueDateToTodayAction->setIcon( BarIcon("view-calendar") );

    KAction *postingValueDateToMaturityAction = actionCollection()->addAction( "posting_valuedate_to_maturity", this, SLOT( onPostingValueDateToMaturity() ) );
    postingValueDateToMaturityAction->setText( tr( "Set Value Date to Maturity" ) );
    postingValueDateToMaturityAction->setIcon( BarIcon("view-calendar") );

    // Settings Actions
    KAction* configureAccountAction = actionCollection()->addAction( "configure_account", this, SLOT( onConfigureAccount() ) );
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
    m_exportMenu = ui->menuFile->addMenu( BarIcon("document-export"), tr( "Export..." ) );
    m_importMenu = ui->menuFile->addMenu( BarIcon("document-import"), tr( "Import..." ) );
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
    ui->menuSettings->addAction( showStatusbarAction ); //actionCollection()->action( "options_show_statusbar" ) );
    ui->menuSettings->addSeparator();
    ui->menuSettings->addAction( actionCollection()->action( "options_configure" ) );

    KAction* helpAppAction = StandardAction::help( this, SLOT( onHelp() ), actionCollection() );
    ui->menuHelp->addAction( helpAppAction );

    KAction* aboutAppAction = StandardAction::aboutApp( this, SLOT( onAbout() ), actionCollection() );
    ui->menuHelp->addAction( aboutAppAction );

    QToolBar *mainToolBar = new QToolBar( tr( "Main Toolbar" ), this );
    mainToolBar->setObjectName( "MainToolbar" );

    ui->menuSettings->insertAction( showStatusbarAction, mainToolBar->toggleViewAction() );

    mainToolBar->addAction( actionCollection()->action( "file_new" ) );
    mainToolBar->addAction( actionCollection()->action( "file_open" ) );
    mainToolBar->addSeparator();
    mainToolBar->addAction( actionCollection()->action( "file_save" ) );
    mainToolBar->addAction( actionCollection()->action( "file_save_as" ) );
    mainToolBar->addSeparator();
    mainToolBar->addAction( actionCollection()->action( "file_print" ) );
    mainToolBar->addSeparator();
    mainToolBar->addAction( actionCollection()->action( "file_close" ) );

    addToolBar( mainToolBar );
#endif
}


void MainWindow::addAccountWidget(Account *acc, const QString &filename)
{
    APPLICATION_WAIT_CURSOR;

    static quint32 counter = 0;

    Q_ASSERT( acc );

    AccountWidget *widget = new AccountWidget( this );
    widget->setFileName( filename );

    QString tabname = widget->fileName().isEmpty()
                            ? tr( "Untitled %1" ).arg( ++counter )
                            : widget->fileName();

    int index = ui->tabWidget->addTab( widget, tabname );
    ui->tabWidget->setCurrentIndex( index );

    QCoreApplication::processEvents();

    widget->setAccount( acc );
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

    if( ui->stackedWidget->currentIndex() == 0 ) {
        return list;
    }

    for(int i = 0; i < ui->tabWidget->count(); ++i) {
        QWidget *widget = ui->tabWidget->widget( i );
        Q_ASSERT( widget );

        AccountWidget *accountWidget = qobject_cast<AccountWidget*>( widget );
        Q_ASSERT( accountWidget );

        list.append( accountWidget );
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

    m_exportPluginActionGroup->setEnabled( ui->stackedWidget->currentIndex() != 0 );

#if !defined(HAVE_KDE)
    m_exportMenu->setEnabled( ui->stackedWidget->currentIndex() != 0 && !m_exportPluginActionGroup->actions().isEmpty() );
    m_importMenu->setEnabled( !m_importPluginActionGroup->actions().isEmpty() );
#endif

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

#if defined(HAVE_KDE) // only available in KTabWidget, not in QTabWidget
        if( ui->tabWidget->count() == 1 ) {
            ui->tabWidget->setTabBarHidden( Knipptasch::Preferences::self()->hideEmptyTabBar() );
        }
#endif

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

    accountWidget( i )->saveConfig();

    if( SaveModifiedDialog::queryClose( this, accountWidget( i ) ) ) {
        ui->tabWidget->removeTab( i );
    }

    checkActionStates();
}


void MainWindow::onLoadConfig()
{
    QList<AccountWidget*> list = allAccountWidgets();
    foreach(AccountWidget *widget, list ) {
        widget->loadConfig();
    }

    ui->tabWidget->setMovable( Knipptasch::Preferences::self()->movableTabs() );
    ui->tabWidget->setTabsClosable( Knipptasch::Preferences::self()->closeButtonOnTabs() );

    if( Knipptasch::Preferences::self()->tabCornerCloseButton() ) {

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
    actionCollection()->action( "options_show_statusbar" )
                      ->setChecked( Knipptasch::Preferences::self()->showStatusBar() );
    onShowStatusbar();
#endif
}


void MainWindow::onNewFile()
{
    addAccountWidget( new Account() );

    checkActionStates();
}


void MainWindow::onOpenFile(const QString &str)
{
    QString filename = str.trimmed();
    if( filename.isEmpty() ) {
        filename = getOpenFileName( this, QString(), QString(),
                                    Storage::filterStringQt(),
                                    Storage::filterStringKDE() );

        if( filename.isEmpty() ) {
            return;
        }
    }

    Account *acc = new Account;
    try {
        try {
            Storage::readAccount( acc, filename );

            m_recentFileMenu->addFile( filename );
            addAccountWidget( acc, filename );
            statusBar()->showMessage( tr( "File '%1' loaded" ).arg( filename ) , 2000 );
        }
        catch(StoragePasswordException ex) {
            /// ask user for the password
            QScopedPointer<PasswordDialog> dialog( new PasswordDialog( filename, this ) );

            if( dialog->exec() != QDialog::Accepted || !dialog ) {
                delete acc;
                checkActionStates();
                return;
            }

            Storage::readAccount( acc, filename, dialog->password() );
            m_recentFileMenu->addFile( filename );
            addAccountWidget( acc, filename );
            statusBar()->showMessage( tr( "File '%1' loaded" ).arg( filename ) , 2000 );
        }
    }
    catch(StorageException ex) {
        delete acc;

#if defined(HAVE_KDE)
        KMessageBox::error( this, ex.errorMessage() );
#else
        QMessageBox::warning( this, // krazy:exclude=qclasses
                              QObject::tr( "Error - %1" )
                                .arg( QCoreApplication::applicationName() ),
                              ex.errorMessage()
        );
#endif
    }

    checkActionStates();
}


void MainWindow::onOpenFile(const QUrl &url)
{
    QString filename;

#if defined(HAVE_KDE)
    if( KIO::NetAccess::download( url, filename, this ) )
#else
    filename = url.toString();
#endif
    {
        Account *acc = new Account;
        try {
            try {
                Storage::readAccount( acc, filename );

                m_recentFileMenu->addFile( url.toString() );
                addAccountWidget( acc, url.toString() );
                statusBar()->showMessage( tr( "File '%1' loaded" ).arg( url.toString() ) , 2000 );
#if defined(HAVE_KDE)
                KIO::NetAccess::removeTempFile( filename );
#endif
            }
            catch(StoragePasswordException ex) {
                /// ask user for the password
                QScopedPointer<PasswordDialog> dialog( new PasswordDialog( filename, this ) );

                if( dialog->exec() != QDialog::Accepted || !dialog ) {
                    delete acc;
                    checkActionStates();
                    return;
                }

                Storage::readAccount( acc, filename, dialog->password() );

                m_recentFileMenu->addFile( url.toString() );
                addAccountWidget( acc, url.toString() );
                statusBar()->showMessage( tr( "File '%1' loaded" ).arg( url.toString() ) , 2000 );

#if defined(HAVE_KDE)
                KIO::NetAccess::removeTempFile( filename );
#endif
            }
        }
        catch(StorageException ex) {
            delete acc;

#if defined(HAVE_KDE)
            KMessageBox::error( this, ex.errorMessage() );
#else
            QMessageBox::warning( this, // krazy:exclude=qclasses
                                QObject::tr( "Error - %1" )
                                    .arg( QCoreApplication::applicationName() ),
                                ex.errorMessage()
            );
#endif
        }
    }
#if defined(HAVE_KDE)
    else {
        KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
    }
#endif

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
    QPointer<PreferencesConfigDialog> dialog = new PreferencesConfigDialog( Knipptasch::Preferences::self(), this );
    connect( dialog, SIGNAL( committed() ), this, SLOT( onLoadConfig() ) );

    if( dialog->exec() == QDialog::Accepted ) {
        onLoadConfig();
    }

    delete dialog;
}


void MainWindow::onShowStatusbar()
{
    bool statusbarState = actionCollection()->action( "options_show_statusbar" )->isChecked();
    statusBar()->setVisible( statusbarState );

    Knipptasch::Preferences::self()->setShowStatusBar( statusbarState );
}


void MainWindow::onExportPluginClicked(QAction *action)
{
    Q_ASSERT( action );

    AccountWidget *caw = currentAccountWidget();
    int index = action->data().toInt();

    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < m_exportPlugins.size() );
    Q_ASSERT( caw );

    const Knipptasch::ExportPlugin *plugin = m_exportPlugins.at( index );
    plugin->exportAccount( caw->account(), caw->selectedPostings(), this );
}


void MainWindow::onImportPluginClicked(QAction *action)
{
    Q_ASSERT( action );

    int index = action->data().toInt();
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < m_importPlugins.size() );

    const Knipptasch::ImportPlugin *plugin = m_importPlugins.at( index );

    Account *account = plugin->importAccount( this );
    if( account ) {
        addAccountWidget( account );
        statusBar()->showMessage(
                    tr( "Account with %1 postings successfully imported." )
                                        .arg( account->countPostings() ), 2000
        );
    }
}


void MainWindow::onTabContextMenuRequest(QWidget *widget, const QPoint &point)
{
    int index = ui->tabWidget->indexOf( widget );

    if( index >= 0 ) {
        QPointer<KMenu> menu = new KMenu( this );

        menu->addAction( actionCollection()->action( "file_close" ) );
        menu->exec( point );

        menu->deleteLater();
    }
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


void MainWindow::loadPlugins()
{
    unloadPlugins();
    
    QSet<QByteArray> plugins = Knipptasch::PluginManager::self()->enabledPluginIdentifiers();

    QList<QAction*> importActions;
    QList<QAction*> exportActions;
    
    foreach(const QByteArray &pluginName, plugins) {
        Q_ASSERT( Knipptasch::PluginManager::self()->containsPlugin( pluginName ) );
        Knipptasch::Plugin* plugin = Knipptasch::PluginManager::self()->plugin( pluginName );
        Q_ASSERT( plugin );
        Q_ASSERT( plugin->isEnabled() );

        Knipptasch::ImportPlugin *importPlugin = dynamic_cast<Knipptasch::ImportPlugin*>( plugin );
        if( importPlugin ) {
            m_importPlugins.append( importPlugin );
            
            KAction *action = new KAction( KIcon( importPlugin->importActionIcon() ), importPlugin->importActionName(), this );
            action->setData( m_importPlugins.size() - 1 );
            
            importActions.append( action );
            m_importPluginActionGroup->addAction( action );
        }
        
        Knipptasch::ExportPlugin *exportPlugin = dynamic_cast<Knipptasch::ExportPlugin*>( plugin );        
        if( exportPlugin ) {
            m_exportPlugins.append( exportPlugin );
            
            KAction *action = new KAction( KIcon( exportPlugin->exportActionIcon() ), exportPlugin->exportActionName(), this );
            action->setData( m_exportPlugins.size() - 1 );
            
            exportActions.append( action );
            m_exportPluginActionGroup->addAction( action );
        }
    }

#if defined(HAVE_KDE)
    plugActionList( "file_import_actionlist", importActions );
    plugActionList( "file_export_actionlist", exportActions );
#else
    m_importMenu->addActions( importActions );
    m_exportMenu->addActions( exportActions );
#endif
}


void MainWindow::unloadPlugins()
{
    qDeleteAll( m_exportPlugins.begin(), m_exportPlugins.end() );
    m_exportPlugins.clear();

    qDeleteAll( m_importPlugins.begin(), m_importPlugins.end() );
    m_importPlugins.clear();
    
#if defined(HAVE_KDE)
    unplugActionList( "file_import_actionlist" );
    unplugActionList( "file_export_actionlist" );
#else
    m_importMenu->clear();
    m_exportMenu->clear();
#endif
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

