/*
* Copyright 2010 by Volker Lanz <vl@fidra.de>
* Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
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
#include "actioncollection.h"

#if !defined(HAVE_KDE)

#include <QAction>


ActionCollection::ActionCollection(QObject* parent)
  : QObject( parent )
{
}


QAction* ActionCollection::addAction(const QString& name, QAction* a)
{
    if( !a ) {
        return 0;
    }

    m_Actions.append( a );
    m_ActionsByName[ name ] = a;

    return a;

}


QAction* ActionCollection::addAction(const QString& name)
{
    QAction* action = new KAction( QString(), this );
    return addAction( name, action );
}


QAction* ActionCollection::addAction(const QString& name, const QObject* recv, const char* slot)
{
    QAction* action = new KAction(QString(), this);

    if( recv && slot ) {
        connect(action, SIGNAL( triggered() ), recv, slot);
    }

    return addAction( name, action );
}


QAction* ActionCollection::action(const QString& name)
{
    if( m_ActionsByName.find( name ) != m_ActionsByName.end() ) {
        return m_ActionsByName[ name ];
    }

    return 0;
}


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
