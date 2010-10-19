/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
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
#include "searchwidget.h"
#include "ui_searchwidget.h"

#include "compat/iconloader.h"


SearchWidget::SearchWidget(QWidget *parent)
  : QFrame( parent ),
    ui( new Ui::SearchWidget ),
    m_view( 0 )
{
    ui->setupUi( this );

    ui->closeButton->setAutoRaise( true );
    ui->closeButton->setIcon( BarIcon("dialog-close") );

    ui->extenderButton->setAutoRaise( true );
    ui->extenderButton->setIcon( BarIcon("arrow-down") );

    ui->columnCombo->clear();
    ui->columnCombo->setVisible( false );

    ui->nextButton->setEnabled( false );
    ui->previousButton->setEnabled( false );

    connect( ui->closeButton, SIGNAL( clicked(bool) ), this, SLOT( hide() ) );
    connect( ui->extenderButton, SIGNAL( clicked(bool) ), this, SIGNAL( transitionButtonClicked() ) );

    connect( ui->columnCombo, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotColumnChanged() ) );
    connect( ui->search, SIGNAL( textChanged(QString) ), this, SLOT( slotStringChanged() ) );
}


void SearchWidget::setItemView(QAbstractItemView *view)
{
    m_view = view;

    ui->columnCombo->clear();
    ui->columnCombo->setVisible( false );

    if( m_view ) {
        ui->columnCombo->addItem( tr( "All Column" ), -1 );
        //TODO

        ui->columnCombo->setVisible( true );
    }
}


void SearchWidget::showEvent(QShowEvent * event)
{
    Q_UNUSED( event );

}


void SearchWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED( event );
}


void SearchWidget::slotStringChanged()
{
    if( ui->search->text().trimmed().isEmpty() ) {
        ui->nextButton->setEnabled( false );
        ui->previousButton->setEnabled( false );
    }

    emit filterStringChanged( ui->search->text().trimmed() );
}


void SearchWidget::slotColumnChanged()
{
    int column = -1;
    if( ui->columnCombo->isVisible() && ui->columnCombo->currentIndex() >= 0 ) {
        column = ui->columnCombo->itemData( ui->columnCombo->currentIndex() ).toInt();
    }

    emit filterColumnChanged( column );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
