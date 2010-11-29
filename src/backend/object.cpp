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
#include "object.h"

#include <QSet>
#include <QHash>
#include <QSharedData>



Object::Object()
{
}


Object::~Object()
{
}


bool Object::isModified() const
{
    if( m_modified ) {
        return true;
    }

    return false;
}


void Object::setModified(bool state)
{
    m_modified = state;
}


bool Object::hasFlags() const
{
    return !m_flags.isEmpty();
}


int Object::countFlags() const
{
    return m_flags.count();
}


bool Object::containsFlag(const QByteArray &flag) const
{
    return m_flags.contains( flag );
}


QSet<QByteArray> Object::flags() const
{
    return m_flags;
}


void Object::insertFlag(const QByteArray &flag)
{
    if( !m_flags.contains( flag ) ) {
        m_flags.insert( flag );
        setModified();
    }
}


void Object::insertFlags(const QSet<QByteArray> &flags)
{
    if( !m_flags.contains( flags ) ) {
        m_flags.unite( flags );
        setModified();
    }
}


void Object::setFlags(const QSet<QByteArray> &flags)
{
    if( m_flags != flags ) {
        m_flags = flags;
        setModified();
    }

    Q_ASSERT( m_flags == flags );
}


void Object::removeFlag(const QByteArray &flag)
{
    if( m_flags.remove( flag ) ) {
        setModified();
    }
}


void Object::removeFlags(const QSet<QByteArray> &flags)
{
    const int s = m_flags.size();

    m_flags.subtract( flags );
    if( s != m_flags.size() ) {
        setModified();
    }
}


void Object::clearFlags()
{
    if( !m_flags.isEmpty() ) {
        m_flags.clear();
        setModified();
    }
}


bool Object::hasAttributes() const
{
    return !m_attributes.isEmpty();
}


int Object::countAttributes() const
{
    return m_attributes.count();
}


bool Object::containsAttribute(const QByteArray &name) const
{
    return m_attributes.contains( name );
}


QSet<QByteArray> Object::attributeNames() const
{
    return m_attributes.keys().toSet();
}


QHash<QByteArray, QVariant> Object::attributes() const
{
    return m_attributes;
}


QVariant Object::attribute(const QByteArray &name, const QVariant &defaultKey) const
{
    return m_attributes.value( name, defaultKey );
}


void Object::insertAttribute(const QByteArray &name, const QVariant &value)
{
    if( !m_attributes.contains( name ) || m_attributes.value( name ) != value ) {
        m_attributes.insert( name, value );
        setModified();
    }
}


void Object::setAttributes(const QHash<QByteArray, QVariant> &attributes)
{
    if( m_attributes != attributes ) {
        m_attributes = attributes;
        setModified();
    }

    Q_ASSERT( m_attributes == attributes );
}


void Object::removeAttribute(const QByteArray &name)
{
    if( m_attributes.remove( name ) >= 1 ) {
        setModified();
    }
}


void Object::clearAttributes()
{
    if( !m_attributes.isEmpty() ) {
        m_attributes.clear();
        setModified();
    }
}


QDataStream& Object::serialize(QDataStream &stream) const
{
    stream << m_flags;
    stream << m_attributes;

    return stream;
}


QDataStream& Object::deserialize(QDataStream &stream)
{
    stream >> m_flags;
    stream >> m_attributes;

    return stream;
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
