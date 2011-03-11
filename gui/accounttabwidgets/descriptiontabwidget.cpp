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
#include "descriptiontabwidget.h"

#include "accountmodel.h"
#include "backend/posting.h"

#include "compat/iconloader.h"

#include <QIcon>
#include <QTimer>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QDataWidgetMapper>
#include <QCoreApplication>



DescriptionTabWidget::DescriptionTabWidget( QWidget *parent )
    : AbstractAccountTabWidget( tr( "Description" ), parent ),
      m_richttextwidget( new QPlainTextEdit( this ) ),
      m_mapper( new QDataWidgetMapper( this ) )
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( m_richttextwidget );
    setLayout( layout );

    m_mapper = new QDataWidgetMapper;
    m_mapper->setSubmitPolicy( QDataWidgetMapper::AutoSubmit );

    connect( m_richttextwidget, SIGNAL( textChanged() ), this, SLOT( slotUpdateState() ) );

    QTimer::singleShot( 0, this, SLOT( slotUpdateState() ) );
}


void DescriptionTabWidget::accountModelChanged( AccountModel *model )
{
    m_mapper->setModel( model );
    m_mapper->addMapping( m_richttextwidget, AccountModel::DESCRIPTION );

    slotUpdateState();
    setEnabled( false );
}


void DescriptionTabWidget::currentSelectedIndexChanged( const QModelIndex &index )
{
    m_mapper->submit();
    m_mapper->setCurrentModelIndex( index );

    slotUpdateState();
    setEnabled( accountModel() && index.isValid() );
}


void DescriptionTabWidget::slotUpdateState()
{
    m_mapper->submit();

    QPixmap pix = BarIcon( "knotes" ).scaled( 16, 16 );

    if( currentSelectedIndex().isValid() && accountModel() ) {
        if( accountModel()->data( accountModel()->index( currentSelectedIndex().row(), AccountModel::DESCRIPTION ) ).toString().isEmpty() ) {
            QImage img = pix.toImage();
            Q_ASSERT( img.depth() == 32 );

            if( img.format() == QImage::Format_ARGB32_Premultiplied ) {
                img = img.convertToFormat( QImage::Format_ARGB32 );
            }

            unsigned char *line;
            for( int y = 0; y < img.height(); ++y ) {
                if( QSysInfo::ByteOrder == QSysInfo::BigEndian ) {
                    line = img.scanLine( y );
                } else {
                    line = img.scanLine( y ) + 3;
                }
                for( int x = 0; x < img.width(); ++x ) {
                    *line >>= 1;
                    line += 4;
                }
            }
            pix = QPixmap::fromImage( img );
        }
    }

    setIcon( pix );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
