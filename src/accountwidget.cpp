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
#include "accountwidget.h"
#include "ui_accountwidget.h"

#include "mainwindow.h"
#include "preferences.h"
#include "accountmodel.h"
#include "accountsortfilterproxymodel.h"
#include "accountsettingsdialog.h"
#include "quickreportpopup.h"

#include "delegate/datedelegate.h"
#include "delegate/postingtextdelegate.h"

#include "compat/iconloader.h"
#include "compat/actioncollection.h"
#include "compat/standardaction.h"
#include <compat/utils.h>

#include "backend/money.h"
#include "backend/storage.h"

#include <modeltest/modeltest.h>

#if defined(HAVE_KDE)
#include <KMessageBox>
#include <KFileDialog>
#else
#include <QMessageBox>
#include <QFileDialog>
#endif

#include <QMenu>
#include <QPointer>
#include <QContextMenuEvent>
#include "delegate/moneydelegate.h"


AccountWidget::AccountWidget(Account *account, MainWindow *mainWindow)
  : QWidget( mainWindow ),
    ui( new Ui::AccountWidget ),
    m_model( 0 ),
    m_proxy( 0 ),
    m_mainWindow( mainWindow )
{
    ui->setupUi( this );

    ui->balance->setMenu( new QuickReportPopup( this ) );

    m_model = new AccountModel( account, this );
    new ModelTest( m_model, this );

    m_proxy = new AccountSortFilterProxyModel( this );
    m_proxy->setSourceModel( m_model );
    m_proxy->sort( AccountModel::MATURITY, Qt::AscendingOrder );
    m_proxy->setDynamicSortFilter( true );
    new ModelTest( m_proxy, this );

    ui->view->setModel( m_proxy );

    ui->view->setItemDelegateForColumn( AccountModel::MATURITY, new DateDelegate( this ) );
    ui->view->setItemDelegateForColumn( AccountModel::POSTINGTEXT, new PostingTextDelegate( this ) );
    ui->view->setItemDelegateForColumn( AccountModel::AMOUNT, new MoneyDelegate( this ) );
    ui->view->setItemDelegateForColumn( AccountModel::VALUEDATE, new DateDelegate( this ) );
    ui->view->setItemDelegateForColumn( AccountModel::WARRANTY, new DateDelegate( this ) );
    //ui->view->setItemDelegateForColumn();

    ui->view->resizeColumnsToContents();
    ui->view->verticalHeader()->hide();
    ui->view->setSelectionBehavior( QAbstractItemView::SelectRows );

    ui->view->horizontalHeader()->setResizeMode( AccountModel::POSTINGTEXT, QHeaderView::Stretch );
    ui->view->horizontalHeader()->installEventFilter( this );
    ui->view->installEventFilter( this );

    ui->searchWidget->setVisible( false );
    ui->replaceWidget->setVisible( false );

    QByteArray arr = Preferences::self()->value<QByteArray>( "TableView", "HorizontalHeaderState" );
    if( arr.isEmpty() ) {
        ui->view->setColumnHidden( AccountModel::TYPE, true );
        ui->view->setColumnHidden( AccountModel::PAYEE, true );
        ui->view->setColumnHidden( AccountModel::STATEMENT, true );
        ui->view->setColumnHidden( AccountModel::VOUCHER, true );
        ui->view->setColumnHidden( AccountModel::WARRANTY, true );
        ui->view->setColumnHidden( AccountModel::PAYMENT, true );
        ui->view->setColumnHidden( AccountModel::DESCRIPTION, true );
    }
    else {
        ui->view->horizontalHeader()->restoreState( arr );
    }

    ui->searchWidget->setItemView( ui->view );


    connect( ui->view->horizontalHeader(), SIGNAL( sectionDoubleClicked(int) ), this, SLOT( onResizeColumnToContents(int) ) );
    connect( ui->searchWidget, SIGNAL( transitionButtonClicked() ), this, SLOT( onSearchReplaceTransition() ) );
    //connect( ui->searchWidget, SIGNAL( clear() ), m_proxy, SLOT( invalidate() ) );
    connect( ui->searchWidget, SIGNAL( filterStringChanged(QString) ), m_proxy, SLOT( setFilterFixedString(QString) ) );
    connect( ui->searchWidget, SIGNAL( filterColumnChanged(int) ), m_proxy, SLOT( setFilterColumn(int) ) );

    connect( ui->replaceWidget, SIGNAL( transitionButtonClicked() ), this, SLOT( onSearchReplaceTransition() ) );

    connect( m_model, SIGNAL( dataChanged(QModelIndex,QModelIndex) ), this, SLOT( slotUpdateAccountInfo() ) );
    connect( m_model, SIGNAL( dataChanged(QModelIndex,QModelIndex) ), this, SIGNAL( changed() ) );


    loadConfig();

    slotUpdateAccountInfo();
}


AccountWidget::~AccountWidget()
{
    delete ui;
}


bool AccountWidget::isModified() const
{
    return account()->isModified();
}


QString AccountWidget::fileName() const
{
    return m_filename;
}


void AccountWidget::setFileName(const QString &name)
{
    m_filename = name;

    emit changed();
}


Account* AccountWidget::account()
{
    return m_model->account();
}


const Account* AccountWidget::account() const
{
    return m_model->account();
}


void AccountWidget::loadConfig()
{
    ui->view->horizontalHeader()->setMovable(
        Preferences::self()->value<bool>( "TableView", "MovableColumns", true )
    );

    ui->view->horizontalHeader()->setCascadingSectionResizes(
        Preferences::self()->value<bool>( "TableView", "CascadingSectionResize", false )
    );
}


void AccountWidget::saveConfig()
{
    Preferences::self()->setValue( "TableView", "HorizontalHeaderState",
                                    ui->view->horizontalHeader()->saveState() );
}


void AccountWidget::checkActionState()
{
    if( !isVisible() ) {
        return;
    }

    mainWindowActionCollection()->action( "file_save" )->setEnabled( isModified() );
    mainWindowActionCollection()->action( "file_save_as" )->setEnabled( true );
    mainWindowActionCollection()->action( "file_print" )->setEnabled( true );
    mainWindowActionCollection()->action( "file_close" )->setEnabled( true );

    mainWindowActionCollection()->action( "edit_undo" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_redo" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_cut" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_copy" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_paste" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_find" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_find_next" )->setEnabled( true );
    mainWindowActionCollection()->action( "edit_find_prev" )->setEnabled( true );

    mainWindowActionCollection()->action( "configure_account" )->setEnabled( true );
}


QList<const Posting*> AccountWidget::selectedPostings() const
{
    //TODO This is used by the export stuff for example...

    return QList<const Posting*>();
}


bool AccountWidget::onSaveFile()
{
    return onSaveAsFile( fileName() );
}


bool AccountWidget::onSaveAsFile(const QString &str)
{
    Q_UNUSED( str );

    QString filename = str.trimmed();
    if( filename.isEmpty() ) {
#if defined(HAVE_KDE)
        filename = KFileDialog::getSaveFileName( KUrl(), "*.ka|" + tr( "All Supported Files" ), this );
#else
        filename = QFileDialog::getSaveFileName( this, // krazy:exclude=qclasses
                     tr( "Save File - %1" ).arg( QCoreApplication::applicationName() ),
                     QString(), tr( "All Supported Files" ) + " (*.ka)"
                   );
#endif

        if( filename.isEmpty() ) {
            return false;
        }
    }

    if( Storage::writeAccount( this, account(), filename ) != QFile::NoError ) {
        return false;
    }

    setFileName( filename );
    account()->setModified( false );

    emit changed();

    return true;
}


void AccountWidget::onPrintFile()
{
    //TODO
}


void AccountWidget::onRedo()
{
    //TODO
}


void AccountWidget::onUndo()
{
    //TODO
}


void AccountWidget::onCut()
{
    //TODO
}


void AccountWidget::onCopy()
{
    //TODO
}


void AccountWidget::onPaste()
{
    //TODO
}


void AccountWidget::onFind()
{
    //TODO
    ui->replaceWidget ->setVisible( false );
    ui->searchWidget->setVisible( true );
}


void AccountWidget::onFindNext()
{
    //TODO
}


void AccountWidget::onFindPrev()
{
    //TODO
}


void AccountWidget::onReplace()
{
    //TODO
    ui->replaceWidget ->setVisible( true );
    ui->searchWidget->setVisible( false );
}


void AccountWidget::onPostingClone()
{
    //TODO
}


void AccountWidget::onPostingDelete()
{
    //TODO
}


void AccountWidget::onPostingValueDateToToday()
{
    //TODO
}


void AccountWidget::onPostingValueDateToMaturity()
{
    //TODO
}


void AccountWidget::onConfigureAccount()
{
    QPointer<AccountSettingsDialog> dialog = new AccountSettingsDialog( account(), this );

    if( dialog->exec() == QDialog::Accepted ) {
        slotUpdateAccountInfo();
        emit changed();
    }

    delete dialog;
}


void AccountWidget::onResizeColumnToContents(int index)
{
    if( Preferences::self()->value<bool>( "TableView", "DoubleClickResizeColumnToCountent", false ) ) {
        ui->view->resizeColumnToContents( index );
    }
}


void AccountWidget::onSearchReplaceTransition()
{
    if( ui->searchWidget->isVisible() ) {
        ui->searchWidget->setVisible( false );
        ui->replaceWidget->setVisible( true );
    }
    else {
        ui->searchWidget->setVisible( true );
        ui->replaceWidget->setVisible( false );
    }
}


void AccountWidget::slotSetIndexToCurrentInput()
{
    const QModelIndex index = m_proxy->mapFromSource( m_model->index( m_model->rowCount()-1, AccountModel::MATURITY ) );

    Q_ASSERT( index.isValid() );

    ui->view->selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect );
    ui->view->scrollTo( index, QAbstractItemView::PositionAtCenter );
}


void AccountWidget::slotUpdateAccountInfo()
{
    const QString &acname = account()->name().isEmpty() ? tr( "Unnamed Account" ) : account()->name();
    ui->accountName->setText( acname );

    const QModelIndex index = m_proxy->mapFromSource( m_model->index( m_model->rowCount()-1, AccountModel::BALANCE ) );
    Q_ASSERT( index.isValid() );
    const Money value = m_proxy->data( index, Qt::EditRole ).value<Money>();

    QString stylesheet;
    if( Preferences::self()->positiveAmountForegroundEnabled() && value >= 0.0 ) {
        stylesheet = QString::fromLatin1("color: %1;").arg( Preferences::self()->positiveAmountForegroundColor().name() );
    }
    else if( Preferences::self()->negativeAmountForegroundEnabled() && value < 0.0 ) {
        stylesheet = QString::fromLatin1("color: %1;").arg( Preferences::self()->negativeAmountForegroundColor().name() );
    }

    ui->balance->setStyleSheet( stylesheet );
    ui->balance->setText( formatMoney( value ) );
}


bool AccountWidget::eventFilter(QObject *obj, QEvent *event)
{
    if( event->type() == QEvent::ContextMenu ) {
        QContextMenuEvent *e = dynamic_cast<QContextMenuEvent*>(event);

        if( e ) {
            if( ui->view->horizontalHeader() && obj == ui->view->horizontalHeader() ) {
                showHeaderMenu( e, ui->view->indexAt( e->pos() ) );
                return true;
            }

            if( obj == ui->view ) {
                showTableMenu( e, ui->view->indexAt( ui->view->mapFromParent( e->pos() ) ) );
                return true;
            }
        }
    }

    return QWidget::eventFilter( obj, event );
}


#if defined(HAVE_KDE)
KActionCollection* AccountWidget::mainWindowActionCollection()
#else
ActionCollection* AccountWidget::mainWindowActionCollection()
#endif
{
    Q_ASSERT( m_mainWindow );

    return m_mainWindow->mainWindowActionCollection();
}


void AccountWidget::showHeaderMenu(QContextMenuEvent *e, const QModelIndex &index)
{
    QMenu menu( this );
    menu.setTitle( tr( "Columns" ) );

    QAction *hide = 0;
    if( index.isValid() ) {
        QModelIndex idx = m_proxy->mapToSource( index );
        Q_ASSERT( idx.isValid() );

        do {
            if( idx.column() == AccountModel::MATURITY ) {
                break;
            }
            if( idx.column() == AccountModel::POSTINGTEXT ) {
                break;
            }
            if( idx.column() == AccountModel::VALUEDATE ) {
                break;
            }
            if( idx.column() == AccountModel::AMOUNT ) {
                break;
            }

            hide = menu.addAction( tr( "Hide %1" ).arg(
                    ui->view->model()->headerData(
                        index.column(), Qt::Horizontal, Qt::DisplayRole ).toString()
                    )
                );

            // Only one column is visible, so this can't be hidden any more
            if( ui->view->horizontalHeader()->count() - 1
                    == ui->view->horizontalHeader()->hiddenSectionCount() ) {
                hide->setEnabled( false );
            }
        } while( false );
    }

    QActionGroup *showcolumns = new QActionGroup( this );
    QMenu *showcol = new QMenu( this );
    showcol->setTitle( tr( "Show Column" ) );

    for(int i = 0; i < ui->view->horizontalHeader()->count(); ++i) {
        if( ui->view->horizontalHeader()->isSectionHidden( i ) ) {
            showcolumns->addAction(
                    showcol->addAction(
                    ui->view->model()->headerData(
                        i, Qt::Horizontal, Qt::DisplayRole ).toString()
                ) )->setData( i );
        }
    }

    if( !ui->view->horizontalHeader()->hiddenSectionCount() ) {
        showcol->setEnabled( false );
    }

    menu.addMenu( showcol );
    menu.addSeparator();

    QAction *fitToWidth = menu.addAction( tr( "Fit to Width" ) );

    //Execute Menu and process results...
    QAction *action = menu.exec( e->globalPos() );
    if( action ) {
        if( hide && action == hide ) {
            ui->view->setColumnHidden( index.column(), true );
            saveConfig();
        }
        else if( action == fitToWidth ) {
            ui->view->resizeColumnToContents( index.column() );
            saveConfig();
        }
        else if( action->actionGroup() && action->actionGroup() == showcolumns ) {
            bool ok;
            int i = action->data().toInt( &ok );
            if( ok ) {
                ui->view->setColumnHidden( i, false );
                saveConfig();
            }
        }
    }
}


void AccountWidget::showTableMenu(QContextMenuEvent *e, const QModelIndex &index)
{
    Q_UNUSED( e );
    Q_UNUSED( index );

    QMenu *menu = new QMenu( this );
    menu->setAttribute( Qt::WA_DeleteOnClose );

    menu->addAction( mainWindowActionCollection()->action( "posting_clone" ) );
    menu->addSeparator();
    menu->addAction( mainWindowActionCollection()->action( "posting_delete" ) );
    menu->addSeparator();
    menu->addAction( mainWindowActionCollection()->action( "posting_valuedate_to_today" ) );
    menu->addAction( mainWindowActionCollection()->action( "posting_valuedate_to_maturity" ) );

    menu->popup( e->globalPos() );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
