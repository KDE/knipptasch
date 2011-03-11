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
#include "storageexception.h"

#include <QtGlobal>
#include <qxmlstream.h>



/**************** StorageException ****************/

StorageException::StorageException( const QString &s )
    : std::runtime_error( qPrintable( s ) ),
      m_error( s )
{
}


StorageException::~StorageException() throw()
{
}


QString StorageException::errorMessage() const
{
    return m_error;
}




/**************** StorageFileException ****************/

StorageFileException::StorageFileException( const QString &s )
    : StorageException( s )
{
}


StorageFileException::~StorageFileException() throw()
{
}




/**************** StorageFileVersionException ****************/

StorageFileVersionException::StorageFileVersionException( const QString &s )
    : StorageFileException( s )
{
}


StorageFileVersionException::~StorageFileVersionException() throw()
{
}




/**************** StorageParserException ****************/

StorageParserException::StorageParserException( const QString &s, const QString element, int line, int column, int characterOffset )
    : StorageException( s ),
      m_element( element ),
      m_line( line ),
      m_column( column ),
      m_characterOffset( characterOffset )
{
}


StorageParserException::StorageParserException( const QString &s, const QXmlStreamReader &xml )
    : StorageException( s ),
      m_element( xml.name().toString() ),
      m_line( xml.lineNumber() ),
      m_column( xml.columnNumber() ),
      m_characterOffset( xml.characterOffset() )
{
}

StorageParserException::~StorageParserException() throw()
{
}

QString StorageParserException::element() const
{
    return m_element;
}


int StorageParserException::line() const
{
    return m_line;
}


int StorageParserException::column() const
{
    return m_column;
}


int StorageParserException::characterOffset() const
{
    return m_characterOffset;
}




/**************** StorageParserAttributeException ****************/

StorageParserAttributeException::StorageParserAttributeException( const QString &s, const QString attribute, const QString element, int line, int column , int characterOffset )
    : StorageParserException( s, element, line, column, characterOffset ),
      m_attribute( attribute )
{
}


StorageParserAttributeException::StorageParserAttributeException( const QString &s, const QString attribute, const QXmlStreamReader &xml )
    : StorageParserException( s, xml ),
      m_attribute( attribute )
{
}


StorageParserAttributeException::~StorageParserAttributeException() throw()
{
}


QString StorageParserAttributeException::attribute() const
{
    return m_attribute;
}




/**************** StoragePasswordException ****************/

StoragePasswordException::StoragePasswordException( const QString &s )
    : StorageException( s )
{
}


StoragePasswordException::~StoragePasswordException() throw()
{
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
