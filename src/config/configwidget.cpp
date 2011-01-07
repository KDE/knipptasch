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
#include "abstractconfigpage.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSplitter>

#include <QDebug>



ConfigWidget::ConfigWidget(QWidget* parent)
  : QWidget( parent ),
    m_splitter( new QSplitter( this ) ),
    m_stack( new QStackedWidget( this ) ),
    m_view( new QListWidget( this ) )
{
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    m_view->setSelectionMode( QAbstractItemView::SingleSelection );
    m_view->setEditTriggers( QAbstractItemView::NoEditTriggers );
    m_view->setTabKeyNavigation( true );
    m_view->setIconSize( QSize( 48, 48 ) );

    m_splitter->setOrientation( Qt::Horizontal );
    m_splitter->addWidget( m_view );
    m_splitter->addWidget( m_stack );

    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->addWidget( m_splitter );
    setLayout( layout );

    connect( m_view, SIGNAL( currentRowChanged(int) ), this, SLOT( setCurrentIndex(int) ) );
    connect( m_stack, SIGNAL( currentChanged(int) ), this, SIGNAL( currentIndexChanged(int) ) );
}


ConfigWidget::~ConfigWidget()
{
}


bool ConfigWidget::isModified() const
{
    for(int i = 0; i < countPages(); ++i) {
        if( page( i )->isModified() ) {
            return true;
        }
    }

    return false;
}


bool ConfigWidget::isValid() const
{
    for(int i = 0; i < countPages(); ++i) {
        if( !page( i )->isValid() ) {
            return false;
        }
    }

    return true;
}


QSize ConfigWidget::iconSize() const
{
    return m_view->iconSize();
}


void ConfigWidget::setIconSize(const QSize& size)
{
    m_view->setIconSize( size );
}


int ConfigWidget::addPage(AbstractConfigPage *page)
{
    return insertPage( -1, page );
}


int ConfigWidget::insertPage(int index, AbstractConfigPage *page)
{
    if( !page ) {
        qWarning() << "ConfigWidget::insertPage(): Attempt to insert null page";
        return -1;
    }

    index = m_stack->insertWidget( index, page );

    connect( page, SIGNAL( pageModified() ), this, SLOT( onPageModified() ) );

    QListWidgetItem *item = new QListWidgetItem( m_view );
    item->setText( page->pageTitle() );
    item->setTextAlignment( Qt::AlignVCenter );
    item->setIcon( page->pageIcon() );
    item->setToolTip( page->pageToolTip() );
    item->setWhatsThis( page->pageWhatsThis() );
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    setCurrentIndex( 0 );
    m_view->updateGeometry();

    emit pageModified( page );

    return index;
}


AbstractConfigPage* ConfigWidget::takePage(int index)
{
    QWidget *p = m_stack->widget( index );

    if( p ) {
        AbstractConfigPage *page = qobject_cast<AbstractConfigPage*>( p );
        Q_ASSERT( page );

        disconnect( page, 0, this, 0 );

        m_stack->removeWidget( page );
        delete m_view->item( index );

        emit pageModified( page );

        return page;
    }

    qWarning() << "ConfigWidget::removePage(): Unknown index";
    return 0;
}


int ConfigWidget::countPages() const
{
    return m_stack->count();
}


int ConfigWidget::currentIndex() const
{
    return m_stack->currentIndex();
}


AbstractConfigPage* ConfigWidget::currentPage() const
{
    return qobject_cast<AbstractConfigPage*>( m_stack->currentWidget() );
}


int ConfigWidget::indexOf(AbstractConfigPage* page) const
{
    return m_stack->indexOf( page );
}


AbstractConfigPage* ConfigWidget::page(int index) const
{
    return qobject_cast<AbstractConfigPage*>( m_stack->widget( index ) );
}


void ConfigWidget::setCurrentIndex(int index)
{
    m_stack->setCurrentIndex( index );
    m_view->setCurrentItem( m_view->item( index ) );
}


void ConfigWidget::setCurrentPage(AbstractConfigPage *page)
{
    setCurrentIndex( m_stack->indexOf( page ) );
}


bool ConfigWidget::commit()
{
    bool b = false;

    Q_ASSERT( m_stack );
    for(int i = 0; i < m_stack->count(); ++i) {
        Q_ASSERT( m_stack->widget( i ) );
        AbstractConfigPage *page = qobject_cast<AbstractConfigPage*>( m_stack->widget( i ) );
        Q_ASSERT( page );

        b = page->commit() || b;
    }

    emit committed();

    return b;
}


void ConfigWidget::revert()
{
    Q_ASSERT( m_stack );
    for(int i = 0; i < m_stack->count(); ++i) {
        Q_ASSERT( m_stack->widget( i ) );
        AbstractConfigPage *page = qobject_cast<AbstractConfigPage*>( m_stack->widget( i ) );
        Q_ASSERT( page );

        page->revert();
    }

    emit reverted();
}


QSplitter* ConfigWidget::splitter() const
{
    return m_splitter;
}


QListWidget* ConfigWidget::listWidget() const
{
    return m_view;
}


QStackedWidget* ConfigWidget::stackedWidget() const
{
    return m_stack;
}


void ConfigWidget::onPageModified()
{
    AbstractConfigPage *page = qobject_cast<AbstractConfigPage*>( sender() );
    Q_ASSERT( page );

    emit pageModified( page );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
