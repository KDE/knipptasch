/*  This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)
    Copyright (C) 1999 Cristian Tibirna (ctibirna@kde.org)
    Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "kcolorbutton.h"


#include <QtCore/QPointer>
#include <QtGui/QPainter>
#include <QtGui/qdrawutil.h>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QStyle>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QColorDialog>


void fillOpaqueRect( QPainter *painter, const QRect &rect, const QBrush &brush )
{
    if( !brush.isOpaque() ) {
        QPixmap chessboardPattern( 16, 16 );
        QPainter patternPainter( &chessboardPattern );
        patternPainter.fillRect( 0, 0, 8, 8, Qt::black );
        patternPainter.fillRect( 8, 8, 8, 8, Qt::black );
        patternPainter.fillRect( 0, 8, 8, 8, Qt::white );
        patternPainter.fillRect( 8, 0, 8, 8, Qt::white );
        patternPainter.end();
        painter->fillRect( rect, QBrush( chessboardPattern ) );
    }
    painter->fillRect( rect, brush );
}


class KColorButton::KColorButtonPrivate
{
    public:
        KColorButtonPrivate( KColorButton *q );

        void _k_chooseColor();

        KColorButton *q;
        QColor m_defaultColor;
        bool m_bdefaultColor : 1;
        bool m_alphaChannel : 1;

        QColor col;
        QPoint mPos;

        void initStyleOption( QStyleOptionButton *opt ) const;
};


KColorButton::KColorButtonPrivate::KColorButtonPrivate( KColorButton *q )
    : q( q )
{
    m_bdefaultColor = false;
    m_alphaChannel = false;
    q->setAcceptDrops( true );

    connect( q, SIGNAL( clicked() ), q, SLOT( chooseColor() ) );
}

KColorButton::KColorButton( QWidget *parent )
    : QPushButton( parent )
    , d( new KColorButtonPrivate( this ) )
{
}

KColorButton::KColorButton( const QColor &c, QWidget *parent )
    : QPushButton( parent )
    , d( new KColorButtonPrivate( this ) )
{
    d->col = c;
}

KColorButton::KColorButton( const QColor &c, const QColor &defaultColor, QWidget *parent )
    : QPushButton( parent )
    , d( new KColorButtonPrivate( this ) )
{
    d->col = c;
    setDefaultColor( defaultColor );
}

KColorButton::~KColorButton()
{
    delete d;
}

QColor KColorButton::color() const
{
    return d->col;
}

void KColorButton::setColor( const QColor &c )
{
    if( d->col != c ) {
        d->col = c;
        update();
        emit changed( d->col );
    }
}

void KColorButton::setAlphaChannelEnabled( bool alpha )
{
    d->m_alphaChannel = alpha;
}

bool KColorButton::isAlphaChannelEnabled() const
{
    return d->m_alphaChannel;
}

QColor KColorButton::defaultColor() const
{
    return d->m_defaultColor;
}

void KColorButton::setDefaultColor( const QColor &c )
{
    d->m_bdefaultColor = c.isValid();
    d->m_defaultColor = c;
}

void KColorButton::KColorButtonPrivate::initStyleOption( QStyleOptionButton *opt ) const
{
    opt->initFrom( q );
    opt->state |= q->isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
    opt->features = QStyleOptionButton::None;
    if( q->isDefault() ) {
        opt->features |= QStyleOptionButton::DefaultButton;
    }
    opt->text.clear();
    opt->icon = QIcon();
}

void KColorButton::paintEvent( QPaintEvent * )
{
    QPainter painter( this );
    QStyle *style = QWidget::style();

    //First, we need to draw the bevel.
    QStyleOptionButton butOpt;
    d->initStyleOption( &butOpt );
    style->drawControl( QStyle::CE_PushButtonBevel, &butOpt, &painter, this );

    //OK, now we can muck around with drawing out pretty little color box
    //First, sort out where it goes
    QRect labelRect = style->subElementRect( QStyle::SE_PushButtonContents,
                      &butOpt, this );
    int shift = style->pixelMetric( QStyle::PM_ButtonMargin, &butOpt, this ) / 2;
    labelRect.adjust( shift, shift, -shift, -shift );
    int x, y, w, h;
    labelRect.getRect( &x, &y, &w, &h );

    if( isChecked() || isDown() ) {
        x += style->pixelMetric( QStyle::PM_ButtonShiftHorizontal, &butOpt, this );
        y += style->pixelMetric( QStyle::PM_ButtonShiftVertical, &butOpt, this );
    }

    QColor fillCol = isEnabled() ? d->col : palette().color( backgroundRole() );
    qDrawShadePanel( &painter, x, y, w, h, palette(), true, 1, NULL );
    if( fillCol.isValid() ) {
        fillOpaqueRect( &painter, QRect( x + 1, y + 1, w - 2, h - 2 ), fillCol );
    }

    if( hasFocus() ) {
        QRect focusRect = style->subElementRect( QStyle::SE_PushButtonFocusRect, &butOpt, this );
        QStyleOptionFocusRect focusOpt;
        focusOpt.init( this );
        focusOpt.rect            = focusRect;
        focusOpt.backgroundColor = palette().background().color();
        style->drawPrimitive( QStyle::PE_FrameFocusRect, &focusOpt, &painter, this );
    }
}

QSize KColorButton::sizeHint() const
{
    QStyleOptionButton opt;
    d->initStyleOption( &opt );
    return style()->sizeFromContents( QStyle::CT_PushButton, &opt, QSize( 40, 15 ), this ).
           expandedTo( QApplication::globalStrut() );
}

QSize KColorButton::minimumSizeHint() const
{
    QStyleOptionButton opt;
    d->initStyleOption( &opt );
    return style()->sizeFromContents( QStyle::CT_PushButton, &opt, QSize( 3, 3 ), this ).
           expandedTo( QApplication::globalStrut() );
}


void KColorButton::chooseColor()
{
    QPointer<QColorDialog> dialog = new QColorDialog( color(), this ); // krazy:exclude=qclasses
    dialog->setOption( QColorDialog::ShowAlphaChannel );

    if( dialog->exec() != QDialog::Rejected ) {
        if( dialog->selectedColor().isValid() ) {
            setColor( dialog->selectedColor() );
        } else if( d->m_bdefaultColor ) {
            setColor( d->m_defaultColor );
        }
    }
    delete dialog;
}
