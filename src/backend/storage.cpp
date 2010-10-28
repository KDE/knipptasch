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
#include "money.h"

#if defined( WITH_QCA2 )
#include "passworddialog.h"
#endif

#include <QBuffer>
#include <QFileInfo>
#include <QFile>
#include <QCoreApplication>

#if defined(HAVE_KDE)
#include <KMessageBox>
#else
#include <QMessageBox>
#endif

#include <QDebug>
#include <QPointer>



bool Storage::writeAccount(QWidget *parent, Account *acc, const QString &filename)
{
    return Storage( parent, filename ).write( acc );
}


bool Storage::readAccount(QWidget *parent, Account *acc, const QString &filename)
{
    return Storage( parent, filename ).read( acc );
}


Storage::Storage(QWidget *parent, const QString &filename)
  : m_parent( parent ),
    m_filename( filename )
{
}


Storage::~Storage()
{
}


bool Storage::write(Account *acc)
{
    Q_ASSERT( acc );

    if( m_filename.isEmpty() ) {
        return QFile::ResourceError;
    }

    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );
    {
        QDataStream out( &buffer );
        out.setVersion( QDataStream::Qt_4_6 );

        out << static_cast<quint32>( 0x141886C );
        out << static_cast<quint32>( 0x100 );

        bool ok = false;

        out << metaData( acc, ok );
        if( !ok ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - An unknown error occurred.";

            errorMessage( QObject::tr( "An unknown error occurred." ) );
            return QFile::FatalError;
        }

        ok = false;
        out << encodedData( acc, ok );
        if( !ok ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - An unknown error occurred.";

            errorMessage( QObject::tr( "An unknown error occurred." ) );
            return QFile::FatalError;
        }

        if( out.status() != QDataStream::Ok ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - An unknown error occurred.";

            errorMessage( QObject::tr( "An unknown error occurred." ) );
            return QFile::FatalError;
        }
    }
    buffer.close();

    QFile file( m_filename );
    if( !file.open( QFile::WriteOnly ) ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << m_filename << "could not be opened.";

        errorMessage( QObject::tr( "The file given could not be written; "
                                   "check whether it exists or is writeable "
                                   "for the current user." )
        );

        return QFile::WriteError;
    }

    //TODO write to a temporary file first or make a backup or... 
    qint64 size = file.write( byteArray );
    if( size < 0 || size != byteArray.size() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << m_filename << "could not be written ("
                 << size << "out of" << byteArray.size() << ").";

        errorMessage( QObject::tr( "The file given could not be written; "
                                   "check whether it exists or is writeable "
                                   "for the current user." )
        );

        return QFile::WriteError;
    }

    return QFile::NoError;
}


bool Storage::read(Account *acc)
{
    Q_ASSERT( acc );

    if( m_filename.isEmpty() ) {
        return false;
    }

    QFile file( m_filename );
    if( !file.open( QFile::ReadOnly ) ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - The file given could not be read.";

        errorMessage( QObject::tr( "The file given could not be read; check "
                                   "whether it exists or is readable for the "
                                   "current user." )
        );

        return false;
    }

    QDataStream in( &file );
    in.setVersion( QDataStream::Qt_4_6 );

    quint32 magic;
    in >> magic;
    if( magic != 0x141886C ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - Unknown file format:" << magic;

        errorMessage( QObject::tr( "Unknown file format." ) );
        return false;
    }

    qint32 version;
    in >> version;
    if( version != 0x100 ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - Unknown file version:" << version;

        errorMessage( QObject::tr( "Unknown file version." ) );
        return false;
    }

    { // skip metadata...
        QByteArray metaData;
        in >> metaData;

        qDebug() << "skipped metadata:" << QByteArray::fromHex( metaData );
    }


    QByteArray data;
    in >> data;
    if( !decodedData( data, acc ) ) {
        return false;
    }

    return true;
}


QByteArray Storage::metaData(const Account *acc, bool &ok) const
{
    ok = false;
    Account::SecurityLevel level = acc->isPasswordEnabled()
                                    ? acc->securityLevel() : Account::Low;

    QString headerData( "{\n" );
    switch( level ) {
        case Account::Low:
            headerData.append( "  \"description\": \"");
            headerData.append( acc->description() );
            headerData.append( "\"\n");
        case Account::Average:
            headerData.append( "  \"name\": \"");
            headerData.append( acc->name() );
            headerData.append( "\"\n  \"number\": \"");
            headerData.append( acc->number() );
            headerData.append( "\"\n  \"openingDate\": \"");
            headerData.append( acc->openingDate().toString( Qt::ISODate ) );
            headerData.append( "\"\n  \"openingBalance\": ");
            headerData.append( QString::number( acc->openingBalance().cents() ) );
            headerData.append( "\n  \"iban\": \"");
            headerData.append( acc->iban() );
            headerData.append( "\"\n  \"owner\": \"");
            headerData.append( acc->owner() );
            headerData.append( "\"\n  \"institution\": \"");
            headerData.append( acc->institution() );
            headerData.append( "\"\n  \"bic\": \"");
            headerData.append( acc->bic() );
            headerData.append( "\"\n");
        case Account::High:
            headerData.append( "  \"count\": ");
            headerData.append( QString::number( acc->countPostings() ) );
    }
    headerData.append( "\n}\n" );

    ok = true;
    return headerData.toUtf8().toHex();
}


QByteArray Storage::encodedData(const Account *acc, bool &ok) const
{
    ok = false;
    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );

    QDataStream out( &buffer );
    out.setVersion( QDataStream::Qt_4_6 );

#if defined( WITH_QCA2 )
    if( acc->isPasswordEnabled() ) {
        qDebug() << "Trying to write password protected file.";

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
                qDebug() << Q_FUNC_INFO << ':' << __LINE__
                         << " - An unknown error occurred.";

                errorMessage( QObject::tr( "An unknown error occurred." ) );

                return QByteArray();
            }
        }
        Q_ASSERT( !qcaByteArray.isEmpty() );

        QCA::SecureArray data = cipher.update( qcaByteArray );
        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - cipher update error.";

            return QByteArray();
        }

        // Because we are using PKCS7 padding, we need to output the final block.
        data.append( cipher.final() );

        // Check if the final() call worked
        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - cipher final error.";

            return QByteArray();
        }

        // write initialisation vector and the encoded data to out
        out << ( iv.toByteArray() );
        out << ( data.toByteArray() );
    }
    else {
        qDebug() << "Writing content without password protection.";

        out << static_cast<quint8>( 0 );
        out << *acc;
    }
#else
    qDebug() << "Writing content without password protection (no QCA support).";

    Q_ASSERT( !acc->isPasswordEnabled() );

    out << static_cast<quint8>( 0 );
    out << *acc;
#endif

    buffer.close();

    if( out.status() != QDataStream::Ok ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - stream status error.";
        return QByteArray();
    }

    Q_ASSERT( !byteArray.isEmpty() );

    ok = true;
    return byteArray;
}


bool Storage::decodedData(const QByteArray &data, Account *acc) const
{
    QByteArray dataArray( data );
    QBuffer dataBuffer( &dataArray );
    dataBuffer.open( QIODevice::ReadOnly );

    QDataStream dataIn( &dataBuffer );
    dataIn.setVersion( QDataStream::Qt_4_6 );

    quint8 sec;
    dataIn >> sec;
    acc->setSecurityLevel( static_cast<Account::SecurityLevel>( sec ) );

    qDebug() << "trying to read file with security level" << sec;

    if( sec == 0 ) {
        dataIn >> *acc;
    }
    else {
        Q_ASSERT( sec == 1 );

#if defined( WITH_QCA2 )
        if( !QCA::isSupported( "aes256-cbc-pkcs7" ) ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - AES data decryption error.";

            errorMessage( QObject::tr( "AES data decryption error." ) );
            return false;
        }

        /// ask user for the password
        QPointer<PasswordDialog> dialog = new PasswordDialog( m_parent );

        if( dialog->exec() != QDialog::Accepted ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__;

            return false;
        }

        if( !dialog ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__ ;

            return false;
        }

        QCA::SecureArray password = dialog->password();
        delete dialog;

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
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - An unknown error occurred (chipher update error).";

            errorMessage( QObject::tr( "An error occurred during the decryption process." ) );

            return false;
        }

        plainData.append( cipher.final() );

        if( !cipher.ok() ) {
            qDebug() << Q_FUNC_INFO << ':' << __LINE__
                     << " - An unknown error occurred (chipher final error).";

            errorMessage( QObject::tr( "An error occurred during the decryption process." ) );

            return false;
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

            errorMessage( QObject::tr( "An unknown error occurred." ) );

            return false;
        }

        acc->setPasswordEnabled( true );
        acc->setPassword( password );
#else
        errorMessage( QObject::tr( "This %1 version was build without support "
                                   "to read encrypted files." ).arg(
                                       QCoreApplication::applicationName() ) );
        return false;
#endif
    }

    if( dataIn.status() != QDataStream::Ok ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
            << " - An unknown error occurred (stream status error"
            << dataIn.status() << ").";

        errorMessage( QObject::tr( "An unknown error occurred." ) );
        return false;
    }

    return true;
}


void Storage::errorMessage(const QString &message) const
{
#if defined(HAVE_KDE)
        KMessageBox::error( m_parent, message );
#else
        QMessageBox::warning( m_parent, // krazy:exclude=qclasses
                              QObject::tr( "Error - %1" )
                                .arg( QCoreApplication::applicationName() ),
                              message
        );
#endif
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
