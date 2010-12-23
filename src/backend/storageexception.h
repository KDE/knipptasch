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
#ifndef STORAGEEXCEPTION_H
#define STORAGEEXCEPTION_H

#include <stdexcept>
#include <QString>

class QXmlStreamReader;


/**
 * @brief Base class for exception used by the Knipptasch storage engine.
 */
class StorageException : public std::runtime_error
{
    public:
        StorageException(const QString &s);
        ~StorageException() throw();

        QString errorMessage() const;

    private:
        QString m_error;
};


class StorageFileException : public StorageException
{
    public:
        StorageFileException(const QString &s);
        ~StorageFileException() throw();
};


class StorageParserException : public StorageException
{
    public:
        StorageParserException(const QString &s, const QString element, int line = -1, int column = -1, int characterOffset = -1);
        StorageParserException(const QString &s, const QXmlStreamReader &xml);
        ~StorageParserException() throw();

        QString element() const;
        int line() const;
        int column() const;
        int characterOffset() const;

    private:
        QString m_element;
        int m_line;
        int m_column;
        int m_characterOffset;
};


class StorageParserAttributeException : public StorageParserException
{
    public:
        StorageParserAttributeException(const QString &s, const QString attribute, const QString element, int line = -1, int column = -1, int characterOffset = -1);
        StorageParserAttributeException(const QString &s, const QString attribute, const QXmlStreamReader &xml);
        ~StorageParserAttributeException() throw();

        QString attribute() const;
    private:
        QString m_attribute;
};


class StoragePasswordException : public StorageException
{
    public:
        StoragePasswordException(const QString &s);
        ~StoragePasswordException() throw();
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
