/*
 * Copyright 2010,2011 by Stefan Böhmann <kde@hilefoks.org>
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
#include "accountcategoryconfigpage.h"

#include "ui_accountcategoryconfigpage.h"

#include "categorymodel.h"

#include "compat/iconloader.h"

#include <modeltest/modeltest.h>

#include <QStyledItemDelegate>
#include <QPainter>

#include <KColorCombo>
#include <QSortFilterProxyModel>



class CategoryColorDelegate : public QStyledItemDelegate
{
    public:
        CategoryColorDelegate(QObject *parent = 0)
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
                    r.setWidth( qMin( 5 * r.height(), r.width() ) );
                    painter->drawRect( r );

                    painter->restore();
                }
            }
        }

        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
            Q_UNUSED(index)

            return QSize( 4 * option.fontMetrics.height(),
                          option.fontMetrics.height() + 3 );
        }
};




AccountCategoryConfigPage::AccountCategoryConfigPage(Account *account, ConfigWidget* parent)
  : AbstractConfigPage( tr( "Categories" ), DesktopIcon("view-categories"), parent ),
    ui( new Ui::AccountCategoryConfigPage ),
    m_account( account ),
    m_model( new CategoryModel( this ) )
{
    ui->setupUi( this );
    new ModelTest( m_model, this );

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( m_model );
    new ModelTest( proxyModel, this );

    ui->view->setModel( proxyModel );
    ui->view->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->view->setSortingEnabled( true );
    ui->view->sortByColumn( 0, Qt::AscendingOrder );
    ui->view->setRootIsDecorated( true );
    ui->view->setAnimated( true );
    ui->view->setUniformRowHeights( true );

    //TODO implement Drag & Drop
    //ui->view->setDragEnabled( true );
    //ui->view->setAcceptDrops( true );
    //ui->view->setDragDropMode( QAbstractItemView::InternalMove );
    //ui->view->setDropIndicatorShown( true );

    ui->view->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->view->setItemDelegateForColumn(
                     CategoryModel::CATEGORY_COLOR,
                     new CategoryColorDelegate( this ) );

    ui->addCategoryButton->setIcon( BarIcon( "list-add" ) );
    ui->addSubCategoryButton->setIcon( BarIcon( "insert-horizontal-rule" ) );
    ui->importCategoryButton->setIcon( BarIcon( "document-import" ) );
    ui->exportCategoryButton->setIcon( BarIcon( "document-export" ) );
    ui->removeCategoryButton->setIcon( BarIcon( "edit-delete" ) );

    /* TODO implement category import */
    ui->importCategoryButton->setVisible( false );
    /* TODO implement category export */
    ui->exportCategoryButton->setVisible( false );

    connect( ui->view->selectionModel(), SIGNAL( currentRowChanged(QModelIndex,QModelIndex) ), this, SLOT( onValueChanged() ) );

    connect( ui->view->model(), SIGNAL( rowsInserted(const QModelIndex&, int, int) ), this, SLOT( onValueChanged() ) );
    connect( ui->view->model(), SIGNAL( rowsInserted(const QModelIndex&, int, int) ), this, SLOT( selectNewCategory(const QModelIndex&, int) ) );
    connect( ui->view->model(), SIGNAL( rowsRemoved(const QModelIndex&, int, int) ), this, SLOT( onValueChanged() ) );

    connect( ui->addCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( addCategoryClicked() ) );
    connect( ui->addSubCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( addSubCategoryClicked() ) );
    connect( ui->importCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( importCategoryClicked() ) );
    connect( ui->exportCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( exportCategoryClicked() ) );
    connect( ui->removeCategoryButton, SIGNAL( clicked(bool) ), this, SLOT( removeCategoryClicked() ) );

    revert();
}


AccountCategoryConfigPage::~AccountCategoryConfigPage()
{
    delete ui;
}


bool AccountCategoryConfigPage::isModified() const
{
    return false;
}


bool AccountCategoryConfigPage::commit()
{
    return true;
}


void AccountCategoryConfigPage::revert()
{
    m_model->setAccount( m_account );

    ui->view->expandToDepth( 1 );
    ui->view->resizeColumnToContents( 0 );
    ui->view->setCurrentIndex( ui->view->model()->index( 0, 0 ) );

    onValueChanged();
}


void AccountCategoryConfigPage::onValueChanged()
{
    QModelIndex index = ui->view->currentIndex();
    if( index.isValid() ) {
        ui->addSubCategoryButton->setEnabled( true );
        ui->removeCategoryButton->setEnabled( !index.child( 0, 0 ).isValid() );
    }
    else {
        ui->addSubCategoryButton->setEnabled( false );
        ui->removeCategoryButton->setEnabled( false );
    }
}


void AccountCategoryConfigPage::addCategoryClicked()
{
    addCategory( false );
}


void AccountCategoryConfigPage::addSubCategoryClicked()
{
    addCategory( true );
}


void AccountCategoryConfigPage::importCategoryClicked()
{
    //TODO implement category import
}


void AccountCategoryConfigPage::exportCategoryClicked()
{
    //TODO implement category export
}


void AccountCategoryConfigPage::removeCategoryClicked()
{
    QModelIndex index = ui->view->currentIndex();
    Q_ASSERT( index.isValid() );

    ui->view->model()->removeRow( index.row(), index.parent() );
}


void AccountCategoryConfigPage::selectNewCategory(const QModelIndex &parent, int start)
{
    ui->view->setCurrentIndex( ui->view->model()->index( start, 0, parent ) );
    ui->view->scrollTo( ui->view->currentIndex() );
}


void AccountCategoryConfigPage::addCategory(bool belowOfCurrent)
{
    QModelIndex index = ui->view->currentIndex();
    QModelIndex parent = belowOfCurrent ? index : index.parent();

    int row = 0;
    if( index.isValid() && index.row() ) {
        row = index.row() + 1;
    }

    ui->view->model()->insertRow( 0, parent );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
