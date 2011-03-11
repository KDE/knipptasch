/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * Highly inspired by Kate (katesavemodifieddialog.[h|cpp]
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
#include "savemodifieddialog.h"
#include "ui_savemodifieddialog.h"

#include "accountwidget.h"

#include "compat/iconloader.h"

#include "backend/account.h"

#if defined( HAVE_KDE )
#include <KFileDialog>
#include <KMessageBox>
#else
#include <QFileDialog>
#include <QMessageBox>
#endif

#include <QList>
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QCoreApplication>
#include <QApplication>



class AccountListItem : public QTreeWidgetItem
{
    public:
        enum AccountListItemState {
            InitialState,
            SaveOKState,
            SaveFailedState
        };

        AccountListItem( QTreeWidget *parent, AccountWidget *account );

        AccountListItemState state() const {
            return m_state;
        }

        void setState( AccountListItemState state );

        bool synchronousSave( QWidget *dialogParent );

    private:
        AccountWidget *m_accountWidget;
        AccountListItemState m_state;
};


AccountListItem::AccountListItem( QTreeWidget *parent, AccountWidget *account )
    : QTreeWidgetItem( parent ),
      m_accountWidget( account ),
      m_state( InitialState )
{
    Q_ASSERT( m_accountWidget );

    setFlags( flags() | Qt::ItemIsUserCheckable );

    setText( 0, m_accountWidget->account()->name().isEmpty()
             ? QObject::tr( "Unnamed Account" )
             : m_accountWidget->account()->name()
           );
    setText( 1, m_accountWidget->account()->number() );
    setText( 2, m_accountWidget->account()->owner() );
    setText( 3, m_accountWidget->fileName() );

    setCheckState( 0, Qt::Checked );

    setState( InitialState );
}


void AccountListItem::setState( AccountListItemState state )
{
    m_state = state;

    switch( m_state ) {
        case SaveOKState:
            setIcon( 0, BarIcon( "dialog-ok" ) );
            break;

        case SaveFailedState:
            setIcon( 0, BarIcon( "dialog-error" ) );
            break;

        default:
            setIcon( 0, QIcon() );
    }
}


bool AccountListItem::synchronousSave( QWidget *dialogParent )
{
    if( m_accountWidget->fileName().isEmpty() ) {
        const QString caption = QObject::tr( "Save As (%1)" ).arg( m_accountWidget->account()->name() );
        const QString filter = QObject::tr( "Knipptasch Account File (*.kta, *.kca)" );

        QString result =
#if defined(HAVE_KDE)
            KFileDialog::getSaveFileName( KUrl(), filter, dialogParent, caption,  KFileDialog::ConfirmOverwrite );
#else
            QFileDialog::getSaveFileName( dialogParent, caption, QString(), filter ); // krazy:exclude=qclasses
#endif

        if( !result.isEmpty() ) {
            if( !m_accountWidget->onSaveAsFile( result ) ) {
                setState( SaveFailedState );
                setText( 1, m_accountWidget->fileName() );

                return false;
            } else {
                setState( SaveOKState );
                setText( 1, m_accountWidget->fileName() );

                return true;
            }
        } else {
            setState( SaveFailedState );
            return false;
        }
    } else { //account has an exising location
        if( !m_accountWidget->onSaveFile() ) {
            setState( SaveFailedState );
            setText( 1, m_accountWidget->fileName() );
            return false;
        } else {
            setText( 1, m_accountWidget->fileName() );
            setState( SaveOKState );
            return true;
        }
    }

    return false;
}




SaveModifiedDialog::SaveModifiedDialog( QWidget *parent, QList<AccountWidget *> account )
    : QDialog( parent ),
      ui( new Ui::SaveModifiedDialog ),
      m_root( 0 ),
      m_doNotSaveButton( 0 ),
      m_doSaveSelectedButton( 0 ),
      m_doNotCloseButton( 0 )
{
    ui->setupUi( this );

    setWindowTitle( tr( "Save Account Files - %1" ).arg( QCoreApplication::applicationName() ) );

    QIcon windowIcon = DesktopIcon( QCoreApplication::applicationName().toLower() );
    if( windowIcon.isNull() ) {
        windowIcon = qApp->windowIcon();
    }
    ui->iconLabel->setPixmap( windowIcon.pixmap( 64, 64 ) );

    m_doNotSaveButton = new QPushButton( tr( "&Do Not Save" ) );
    m_doSaveSelectedButton = new QPushButton( tr( "&Save Selected" ) );
    m_doNotCloseButton = new QPushButton( tr( "Do &Not Close" ) );

    m_doSaveSelectedButton->setIcon( BarIcon( "document-save" ) );
    m_doNotCloseButton->setIcon( BarIcon( "dialog-cancel" ) );

    m_doNotCloseButton->setDefault( true );
    m_doNotCloseButton->setAutoDefault( true );

    ui->buttonBox->addButton( m_doNotSaveButton, QDialogButtonBox::AcceptRole );
    ui->buttonBox->addButton( m_doSaveSelectedButton, QDialogButtonBox::ApplyRole );
    ui->buttonBox->addButton( m_doNotCloseButton, QDialogButtonBox::RejectRole );

    connect( m_doSaveSelectedButton, SIGNAL( clicked( bool ) ), this, SLOT( slotSaveSelected() ) );
    connect( m_doNotSaveButton, SIGNAL( clicked( bool ) ), this, SLOT( accept() ) );
    connect( m_doNotCloseButton, SIGNAL( clicked( bool ) ), this, SLOT( reject() ) );

    ui->list->setColumnCount( 4 );
    ui->list->setHeaderLabels( QStringList() << tr( "Account Name" ) << tr( "Account Number" ) << tr( "Owner" ) << tr( "Location" ) );
    ui->list->setRootIsDecorated( false );

    Q_ASSERT( account.size() > 0 );
    foreach( AccountWidget * acc, account ) {
        m_items.append( new AccountListItem( ui->list, acc ) );
    }

    ui->list->resizeColumnToContents( 0 );

    connect( ui->list, SIGNAL( itemActivated( QTreeWidgetItem *, int ) ), SLOT( slotItemActivated( QTreeWidgetItem *, int ) ) );
    connect( ui->list, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ), SLOT( slotItemActivated( QTreeWidgetItem *, int ) ) );
    connect( ui->list, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), SLOT( slotItemActivated( QTreeWidgetItem *, int ) ) );
}


SaveModifiedDialog::~SaveModifiedDialog()
{
    delete ui;
}


void SaveModifiedDialog::slotItemActivated( QTreeWidgetItem *, int )
{
    foreach( AccountListItem * item, m_items ) {
        if( item->checkState( 0 ) == Qt::Checked ) {
            m_doSaveSelectedButton->setEnabled( true );
            return;
        }
    }

    m_doSaveSelectedButton->setEnabled( false );
}


void SaveModifiedDialog::slotSaveSelected()
{
    foreach( AccountListItem * item, m_items ) {
        if( item->checkState( 0 ) == Qt::Checked && ( item->state() != AccountListItem::SaveOKState ) ) {
            if( !item->synchronousSave( this ) ) {
                const QString msg = tr( "Data you requested to be saved could not be written. Please choose how you want to proceed." );
#if defined( HAVE_KDE )
                KMessageBox::sorry( this, msg );
#else
                QMessageBox::information( // krazy:exclude=qclasses
                    this,
                    tr( "Sorry - %1" ).arg( QCoreApplication::applicationName() ),
                    msg,
                    QMessageBox::Ok,
                    QMessageBox::Ok
                );
#endif
                return;
            }
        } else if(( item->checkState( 0 ) != Qt::Checked ) && ( item->state() == AccountListItem::SaveFailedState ) ) {
            item->setState( AccountListItem::InitialState );
        }
    }

    done( QDialog::Accepted );
}


bool SaveModifiedDialog::queryClose( QWidget *parent, AccountWidget *account )
{
    Q_ASSERT( account );

    if( !account->isModified() ) {
        return true;
    }

    QString fname = account->fileName().isEmpty() ? tr( "Untitled" ) : account->fileName();
    QString msg = tr( "<qt>The file \"%1\" has been modified.<br/>\nDo you want "
                      "to save your changes or discard them?</qt>" ).arg( fname );

#if defined( HAVE_KDE )
    {
        int result = KMessageBox::questionYesNoCancel(
                         parent, msg, tr( "Close File" ), KStandardGuiItem::save(),
                         KStandardGuiItem::discard(), KStandardGuiItem::cancel() );

        if( result == KMessageBox::Yes ) {
            if( account->onSaveFile() ) {
                return true;
            }
        } else if( result == KMessageBox::No ) {
            return true;
        }
    }
#else
    {
        QMessageBox::StandardButton result = QMessageBox::question(  // krazy:exclude=qclasses
                parent,
                tr( "Close File - %1" ).arg( QCoreApplication::applicationName() ),
                msg,
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                QMessageBox::Save
                                             );

        if( result == QMessageBox::Save ) {
            if( account->onSaveFile() ) {
                return true;
            }
        } else if( result == QMessageBox::Discard ) {
            return true;
        }
    }
#endif

    return false;
}


bool SaveModifiedDialog::queryClose( QWidget *parent, QList<AccountWidget *> accountWidgetList )
{
    QList<AccountWidget *> modifiedList;
    foreach( AccountWidget * widget, accountWidgetList ) {
        if( widget->isModified() ) {
            modifiedList.append( widget );
        }
    }

    if( modifiedList.isEmpty() ) {
        return true;
    }

    if( modifiedList.size() == 1 ) {
        return queryClose( parent, modifiedList.first() );
    }

    SaveModifiedDialog d( parent, modifiedList );

    return d.exec() != QDialog::Rejected;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

