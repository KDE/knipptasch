/*
 * Copyright 2011  Stefan Böhmann <kde@hilefoks.org>
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
#include "configwidget.h"
#include "ui_configwidget.h"

#include "abstractconfigpage.h"

#include "compat/iconloader.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSplitter>

#include <QDebug>
#include <QStyledItemDelegate>



/**
 *
 */
class ListWidgetDelegate : public QStyledItemDelegate
{
    public:
        ListWidgetDelegate( ConfigWidget *parent )
            : QStyledItemDelegate( parent ),
              m_configWidget( parent ) {
        }


        void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const {
            if( !index.isValid() || !index.internalPointer() ) {
                return;
            }

            QStyleOptionViewItemV4 opt( *static_cast<const QStyleOptionViewItemV4 *>( &option ) );
            opt.decorationPosition = QStyleOptionViewItem::Top;
            opt.decorationSize = m_configWidget->iconSize();
            opt.textElideMode = Qt::ElideNone;

            QStyledItemDelegate::paint( painter, opt, index );
        }


        QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const {
            if( !index.isValid() || !index.internalPointer() ) {
                return QSize();
            }

            QStyleOptionViewItemV4 opt( *static_cast<const QStyleOptionViewItemV4 *>( &option ) );
            opt.decorationPosition = QStyleOptionViewItem::Top;
            opt.decorationSize = m_configWidget->iconSize();
            opt.textElideMode = Qt::ElideNone;

            return QStyledItemDelegate::sizeHint( opt, index );
        }

    private:
        ConfigWidget *m_configWidget;
};



ConfigWidget::ConfigWidget( QWidget *parent )
    : QWidget( parent ),
      ui( new Ui::ConfigWidget )
{
    ui->setupUi( this );

    ui->view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->view->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    ui->view->setSelectionMode( QAbstractItemView::SingleSelection );
    ui->view->setEditTriggers( QAbstractItemView::NoEditTriggers );
    ui->view->setItemDelegate( new ListWidgetDelegate( this ) );
    ui->view->setTabKeyNavigation( true );
    ui->view->setIconSize( QSize( 48, 48 ) );


    ui->errorWidget->setVisible( false );


    connect( ui->view, SIGNAL( currentRowChanged( int ) ), this, SLOT( setCurrentIndex( int ) ) );
    connect( ui->stackedWidget, SIGNAL( currentChanged( int ) ), this, SIGNAL( currentIndexChanged( int ) ) );
}


ConfigWidget::~ConfigWidget()
{
}


bool ConfigWidget::isModified() const
{
    for( int i = 0; i < countPages(); ++i ) {
        if( page( i )->isModified() ) {
            return true;
        }
    }

    return false;
}


bool ConfigWidget::isValid() const
{
    for( int i = 0; i < countPages(); ++i ) {
        if( !page( i )->isValid() ) {
            return false;
        }
    }

    return true;
}


QSize ConfigWidget::iconSize() const
{
    return ui->view->iconSize();
}


void ConfigWidget::setIconSize( const QSize &size )
{
    ui->view->setIconSize( size );
}


int ConfigWidget::addPage( AbstractConfigPage *page )
{
    return insertPage( -1, page );
}


int ConfigWidget::insertPage( int index, AbstractConfigPage *page )
{
    if( !page ) {
        qWarning() << "ConfigWidget::insertPage(): Attempt to insert null page";
        return -1;
    }

    index = ui->stackedWidget->insertWidget( index, page );

    connect( page, SIGNAL( pageModified() ), this, SLOT( onPageModified() ) );
    connect( page, SIGNAL( errorMessageChanged() ), this, SLOT( onErrorMessageChanged() ) );

    QListWidgetItem *item = new QListWidgetItem( ui->view );
    item->setText( page->pageLabel() );
    item->setTextAlignment( Qt::AlignCenter );
    item->setIcon( page->pageIcon() );
    item->setToolTip( page->pageToolTip() );
    item->setWhatsThis( page->pageWhatsThis() );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    setCurrentIndex( 0 );
    ui->view->updateGeometry();

    emit pageModified( page );

    return index;
}


AbstractConfigPage *ConfigWidget::takePage( int index )
{
    QWidget *p = ui->stackedWidget->widget( index );

    if( p ) {
        AbstractConfigPage *page = qobject_cast<AbstractConfigPage *>( p );
        Q_ASSERT( page );

        disconnect( page, 0, this, 0 );

        ui->stackedWidget->removeWidget( page );
        delete ui->view->item( index );

        emit pageModified( page );

        return page;
    }

    qWarning() << "ConfigWidget::removePage(): Unknown index";
    return 0;
}


int ConfigWidget::countPages() const
{
    return ui->stackedWidget->count();
}


int ConfigWidget::currentIndex() const
{
    return ui->stackedWidget->currentIndex();
}


AbstractConfigPage *ConfigWidget::currentPage() const
{
    return qobject_cast<AbstractConfigPage *>( ui->stackedWidget->currentWidget() );
}


int ConfigWidget::indexOf( AbstractConfigPage *page ) const
{
    return ui->stackedWidget->indexOf( page );
}


AbstractConfigPage *ConfigWidget::page( int index ) const
{
    return qobject_cast<AbstractConfigPage *>( ui->stackedWidget->widget( index ) );
}


void ConfigWidget::setCurrentIndex( int index )
{
    AbstractConfigPage *p = page( index );

    ui->stackedWidget->setCurrentIndex( index );
    ui->view->setCurrentItem( ui->view->item( index ) );

    ui->pageIcon->setPixmap( p->pageIcon().pixmap( 48, 48 ) );
    ui->pageTitle->setText( p->pageTitle() );
    ui->pageText->setText( p->pageDescription() );
    ui->pageText->setVisible( !p->pageDescription().isEmpty() );

    onErrorMessageChanged();
}


void ConfigWidget::setCurrentPage( AbstractConfigPage *page )
{
    setCurrentIndex( ui->stackedWidget->indexOf( page ) );
}


bool ConfigWidget::commit()
{
    bool b = false;

    Q_ASSERT( ui->stackedWidget );
    for( int i = 0; i < ui->stackedWidget->count(); ++i ) {
        Q_ASSERT( ui->stackedWidget->widget( i ) );
        AbstractConfigPage *page = qobject_cast<AbstractConfigPage *>( ui->stackedWidget->widget( i ) );
        Q_ASSERT( page );

        b = page->commit() || b;
    }

    emit committed();

    return b;
}


void ConfigWidget::revert()
{
    Q_ASSERT( ui->stackedWidget );
    for( int i = 0; i < ui->stackedWidget->count(); ++i ) {
        Q_ASSERT( ui->stackedWidget->widget( i ) );
        AbstractConfigPage *page = qobject_cast<AbstractConfigPage *>( ui->stackedWidget->widget( i ) );
        Q_ASSERT( page );

        page->revert();
    }

    emit reverted();
}


QListWidget *ConfigWidget::listWidget() const
{
    return ui->view;
}


QStackedWidget *ConfigWidget::stackedWidget() const
{
    return ui->stackedWidget;
}


void ConfigWidget::onPageModified()
{
    AbstractConfigPage *page = qobject_cast<AbstractConfigPage *>( sender() );
    Q_ASSERT( page );

    emit pageModified( page );
}


void ConfigWidget::onErrorMessageChanged()
{
    AbstractConfigPage *p = currentPage();

    ui->errorTitle->setText( p->errorMessageTitle() );
    ui->errorText->setText( p->errorMessageDescription() );
    switch( p->errorMessageType() ) {
        case AbstractConfigPage::InfoMessage:
            ui->errorIcon->setPixmap( DesktopIcon( "dialog-information" ) );
            break;

        case AbstractConfigPage::WarningMessage:
            ui->errorIcon->setPixmap( DesktopIcon( "dialog-warning" ) );
            break;

        case AbstractConfigPage::ErrorMessage:
            ui->errorIcon->setPixmap( DesktopIcon( "dialog-error" ) );
            break;
    }

    ui->errorWidget->setVisible( p->errorMessageEnabled() );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
