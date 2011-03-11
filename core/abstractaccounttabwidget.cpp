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
#include "abstractaccounttabwidget.h"

#include "accountmodel.h"
#include "backend/account.h"

#include <QIcon>
#include <QCoreApplication>


struct AbstractAccountTabWidget::Private {
    Private()
        : model( 0 ) {
    }

    AccountModel *model;
    QModelIndex index;

    QIcon icon;
    QString label;
    QString toolTip;
    QString whatsThis;
};



AbstractAccountTabWidget::AbstractAccountTabWidget( const QString &title, QWidget *parent )
    : QWidget( parent ),
      d( new AbstractAccountTabWidget::Private )
{
    d->label = title;
}


AbstractAccountTabWidget::AbstractAccountTabWidget( const QString &title, const QIcon &icon, QWidget *parent )
    : QWidget( parent ),
      d( new AbstractAccountTabWidget::Private )
{
    d->label = title;
    d->icon = icon;
}


AbstractAccountTabWidget::~AbstractAccountTabWidget()
{
    delete d;
}


AccountModel *AbstractAccountTabWidget::accountModel()
{
    Q_ASSERT( d );

    return d->model;
}


const AccountModel *AbstractAccountTabWidget::accountModel() const
{
    Q_ASSERT( d );

    return d->model;
}


void AbstractAccountTabWidget::setAccountModel( AccountModel *model )
{
    if( d->model != model ) {
        setCurrentSelectedIndex( QModelIndex() );

        accountModelAboutToBeChanged();
        d->model = model;
        accountModelChanged( d->model );
    }
}


QModelIndex AbstractAccountTabWidget::currentSelectedIndex() const
{
    Q_ASSERT( d );

    return d->index;
}


QIcon AbstractAccountTabWidget::icon() const
{
    Q_ASSERT( d );

    return d->icon;
}


QString AbstractAccountTabWidget::label() const
{
    Q_ASSERT( d );

    return d->label;
}

QString AbstractAccountTabWidget::tabToolTip() const
{
    Q_ASSERT( d );

    return d->toolTip;
}


QString AbstractAccountTabWidget::tabWhatsThis() const
{
    Q_ASSERT( d );

    return d->whatsThis;
}


void AbstractAccountTabWidget::setCurrentSelectedIndex( const QModelIndex &index )
{
    Q_ASSERT( d );

    if( d->index != index ) {
        currentSelectedIndexAboutToBeChanged();
        d->index = index;
        currentSelectedIndexChanged( d->index );
    }
}


void AbstractAccountTabWidget::setIcon( const QIcon &ico )
{
    Q_ASSERT( d );

    d->icon = ico;
    emit updateTabView( this );
}


void AbstractAccountTabWidget::setLabel( const QString &str )
{
    Q_ASSERT( d );

    if( d->label != str ) {
        d->label = str;
        emit updateTabView( this );
    }
}


void AbstractAccountTabWidget::setToolTip( const QString &str )
{
    Q_ASSERT( d );

    if( d->toolTip != str ) {
        d->toolTip = str;
        emit updateTabView( this );
    }
}


void AbstractAccountTabWidget::setWhatsThis( const QString &str )
{
    Q_ASSERT( d );

    if( d->whatsThis != str ) {
        d->whatsThis = str;
        emit updateTabView( this );
    }
}


void AbstractAccountTabWidget::accountModelAboutToBeChanged()
{
}


void AbstractAccountTabWidget::currentSelectedIndexAboutToBeChanged()
{
}


void AbstractAccountTabWidget::changeEvent( QEvent *event )
{
    if( event->type() == QEvent::EnabledChange ) {
        emit updateTabView( this );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
