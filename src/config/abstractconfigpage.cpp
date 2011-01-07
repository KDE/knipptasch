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



AbstractConfigPage::AbstractConfigPage(const QString &title, const QIcon &icon, ConfigWidget *parent)
  : QWidget( parent ),
    m_configWidget( parent )
{
    Q_ASSERT( m_configWidget );

    setPageTitle( title );
    setPageIcon( icon );
}


AbstractConfigPage::~AbstractConfigPage()
{
}


bool AbstractConfigPage::isValid() const
{
    return true;
}


ConfigWidget* AbstractConfigPage::configWidget() const
{
    return m_configWidget;
}


void AbstractConfigPage::setConfigWidget(ConfigWidget *widget)
{
    Q_ASSERT( widget );
    m_configWidget = widget;
}


QString AbstractConfigPage::pageTitle() const
{
    return m_title;
}


void AbstractConfigPage::setPageTitle(const QString &str)
{
    m_title = str;
}


QIcon AbstractConfigPage::pageIcon() const
{
    return m_icon;
}


void AbstractConfigPage::setPageIcon(const QIcon &icon)
{
    m_icon = icon;
}


QString AbstractConfigPage::pageToolTip() const
{
    return m_toolTip;
}


void AbstractConfigPage::setPageToolTip(const QString &str)
{
    m_toolTip = str;
}


QString AbstractConfigPage::pageWhatsThis() const
{
    return m_whatsThis;
}


void AbstractConfigPage::setPageWhatsThis(const QString &str)
{
    m_whatsThis = str;
}


void AbstractConfigPage::revert()
{
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
