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
#include "postingtabwidget.h"
#include "ui_postingtabwidget.h"

#include "accountmodel.h"

#include "compat/iconloader.h"

#include "backend/account.h"

#include <QDataWidgetMapper>



PostingTabWidget::PostingTabWidget( QWidget *parent )
    : AbstractAccountTabWidget( tr( "Posting" ), BarIcon( "knipptasch" ), parent ),
      ui( new Ui::PostingTabWidget ),
      m_mapper( new QDataWidgetMapper( this ) )
{
    ui->setupUi( this );

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy( QDataWidgetMapper::AutoSubmit );
}


void PostingTabWidget::accountModelChanged( AccountModel *model )
{
    m_mapper->setModel( model );

    m_mapper->addMapping( ui->maturity, AccountModel::MATURITY );
    m_mapper->addMapping( ui->postingtext, AccountModel::POSTINGTEXT );
    m_mapper->addMapping( ui->valuedate, AccountModel::VALUEDATE );
    m_mapper->addMapping( ui->amount, AccountModel::AMOUNT );
    m_mapper->addMapping( ui->warranty, AccountModel::WARRANTY );
//    m_mapper->addMapping( ui->payee, AccountModel::PAYEE );
    m_mapper->addMapping( ui->category, AccountModel::CATEGORY );
//    m_mapper->addMapping( ui->payment, AccountModel::PAYMENT );
//    m_mapper->addMapping( ui->voucher, AccountModel::VOUCHER );
    m_mapper->addMapping( ui->statement, AccountModel::STATEMENT );

    setEnabled( false );
}


void PostingTabWidget::currentSelectedIndexChanged( const QModelIndex &index )
{
    m_mapper->submit();
    m_mapper->setCurrentModelIndex( index );

    setEnabled( accountModel() && index.isValid() );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
