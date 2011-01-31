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
#include "datevalidator.h"

#include "compat/utils.h"

#include <QtCore/QDate>
#include <QtGui/QValidator>
#include <QtGui/QApplication>
#include <QtGui/QStatusTipEvent>



DateValidator::DateValidator(QWidget *parent)
  : QValidator( parent ),
    m_statusTipEnabled( true )
{
}


DateValidator::DateValidator(const QString &dateFormat, QWidget *parent)
  : QValidator( parent ),
    m_dateFormat( dateFormat ),
    m_statusTipEnabled( true )
{
}


DateValidator::DateValidator(const QString &dateFormat, const QStringList &keywords, QWidget *parent)
  : QValidator( parent ),
    m_keywords( keywords ),
    m_dateFormat( dateFormat ),
    m_statusTipEnabled( true )
{
}


DateValidator::~DateValidator()
{
    if( m_statusTipEnabled ) {
        QStatusTipEvent *event = new QStatusTipEvent( "" );
        qApp->sendEvent( parent(), event );
    }
}


QValidator::State DateValidator::validate(QString &str, int &value) const
{
    Q_UNUSED( value );

    if( m_statusTipEnabled ) {
        QStatusTipEvent *event = new QStatusTipEvent( "" );
        qApp->sendEvent( parent(), event );
    }

    // empty string is intermediate so one can clear the
    // edit line and start from scratch
    if( str.isEmpty() ) {
        return Intermediate;
    }

    if( m_keywords.contains( str.toLower() ) ) {
        return Acceptable;
    }

    bool ok = false;
    QDate date = readDate( str, m_dateFormat, &ok );

    if( ok ) {
        if( m_statusTipEnabled ) {
            QStatusTipEvent *event = new QStatusTipEvent( formatLongDate( date ) );
            qApp->sendEvent( parent(), event );
        }

        return Acceptable;
    }

    return Intermediate;
}


QStringList DateValidator::keywordList() const
{
    return m_keywords;
}


void DateValidator::setKeywordList(const QStringList &list)
{
    m_keywords = list;
}


QString DateValidator::dateFormat() const
{
    return m_dateFormat;
}


void DateValidator::setDateFormat(const QString &dateFormat)
{
    m_dateFormat = dateFormat;
}


bool DateValidator::statusTipEnabled() const
{
    return m_statusTipEnabled;
}


void DateValidator::setStatusTipEnabled(bool enabled)
{
    if( m_statusTipEnabled != enabled ) {
        if( m_statusTipEnabled ) {
            QStatusTipEvent *event = new QStatusTipEvent( "" );
            qApp->sendEvent( parent(), event );
        }

        m_statusTipEnabled = enabled;
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
