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
#include "storageexception.h"

#include "account.h"
#include "posting.h"
#include "money.h"

#include <QBuffer>
#include <QFile>
#include <QCoreApplication>

#include <QDebug>


void Storage::writeAccount(Account *acc, const QString &filename)
{
    Storage().write( acc, filename );
}


void Storage::readAccount(Account *acc, const QString &filename, const QByteArray &password)
{
    Storage().read( acc, filename, password );
}


Storage::Storage()
{
}


Storage::~Storage()
{
}


void Storage::write(Account *acc, const QString &filename)
{
    Q_ASSERT( acc );

    if( filename.isEmpty() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - Filename is empty.";

        throw StorageFileException(
                QObject::tr( "The file given could not be written; check "
                              "whether it exists or is writeable for the "
                              "current user." ) );
    }

    QFile file( filename );
    if( !file.open( QFile::WriteOnly ) ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << filename << "could not be opened.";

        throw StorageFileException(
                QObject::tr( "The file given could not be written; check "
                              "whether it exists or is writeable for the "
                              "current user." ) );
    }

    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );
    {
        QDataStream out( &buffer );
        out.setVersion( QDataStream::Qt_4_6 );

        out << static_cast<quint32>( 0x141886C );
        out << static_cast<quint32>( 0x002 );

        QByteArray metaArray = metaData( acc );
        int32_t metaArraySize = metaArray.size();
        out << metaArraySize;
        out.writeRawData( metaArray.data(), metaArraySize );
        out << encodeData( acc );

        if( out.status() != QDataStream::Ok ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - An unknown error occurred.";

            throw StorageFileException( QObject::tr( "An unknown error occurred." ) );
        }
    }
    buffer.close();

    //TODO write to a temporary file first or make a backup or...
    qint64 size = file.write( byteArray );
    if( size < 0 || size != byteArray.size() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__  << " - File"
                 << filename << "could not be written ("
                 << size << "out of" << byteArray.size() << ").";

        throw StorageFileException( QObject::tr( "The file given could not be "
                                      "written; check whether it exists or is "
                                      "writeable for the current user." )
        );
    }

    file.close();

    acc->setModified( false );
}


void Storage::read(Account *acc, const QString &filename, const QByteArray &password)
{
    Q_ASSERT( acc );

    if( filename.isEmpty() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - Filename is empty.";

        throw StorageFileException(
                QObject::tr( "The file given could not be read; check "
                              "whether it exists or is readable for the "
                              "current user." ) );
    }

    QFile file( filename );
    if( !file.open( QFile::ReadOnly ) ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << filename << "could not be opened.";

        throw StorageFileException(
                QObject::tr( "The file given could not be read; check "
                              "whether it exists or is readable for the "
                              "current user." ) );
    }

    QDataStream in( &file );
    in.setVersion( QDataStream::Qt_4_6 );

    quint32 magic;
    in >> magic;
    if( magic != 0x141886C ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - Unknown file format:" << magic;

        throw StorageFileException( QObject::tr( "Unknown file format." ) );
    }

    quint32 version;
    in >> version;
    if( version != 0x002 && version != 0x100 ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - Unknown file version:" << version;

        throw StorageFileException( QObject::tr( "Unknown file version." ) );
    }

    { // skip metadata...
        if( version == 0x002 ) {
            int32_t size;
            in >> size;
            if( in.skipRawData( size ) < 0 ) {
                throw StorageFileException( QObject::tr( "etadata error." ) );
            }
        }
        else {
            Q_ASSERT( version == 0x100 );
            QByteArray metaData;
            in >> metaData;
        }
    }

    QByteArray data;
    in >> data;

    decodeData( data, acc, password );
    acc->setModified( false );
}


QByteArray Storage::metaData(const Account *acc) const
{
    bool passwordProtected = acc->isPasswordEnabled();
    Account::SecurityLevel level = passwordProtected
                                    ? acc->securityLevel() : Account::Low;

    QString headerData( "{\n" );

    headerData.append( "  \"passwordProtected\": " );
    headerData.append( passwordProtected ? "true" : "false" );
    headerData.append( ",\n");

    switch( level ) {
        case Account::Low:
            headerData.append( "  \"description\": \"");
            headerData.append( acc->description() );
            headerData.append( "\",\n");
        case Account::Average:
            headerData.append( "  \"name\": \"");
            headerData.append( acc->name() );
            headerData.append( "\",\n  \"number\": \"");
            headerData.append( acc->number() );
            headerData.append( "\",\n  \"openingDate\": \"");
            headerData.append( acc->openingDate().toString( Qt::ISODate ) );
            headerData.append( "\",\n  \"openingBalance\": ");
            headerData.append( QString::number( acc->openingBalance() ) );
            headerData.append( ",\n  \"iban\": \"");
            headerData.append( acc->iban() );
            headerData.append( "\",\n  \"owner\": \"");
            headerData.append( acc->owner() );
            headerData.append( "\",\n  \"institution\": \"");
            headerData.append( acc->institution() );
            headerData.append( "\",\n  \"bic\": \"");
            headerData.append( acc->bic() );
            headerData.append( "\",\n");
        case Account::High:
            headerData.append( "  \"count\": ");
            headerData.append( QString::number( acc->countPostings() ) );
    }
    headerData.append( "\n}\n" );

    return headerData.toLatin1();
}


QByteArray Storage::encodeData(const Account *acc) const
{
    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );

    QDataStream out( &buffer );
    out.setVersion( QDataStream::Qt_4_6 );

#if defined( WITH_QCA2 )
    if( acc->isPasswordEnabled() ) {
        //This should have been already tested in the appropriate settings dialog.
        Q_ASSERT( QCA::isSupported( "aes256-cbc-pkcs7" ) );

        out << static_cast<quint8>( 1 );

        // Create the key
        Q_ASSERT( !acc->password().isEmpty() );
        QCA::SymmetricKey key( acc->password() );

        // The random initialisation vector
        // Needed to decrypt, but it need not be kept secret.
        QCA::InitializationVector iv( 16 );

        // create a 256 bit AES cipher object using Cipher Block Chaining (CBC) mode
        QCA::Cipher cipher( "aes256",
                            QCA::Cipher::CBC, QCA::Cipher::PKCS7, QCA::Encode,
                            key, iv );

        QByteArray qcaByteArray;
        {
            QBuffer qcaBuffer( &qcaByteArray );
            qcaBuffer.open( QIODevice::WriteOnly );

            QDataStream qcaOut( &qcaBuffer );
            qcaOut.setVersion( QDataStream::Qt_4_6 );

            qcaOut << *acc;
            qcaBuffer.close();

            if( qcaOut.status() != QDataStream::Ok ) {
                qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - An unknown error occurred.";
                throw StorageFileException( QObject::tr( "An unknown error occurred." ) );
            }
        }
        Q_ASSERT( !qcaByteArray.isEmpty() );

        QCA::SecureArray data = cipher.update( qcaByteArray );
        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - cipher update error.";

            throw StorageFileException( QObject::tr( "An unknown error occurred." ) );
        }

        // Because we are using PKCS7 padding, we need to output the final block.
        data.append( cipher.final() );

        // Check if the final() call worked
        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - cipher final error.";

            throw StorageFileException( QObject::tr( "An unknown error occurred." ) );
        }

        // write initialisation vector and the encoded data to out
        out << ( iv.toByteArray() );
        out << ( data.toByteArray() );
    }
    else {
        out << static_cast<quint8>( 0 );
        out << *acc;
    }
#else
    Q_ASSERT( !acc->isPasswordEnabled() );

    out << static_cast<quint8>( 0 );
    out << *acc;
#endif

    buffer.close();

    if( out.status() != QDataStream::Ok ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - stream status error.";

        throw StorageFileException( QObject::tr( "An unknown error occurred." ) );
    }

    Q_ASSERT( !byteArray.isEmpty() );

    return byteArray;
}


void Storage::decodeData(const QByteArray &data, Account *acc, const QByteArray &password) const
{
    QByteArray dataArray( data );
    QBuffer dataBuffer( &dataArray );
    dataBuffer.open( QIODevice::ReadOnly );

    QDataStream dataIn( &dataBuffer );
    dataIn.setVersion( QDataStream::Qt_4_6 );

    quint8 sec;
    dataIn >> sec;
    acc->setSecurityLevel( static_cast<Account::SecurityLevel>( sec ) );

    if( sec == 0 ) {
        dataIn >> *acc;
    }
    else {
        Q_ASSERT( sec == 1 );

#if defined( WITH_QCA2 )
        if( !QCA::isSupported( "aes256-cbc-pkcs7" ) ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - AES data decryption error.";

            throw StorageFileException( QObject::tr( "AES data decryption error." ) );
        }

        if( password.isEmpty() ) {
            throw StoragePasswordException( QObject::tr( "Please enter your current password." ) );
        }

        /// Read data
        QByteArray iv_array;
        dataIn >> iv_array;

        QByteArray data_array;
        dataIn >> data_array;

        /// Now, decrypt...
        QCA::InitializationVector iv( iv_array );

        // create a 256 bit AES cipher object using Cipher Block Chaining (CBC) mode
        QCA::Cipher cipher( "aes256",
                            QCA::Cipher::CBC, QCA::Cipher::PKCS7, QCA::Decode,
                            password, iv );

        QCA::SecureArray plainData = cipher.update( data_array );
        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - Incorrect password";

            throw StoragePasswordException( QObject::tr( "Incorrect password. Please try again." ) );
        }

        plainData.append( cipher.final() );

        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - Incorrect password";

            throw StoragePasswordException( "Incorrect password. Please try again." );
        }

        QByteArray plainDataArray = plainData.toByteArray();
        QBuffer buffer( &plainDataArray );
        buffer.open( QIODevice::ReadOnly );

        QDataStream inner_in( &buffer );
        inner_in.setVersion( QDataStream::Qt_4_6 );

        inner_in >> *acc;

        buffer.close();

        if( inner_in.status() != QDataStream::Ok ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - An unknown error occurred (stream status error).";

            throw StorageFileException( QObject::tr( "Incorrect password. Please try again." ) );
        }

        acc->setPasswordEnabled( true );
        acc->setPassword( password );
#else
        throw StorageException(
                QObject::tr( "This %1 version was build without support to read encrypted files." )
                    .arg( QCoreApplication::applicationName() ) );
#endif
    }

    if( dataIn.status() != QDataStream::Ok ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
            << " - An unknown error occurred (stream status error"
            << dataIn.status() << ").";

        throw StorageException( QObject::tr( "An unknown error occurred." ) );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
