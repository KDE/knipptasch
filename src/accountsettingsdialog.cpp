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
#include "accountsettingsdialog.h"
#include "ui_accountsettingsdialog.h"

#include "passwordwidget.h"
#include "categorymodel.h"

#include "compat/iconloader.h"

#include "backend/account.h"
#include "backend/category.h"
#include "backend/money.h"

#include <modeltest/modeltest.h>

#include <QPushButton>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>

#include <QDebug>

#include <KColorCombo>
#include <QSortFilterProxyModel>
#include <QMenu>


class ColorDelegate : public QStyledItemDelegate
{
    public:
        ColorDelegate(QObject *parent = 0)
          : QStyledItemDelegate( parent )
        {
        }


        QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
            if( qVariantCanConvert<QColor>( index.data( Qt::EditRole ) ) ) {
                KColorCombo *combo = new KColorCombo( parent );

                return combo;
            }

            return QStyledItemDelegate::createEditor( parent, option, index );
        }

        void setEditorData(QWidget *widget, const QModelIndex &index) const
        {
            if( qVariantCanConvert<QColor>( index.data( Qt::EditRole ) ) ) {
                KColorCombo *editor = qobject_cast<KColorCombo*>( widget );
                Q_ASSERT( editor );

                editor->setColor( qVariantValue<QColor>( index.data( Qt::EditRole ) ) );
            }
            else {
                QStyledItemDelegate::setEditorData( widget, index );
            }
        }

        void setModelData(QWidget *widget, QAbstractItemModel *model, const QModelIndex &index) const
        {
            if( qVariantCanConvert<QColor>( index.data( Qt::EditRole ) ) ) {
                KColorCombo *editor = qobject_cast<KColorCombo*>( widget );
                Q_ASSERT( editor );

                model->setData( index, qVariantFromValue( editor->color() ) );
            }
            else {
                QStyledItemDelegate::setModelData( widget, model, index );
            }
        }

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
           QStyledItemDelegate::paint( painter, option, index );

            if( !index.isValid() ) {
                return;
            }

            if( qVariantCanConvert<QColor>( index.data( Qt::EditRole ) ) ) {
                QColor color = qVariantValue<QColor>( index.data( Qt::EditRole ) );

                if( color.isValid() ) {
                    painter->save();
                    painter->setRenderHint( QPainter::Antialiasing );
                    painter->setPen( QPen( Qt::black, 1 ) );
                    painter->setBrush( color );

                    QRect r = option.rect.adjusted( 3, 3, -3, -3 );
                    r.setWidth( r.height() );
                    painter->drawRect( r );

                    painter->restore();
                }
            }
        }

        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
            Q_UNUSED(index)

            return QSize( option.fontMetrics.height() + 16,
                          option.fontMetrics.height() + 3 );
        }
};






AccountSettingsDialog::AccountSettingsDialog(Account *account, QWidget* parent)
  : QDialog( parent ),
    ui( new Ui::AccountSettingsDialog ),
    m_passwordWidget( 0 ),
    m_model( new CategoryModel( this ) )
{
    ui->setupUi( this );

    new ModelTest( m_model, this );

    //TODO implement the account and category limit stuff
    ui->accountLimitGroupBox->setVisible( false );

    setWindowTitle( tr( "Configure Account - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->iconLabel->setPixmap( DesktopIcon("view-bank-account") );

    Q_ASSERT( ui->tabWidget->count() >= 2 );
    ui->tabWidget->setTabIcon( 0, BarIcon( "view-bank-account" ) );
    ui->tabWidget->setTabIcon( 1, BarIcon( "view-categories" ) );

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( m_model );
    new ModelTest( proxyModel, this );

    ui->categoriesTreeView->setModel( proxyModel );
    ui->categoriesTreeView->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->categoriesTreeView->setSortingEnabled( true );
    ui->categoriesTreeView->sortByColumn( 0, Qt::AscendingOrder );
    ui->categoriesTreeView->setRootIsDecorated( true );
    ui->categoriesTreeView->setAnimated( true );
    ui->categoriesTreeView->setUniformRowHeights( true );

    //TODO implement Drag & Drop
    //ui->categoriesTreeView->setDragEnabled( true );
    //ui->categoriesTreeView->setAcceptDrops( true );
    //ui->categoriesTreeView->setDragDropMode( QAbstractItemView::InternalMove );
    //ui->categoriesTreeView->setDropIndicatorShown( true );

    ui->categoriesTreeView->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->categoriesTreeView->setItemDelegateForColumn(
                                CategoryModel::CATEGORY_COLOR,
                                new ColorDelegate( this ) );

    ui->addCategoryButton->setIcon( BarIcon( "list-add" ) );
    ui->addSubCategoryButton->setIcon( BarIcon( "insert-horizontal-rule" ) );
    ui->importCategoryButton->setIcon( BarIcon( "document-import" ) );
    ui->exportCategoryButton->setIcon( BarIcon( "document-export" ) );
    ui->removeCategoryButton->setIcon( BarIcon( "edit-delete" ) );


    /*TODO implement category import */ ui->importCategoryButton->setVisible( false );
    /*TODO implement category export */ ui->exportCategoryButton->setVisible( false );

#if defined( WITH_QCA2 )
    m_passwordWidget = new PasswordWidget( this );
    int index = ui->tabWidget->addTab( m_passwordWidget, BarIcon("dialog-password"), tr( "Password Protection" )  );

    if( !QCA::isSupported( "aes256-cbc-pkcs7" ) ) {
        ui->tabWidget->setTabEnabled( index, false );
    }
#endif

    setAccount( account );

    connect( ui->name, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->institution, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->bic, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->owner, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->accountNumber, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->iban, SIGNAL( textChanged(QString) ), this, SLOT( onValueChanged() ) );
    connect( ui->openingDate, SIGNAL( dateChanged(QDate) ), this, SLOT( onValueChanged() ) );
    connect( ui->openingBalance, SIGNAL( valueChanged(double) ), this, SLOT( onValueChanged() ) );
    connect( ui->useInLimit, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->inLimit, SIGNAL( valueChanged(double) ), this, SLOT( onValueChanged() ) );
    connect( ui->useOutLimit, SIGNAL( stateChanged(int) ), this, SLOT( onValueChanged() ) );
    connect( ui->outLimit, SIGNAL( valueChanged(double) ), this, SLOT( onValueChanged() ) );

    connect( ui->categoriesTreeView->selectionModel(), SIGNAL( currentRowChanged(QModelIndex,QModelIndex) ), this, SLOT( onCategoryChanged() ) );

    connect( ui->categoriesTreeView, SIGNAL( customContextMenuRequested(QPoint) ), this, SLOT( onContextMenu(QPoint) ) );

    connect( ui->categoriesTreeView->model(), SIGNAL( rowsInserted(const QModelIndex&, int, int) ), this, SLOT( onCategoryChanged() ) );
    connect( ui->categoriesTreeView->model(), SIGNAL( rowsInserted(const QModelIndex&, int, int) ), this, SLOT( selectNewCategory(const QModelIndex&, int) ) );
    connect( ui->categoriesTreeView->model(), SIGNAL( rowsRemoved(const QModelIndex&, int, int) ), this, SLOT( onCategoryChanged() ) );

    connect( ui->addCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( addCategoryClicked() ) );
    connect( ui->addSubCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( addSubCategoryClicked() ) );
    connect( ui->importCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( importCategoryClicked() ) );
    connect( ui->exportCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( exportCategoryClicked() ) );
    connect( ui->removeCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( removeCategoryClicked() ) );

    connect( ui->buttonBox, SIGNAL( accepted() ), this, SLOT( onApplyChanges() ) );
    connect( ui->buttonBox->button( QDialogButtonBox::Apply ), SIGNAL( clicked(bool) ), this, SLOT( onApplyChanges() ) );

#if defined( WITH_QCA2 )
    connect( m_passwordWidget, SIGNAL( valueChanged() ), this, SLOT( onValueChanged() ) );
#endif

    ui->categoriesTreeView->expandToDepth( 1 );
    ui->categoriesTreeView->resizeColumnToContents( 0 );
    ui->categoriesTreeView->setCurrentIndex( ui->categoriesTreeView->model()->index( 0, 0 ) );

    onValueChanged();
}


AccountSettingsDialog::~AccountSettingsDialog()
{
    delete ui;
    delete m_passwordWidget;
}


Account* AccountSettingsDialog::account()
{
    return m_model->account();
}


const Account* AccountSettingsDialog::account() const
{
    return m_model->account();
}


void AccountSettingsDialog::setAccount(Account *acc)
{
    m_model->setAccount( acc );

    ui->tabWidget->setCurrentIndex( 0 );
    ui->tabWidget->setEnabled( acc );

    ui->name->setEnabled( acc );
    ui->institution->setEnabled( acc );
    ui->bic->setEnabled( acc );
    ui->owner->setEnabled( acc );
    ui->accountNumber->setEnabled( acc );
    ui->iban->setEnabled( acc );
    ui->openingDate->setEnabled( acc );
    ui->openingBalance->setEnabled( acc );
    ui->useInLimit->setEnabled( acc );
    ui->inLimit->setEnabled( acc );
    ui->useOutLimit->setEnabled( acc );
    ui->outLimit->setEnabled( acc );

    ui->removeCategoryButton->setEnabled( false );
    ui->exportCategoryButton->setEnabled( false );

    if( acc ) {
        ui->name->setText( acc->name()  );
        ui->institution->setText( acc->institution() );
        ui->bic->setText( acc->bic() );
        ui->owner->setText( acc->owner() );
        ui->accountNumber->setText( acc->number() );
        ui->iban->setText( acc->iban() );
        ui->openingDate->setDate( acc->openingDate() );
        ui->openingBalance->setValue( acc->openingBalance() );
        ui->useInLimit->setChecked( acc->maximumBalanceEnabled() );
        ui->inLimit->setValue( acc->maximumBalance() );
        ui->inLimit->setEnabled( acc->maximumBalanceEnabled() );
        ui->useOutLimit->setChecked( acc->minimumBalanceEnabled() );
        ui->outLimit->setValue( acc->minimumBalance() );
        ui->outLimit->setEnabled( acc->minimumBalanceEnabled() );
    }

    if( m_passwordWidget ) {
        m_passwordWidget->setAccount( acc );
    }

    onCategoryChanged();
}


void AccountSettingsDialog::onValueChanged()
{
    if( m_passwordWidget && !m_passwordWidget->isValid() ) {
        ui->tabWidget->setCurrentIndex( ui->tabWidget->indexOf( m_passwordWidget ) );

        ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );

        return;
    }

    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
    ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( true );

    do {
        if( m_passwordWidget && m_passwordWidget->isModified() ) {
            break;
        }

        if( ui->name->text().trimmed() != account()->name().trimmed() ) {
            break;
        }

        if( ui->institution->text().trimmed() != account()->institution().trimmed() ) {
            break;
        }

        if( ui->bic->text().trimmed() != account()->bic().trimmed() ) {
            break;
        }

        if( ui->owner->text().trimmed() != account()->owner().trimmed() ) {
            break;
        }

        if( ui->accountNumber->text().trimmed() != account()->number().trimmed() ) {
            break;
        }

        if( ui->iban->text().trimmed() != account()->iban().trimmed() ) {
            break;
        }

        if( ui->openingDate->date() != account()->openingDate() ) {
            break;
        }

        if( ui->openingBalance->value() != account()->openingBalance() ) {
            break;
        }

        if( ui->useInLimit->isChecked() != account()->maximumBalanceEnabled() ) {
            break;
        }

        if( ui->inLimit->value() != account()->maximumBalance() ) {
            break;
        }

        if( ui->useOutLimit->isChecked() != account()->minimumBalanceEnabled() ) {
            break;
        }

        if( ui->outLimit->value() != account()->minimumBalance() ) {
            break;
        }

        ui->buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    } while( false );
}


void AccountSettingsDialog::onApplyChanges()
{
    if( m_passwordWidget ) {
        m_passwordWidget->onApplyChanges();
    }

    Account *acc = account();

    if( acc ) {
        acc->setName( ui->name->text().trimmed() );
        acc->setInstitution( ui->institution->text().trimmed() );
        acc->setBic( ui->bic->text().trimmed() );
        acc->setOwner( ui->owner->text().trimmed() );
        acc->setNumber( ui->accountNumber->text().trimmed() );
        acc->setIban( ui->iban->text().trimmed() );
        acc->setOpeningDate( ui->openingDate->date() );
        acc->setOpeningBalance( ui->openingBalance->value() );
        acc->setMaximumBalanceEnabled( ui->useInLimit->isChecked() );
        acc->setMaximumBalance( ui->inLimit->value() );
        acc->setMinimumBalanceEnabled( ui->useOutLimit->isChecked() );
        acc->setMinimumBalance( ui->outLimit->value() );
    }

    onValueChanged();
}


void AccountSettingsDialog::onContextMenu(const QPoint &point)
{
    Q_UNUSED( point );
/*
    TODO implement context menu for the category tree view

    QMenu menu;

    QAction *add = menu.addAction( BarIcon( "list-add" ), tr( "Add category" ) );
    QAction *insert = menu.addAction( BarIcon( "insert-horizontal-rule" ), tr( "Add sub-category" ) );
    menu.addSeparator();
    QAction *im = menu.addAction( BarIcon( "document-import" ), tr( "Import categories" ) );
    QAction *ex = menu.addAction( BarIcon( "document-export" ), tr( "Export categories" ) );
    menu.addSeparator();
    QAction *remove = menu.addAction( BarIcon( "edit-delete" ), tr( "Delete category" ) );

    menu.exec( ui->categoriesTreeView->mapToGlobal( point ) );
*/
}


void AccountSettingsDialog::onCategoryChanged()
{
    QModelIndex index = ui->categoriesTreeView->currentIndex();
    if( index.isValid() ) {
        ui->addSubCategoryButton->setEnabled( true );
        ui->removeCategoryButton->setEnabled( !index.child( 0, 0 ).isValid() );
    }
    else {
        ui->addSubCategoryButton->setEnabled( false );
        ui->removeCategoryButton->setEnabled( false );
    }
}


void AccountSettingsDialog::addCategoryClicked()
{
    addCategory( false );
}


void AccountSettingsDialog::addSubCategoryClicked()
{
    addCategory( true );
}


void AccountSettingsDialog::importCategoryClicked()
{
    //TODO implement category import
}


void AccountSettingsDialog::exportCategoryClicked()
{
    //TODO implement category export
}


void AccountSettingsDialog::removeCategoryClicked()
{
    QModelIndex index = ui->categoriesTreeView->currentIndex();
    Q_ASSERT( index.isValid() );

    ui->categoriesTreeView->model()->removeRow( index.row(), index.parent() );
}


void AccountSettingsDialog::selectNewCategory(const QModelIndex &parent, int start)
{
    ui->categoriesTreeView->setCurrentIndex( ui->categoriesTreeView->model()->index( start, 0, parent ) );
    ui->categoriesTreeView->scrollTo( ui->categoriesTreeView->currentIndex() );
}


void AccountSettingsDialog::addCategory(bool belowOfCurrent)
{
    QModelIndex index = ui->categoriesTreeView->currentIndex();
    QModelIndex parent = belowOfCurrent ? index : index.parent();

    int row = 0;
    if( index.isValid() && index.row() ) {
        row = index.row() + 1;
    }

    ui->categoriesTreeView->model()->insertRow( 0, parent );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
