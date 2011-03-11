/*
 * Copyright 2011 by Stefan Böhmann <kde@hilefoks.org>
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
#include "appearancecolorchooserwidget.h"
#include "ui_appearancecolorchooserwidget.h"

#include "compat/iconloader.h"

#if defined(HAVE_KDE)
#include <kdeversion.h>
#endif

#ifndef KDE_IS_VERSION
#define KDE_IS_VERSION(a,b,c) 1
#endif

#include <QDebug>


AppearanceColorChooserWidget::AppearanceColorChooserWidget( QWidget *parent )
    : QWidget( parent ),
      ui( new Ui::AppearanceColorChooserWidget )
{
    ui->setupUi( this );

    ui->detailsButton->setIcon( BarIcon( "configure" ) );
    ui->detailsButton->setAutoRaise( true );
    ui->detailsButton->setCheckable( true );
    ui->detailsButton->setChecked( false );
    ui->valueWidget->setVisible( false );

#if KDE_IS_VERSION(4,5,0)
    ui->colorButton->setAlphaChannelEnabled( true );
    ui->detailsButton->setVisible( false );
#endif

    connect( ui->colorButton, SIGNAL( changed( QColor ) ), this, SLOT( onColorButtonValueChanged() ) );
    connect( ui->spinBoxRed, SIGNAL( valueChanged( int ) ), this, SLOT( onSpinBoxValueChanged() ) );
    connect( ui->spinBoxBlue, SIGNAL( valueChanged( int ) ), this, SLOT( onSpinBoxValueChanged() ) );
    connect( ui->spinBoxGreen, SIGNAL( valueChanged( int ) ), this, SLOT( onSpinBoxValueChanged() ) );
    connect( ui->spinBoxAlpha, SIGNAL( valueChanged( int ) ), this, SLOT( onSpinBoxValueChanged() ) );

    setEditable( false );
}


AppearanceColorChooserWidget::~AppearanceColorChooserWidget()
{
    delete ui;
}


bool AppearanceColorChooserWidget::editable()
{
    return ui->colorButton->isEnabled();
}


void AppearanceColorChooserWidget::setEditable( bool b )
{
    ui->valueWidget->setVisible( false );
    ui->detailsButton->setEnabled( b );
    ui->colorButton->setEnabled( b );
}


QColor AppearanceColorChooserWidget::color() const
{
    return ui->colorButton->color();
}


void AppearanceColorChooserWidget::setColor( const QColor &color )
{
    m_color = color;
    updateColorWidgets();
}


void AppearanceColorChooserWidget::onColorButtonValueChanged()
{
    m_color = ui->colorButton->color();
    updateColorWidgets();
}


void AppearanceColorChooserWidget::onSpinBoxValueChanged()
{
    int r = ui->spinBoxRed->value();
    int g = ui->spinBoxGreen->value();
    int b = ui->spinBoxBlue->value();
    int a = ui->spinBoxAlpha->value();
    m_color = QColor::fromRgba( qRgba( r, g, b, a ) );

    Q_ASSERT( m_color.isValid() );

    updateColorWidgets();
}


void AppearanceColorChooserWidget::updateColorWidgets()
{
    int r, g, b, a;
    m_color.getRgb( &r, &g, &b, &a );

    bool blocked = blockSignals( true );

    ui->colorButton->setColor( m_color );

    ui->spinBoxRed->setValue( r );
    ui->spinBoxBlue->setValue( b );
    ui->spinBoxGreen->setValue( g );
    ui->spinBoxAlpha->setValue( a );

    blockSignals( blocked );

    emit colorChanged( m_color );
}



QSize AppearanceColorChooserWidget::sizeHint() const
{
    if( !ui->detailsButton->isVisible() || ui->valueWidget->isVisible() ) {
        return QWidget::sizeHint();
    }

    QSize s = QWidget::sizeHint();
    s.setWidth( s.width() + ui->valueWidget->sizeHint().width() );
    s.setHeight( qMax( s.height(), ui->valueWidget->sizeHint().height() ) );

    return s;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
