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
#include "attachment.h"

#include <QSet>
#include <QHash>
#include <QSharedData>
#include <QBuffer>
#include <qcryptographichash.h>



Object::Object( QObject *parent )
    : QObject( parent ),
      m_modified( false )
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


void Object::setModified( bool state )
{
    if( m_modified != state ) {
        m_modified = state;

        emit stateChanged();
    }
}


QByteArray Object::hash() const
{
    QByteArray array;
    {
        QBuffer buffer( &array );
        {
            buffer.open( QIODevice::WriteOnly );

            QDataStream out( &buffer );
            serialize( out );
        }
        buffer.close();
    }

    return QCryptographicHash::hash( array, QCryptographicHash::Sha1 );
}


bool Object::hasFlags() const
{
    return !m_flags.isEmpty();
}


int Object::countFlags() const
{
    return m_flags.count();
}


bool Object::containsFlag( const QByteArray &flag ) const
{
    return m_flags.contains( flag );
}


QSet<QByteArray> Object::flags() const
{
    return m_flags;
}


void Object::insertFlag( const QByteArray &flag )
{
    if( !m_flags.contains( flag ) ) {
        m_flags.insert( flag );
        setModified();

        emit flagsChanged();
    }
}


void Object::insertFlags( const QSet<QByteArray> &flags )
{
    if( !m_flags.contains( flags ) ) {
        m_flags.unite( flags );
        setModified();

        emit flagsChanged();
    }
}


void Object::setFlags( const QSet<QByteArray> &flags )
{
    if( m_flags != flags ) {
        m_flags = flags;
        setModified();

        emit flagsChanged();
    }

    Q_ASSERT( m_flags == flags );
}


void Object::removeFlag( const QByteArray &flag )
{
    if( m_flags.remove( flag ) ) {
        setModified();
        emit flagsChanged();
    }
}


void Object::removeFlags( const QSet<QByteArray> &flags )
{
    const int s = m_flags.size();

    m_flags.subtract( flags );
    if( s != m_flags.size() ) {
        setModified();
        emit flagsChanged();
    }
}


void Object::clearFlags()
{
    if( !m_flags.isEmpty() ) {
        m_flags.clear();
        setModified();
        emit flagsChanged();
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


bool Object::containsAttribute( const QByteArray &name ) const
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


QVariant Object::attribute( const QByteArray &name, const QVariant &defaultKey ) const
{
    ASSERT_LIMITED_VARIANT( defaultKey );

    return m_attributes.value( name, defaultKey );
}


void Object::insertAttribute( const QByteArray &name, const QVariant &value )
{
    ASSERT_LIMITED_VARIANT( value );

    if( !m_attributes.contains( name ) || m_attributes.value( name ) != value ) {
        m_attributes.insert( name, value );
        setModified();
        emit attributesChanged();
    }
}


void Object::setAttributes( const QHash<QByteArray, QVariant> &attributes )
{
    for( QHash<QByteArray, QVariant>::const_iterator it = attributes.constBegin(); it != attributes.constEnd(); ++it ) {
        ASSERT_LIMITED_VARIANT( it.value() );
    }

    if( m_attributes != attributes ) {
        m_attributes = attributes;
        setModified();
        emit attributesChanged();
    }

    Q_ASSERT( m_attributes == attributes );
}


void Object::removeAttribute( const QByteArray &name )
{
    if( m_attributes.remove( name ) >= 1 ) {
        setModified();
        emit attributesChanged();
    }
}


void Object::clearAttributes()
{
    if( !m_attributes.isEmpty() ) {
        m_attributes.clear();
        setModified();
        emit attributesChanged();
    }
}


bool Object::hasAttachments() const
{
    return countAttachments() > 0;
}


int Object::countAttachments() const
{
    return m_attachments.size();
}


Attachment *Object::attachment( int index )
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < m_attachments.size() );

    return m_attachments.at( index );
}


const Attachment *Object::attachment( int index ) const
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < m_attachments.size() );

    return m_attachments.at( index );
}


Attachment *Object::takeAttachment( int index )
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < m_attachments.size() );

    Attachment *attachment = m_attachments.takeAt( index );
    setModified();

    disconnect( attachment, 0, this, 0 );
    emit attachmentsChanged();

    return attachment;
}


void Object::insertAttachment( Attachment *attachment )
{
    Q_ASSERT( attachment );

    m_attachments.append( attachment );
    connect( attachment, SIGNAL( valueChanged() ), this, SIGNAL( attachmentsChanged() ) );
    emit attachmentsChanged();
    setModified();
}


void Object::removeAttachment( int index )
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < m_attachments.size() );

    Attachment *attachment = m_attachments.takeAt( index );
    disconnect( attachment, 0, this, 0 );
    setModified();
}


void Object::clearAttachments()
{
    if( m_attachments.isEmpty() ) {
        return;
    }

    while( !m_attachments.isEmpty() ) {
        Attachment *attachment = m_attachments.takeFirst();
        disconnect( attachment, 0, this, 0 );
    }

    setModified();
}


QDataStream &Object::serialize( QDataStream &stream ) const
{
    stream << m_flags;
    stream << m_attributes;

    return stream;
}


QDataStream &Object::deserialize( const Account *account, QDataStream &stream )
{
    Q_UNUSED( account );
    Q_ASSERT( account );

    stream >> m_flags;
    stream >> m_attributes;

    return stream;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
