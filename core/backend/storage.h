/*
 * Copyright 2008-2010  Stefan Böhmann <kde@hilefoks.org>
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
#ifndef STORAGE_H
#define STORAGE_H

#include "knipptasch_core_export.h"

#include <QString>

class Account;


/**
 * @class Storage
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class KNIPPTASCH_CORE_EXPORT Storage
{
    public:
        static void writeAccount( Account *acc, const QString &filename );
        static void readAccount( Account *acc, const QString &filename, const QByteArray &password = QByteArray() );

        static QString filterStringQt();
        static QString filterStringKDE();

    private:
        Storage();
        ~Storage();

        /**
         * @exception StorageFileException
         */
        void write( Account *acc, const QString &filename );

        /**
         * @exception StorageFileException
         * @exception StoragePasswordException
         */
        void read( Account *acc, const QString &filename, const QByteArray &password );

        /**
         *
         */
        QByteArray metaData( const Account *acc ) const;

        /**
         * @exception StorageFileException
         * @exception StoragePasswordException
         */
        QByteArray encodeData( const Account *acc ) const;

        /**
         * @exception StorageFileException
         */
        void decodeData( const QByteArray &data, Account *acc, const QByteArray &password ) const;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
