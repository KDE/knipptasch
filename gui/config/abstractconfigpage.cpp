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
#include "abstractconfigpage.h"



AbstractConfigPage::AbstractConfigPage( const QString &label, const QString &title, const QIcon &icon, ConfigWidget *parent )
    : QWidget( parent ),
      m_configWidget( parent ),
      m_errorMessageEnabled( false ),
      m_errorMessageType( ErrorMessage ),
      m_errorMessageTitle( tr( "Error" ) )
{
    Q_ASSERT( m_configWidget );

    setPageLabel( label );
    setPageTitle( title );
    setPageIcon( icon );
}


AbstractConfigPage::AbstractConfigPage( const QString &label, const QIcon &icon, ConfigWidget *parent )
    : QWidget( parent ),
      m_configWidget( parent ),
      m_errorMessageEnabled( false ),
      m_errorMessageType( ErrorMessage ),
      m_errorMessageTitle( tr( "Error" ) )
{
    Q_ASSERT( m_configWidget );

    setPageLabel( label );
    setPageTitle( label );
    setPageIcon( icon );
}


AbstractConfigPage::AbstractConfigPage( const QString &label, ConfigWidget *parent )
    : QWidget( parent ),
      m_configWidget( parent ),
      m_errorMessageEnabled( false ),
      m_errorMessageType( ErrorMessage ),
      m_errorMessageTitle( tr( "Error" ) )
{
    Q_ASSERT( m_configWidget );

    setPageLabel( label );
    setPageTitle( label );
}


AbstractConfigPage::~AbstractConfigPage()
{
}


bool AbstractConfigPage::isValid() const
{
    return true;
}


bool AbstractConfigPage::errorMessageEnabled() const
{
    return m_errorMessageEnabled;
}


void AbstractConfigPage::setErrorMessageEnabled( bool b )
{
    if( m_errorMessageEnabled != b ) {
        m_errorMessageEnabled = b;
        emit errorMessageChanged();
    }
}


AbstractConfigPage::ErrorMessageType AbstractConfigPage::errorMessageType() const
{
    return m_errorMessageType;
}


void AbstractConfigPage::setErrorMessageType( AbstractConfigPage::ErrorMessageType type )
{
    if( m_errorMessageType != type ) {
        m_errorMessageType = type;

        if( m_errorMessageEnabled ) {
            emit errorMessageChanged();
        }
    }
}


QString AbstractConfigPage::errorMessageTitle() const
{
    return m_errorMessageTitle;
}


void AbstractConfigPage::setErrorMessageTitle( const QString &str )
{
    if( m_errorMessageTitle != str ) {
        m_errorMessageTitle = str;

        if( m_errorMessageEnabled ) {
            emit errorMessageChanged();
        }
    }
}


QString AbstractConfigPage::errorMessageDescription() const
{
    return m_errorMessageDescription;
}


void AbstractConfigPage::setErrorMessageDescription( const QString &str )
{
    if( m_errorMessageDescription != str ) {
        m_errorMessageDescription = str;

        if( m_errorMessageEnabled ) {
            emit errorMessageChanged();
        }
    }
}


ConfigWidget *AbstractConfigPage::configWidget() const
{
    return m_configWidget;
}


void AbstractConfigPage::setConfigWidget( ConfigWidget *widget )
{
    Q_ASSERT( widget );
    m_configWidget = widget;
}


QString AbstractConfigPage::pageLabel() const
{
    return m_label;
}


void AbstractConfigPage::setPageLabel( const QString &str )
{
    m_label = str;
}


QString AbstractConfigPage::pageTitle() const
{
    return m_title;
}


void AbstractConfigPage::setPageTitle( const QString &str )
{
    m_title = str;
}


QString AbstractConfigPage::pageDescription() const
{
    return m_description;
}


void AbstractConfigPage::setPageDescription( const QString &str )
{
    m_description = str;
}


QIcon AbstractConfigPage::pageIcon() const
{
    return m_icon;
}


void AbstractConfigPage::setPageIcon( const QIcon &icon )
{
    m_icon = icon;
}


QString AbstractConfigPage::pageToolTip() const
{
    return m_toolTip;
}


void AbstractConfigPage::setPageToolTip( const QString &str )
{
    m_toolTip = str;
}


QString AbstractConfigPage::pageWhatsThis() const
{
    return m_whatsThis;
}


void AbstractConfigPage::setPageWhatsThis( const QString &str )
{
    m_whatsThis = str;
}


void AbstractConfigPage::revert()
{
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
