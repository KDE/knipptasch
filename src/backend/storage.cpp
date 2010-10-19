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
#include "storage.h"

#include "account.h"
#include "posting.h"

#include <QBuffer>
#include <QFileInfo>
#include <QFile>
#include <QCoreApplication>

#if defined(HAVE_KDE)
#include <KMessageBox>
#else
#include <QMessageBox>
#endif



QFile::FileError Storage::writeAccount(QWidget *parent, Account *acc, const QString &filename)
{
    return Storage().write( parent, acc, filename );
}


QFile::FileError Storage::readAccount(QWidget *parent, Account *acc, const QString &filename)
{
    return Storage().read( parent, acc, filename );
}


Storage::Storage()
{
}


Storage::~Storage()
{
}


QFile::FileError Storage::write(QWidget *parent, Account *acc, const QString &filename)
{
    Q_ASSERT( acc );

    if( filename.isEmpty() ) {
        return QFile::ResourceError;
    }

    QFile file( filename );
    if( !file.open( QFile::WriteOnly ) ) {
        QString message = QObject::tr( "The file given could not be written; "
                                       "check whether it exists or is "
                                       "writeable for the current user." );
#if defined(HAVE_KDE)
        KMessageBox::error( parent, message );
#else
        QMessageBox::warning( // krazy:exclude=qclasses
            parent,
            QObject::tr( "Error - %1" ).arg( QCoreApplication::applicationName() ),
            message
        );
#endif

        return QFile::WriteError;
    }

    QDataStream out( &file );
    out.setVersion( QDataStream::Qt_4_6 );

    out << static_cast<quint32>( 0x141886C );
    out << static_cast<quint32>( 0x100 );
    out << ( *acc );

    if( out.status() != QDataStream::Ok ) {
        return QFile::FatalError;
    }

    return QFile::NoError;
}


QFile::FileError Storage::read(QWidget *parent, Account *acc, const QString &filename)
{
    Q_ASSERT( acc );

    if( filename.isEmpty() ) {
        return QFile::ResourceError;
    }

    QFile file( filename );
    if( !file.open( QFile::ReadOnly ) ) {
        QString message = QObject::tr( "The file given could not be read; "
                                       "check whether it exists or is readable "
                                       "for the current user." );
#if defined(HAVE_KDE)
        KMessageBox::error( parent, message );
#else
        QMessageBox::warning( // krazy:exclude=qclasses
            parent,
            QObject::tr( "Error - %1" ).arg( QCoreApplication::applicationName() ),
            message
        );
#endif

        return QFile::ReadError;
    }

    QDataStream in( &file );
    in.setVersion( QDataStream::Qt_4_6 );
    quint32 magic;
    in >> magic;
    if( magic != 0x141886C ) {
        QString msg = QObject::tr( "Unknown file format." );
#if defined(HAVE_KDE)
        KMessageBox::error( parent, msg );
#else
        QMessageBox::warning( parent, QObject::tr( "Error - %1" ).arg( // krazy:exclude=qclasses
                                  QCoreApplication::applicationName() ), msg );
#endif

        return QFile::ResourceError;
    }

    qint32 version;
    in >> version;
    if( version != 0x100 ) {
        QString msg = QObject::tr( "Unknown file version." );
#if defined(HAVE_KDE)
        KMessageBox::error( parent, msg );
#else
        QMessageBox::warning( parent, QObject::tr( "Error - %1" ).arg( // krazy:exclude=qclasses
                                  QCoreApplication::applicationName() ), msg );
#endif
        return QFile::ResourceError;
    }

    in >> ( *acc );

    if( in.status() != QDataStream::Ok ) {
        return QFile::FatalError;
    }

    return QFile::NoError;
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
