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
#include "xmlreader.h"

#include "backend/storageexception.h"

#include "backend/account.h"
#include "backend/posting.h"
#include "backend/subposting.h"
#include "backend/category.h"
#include "backend/attachment.h"
#include "backend/money.h"

#include <QBuffer>
#include <QVector>
#include <QFile>
#include <QFileInfo>

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QUrl>
#include <qxmlstream.h>



struct XmlReader::Private
{
};



XmlReader::XmlReader()
  : d( 0 )
{
}


XmlReader::~XmlReader()
{
    delete d;
}


void XmlReader::read(Account *acc, const QString &filename)
{
    Q_ASSERT( acc );

    if( filename.isEmpty() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - Filename is empty.";

        throw StorageFileException(
               QT_TR_NOOP( "The file given could not be read; check whether it "
                           "exists or is readable for the current user." ) );
    }

    QFile file( filename );
    if( !file.open( QFile::ReadOnly ) ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << filename << "could not be opened.";

        throw StorageFileException(
               QT_TR_NOOP( "The file given could not be read; check whether it "
                           "exists or is readable for the current user." ) );
    }

    delete d;
    d = new XmlReader::Private;

    QXmlStreamReader stream( &file );
    if( stream.readNextStartElement() ) {
        if( stream.name() != "knipptasch" ) {
            throw StorageFileException( QT_TR_NOOP( "The file is not an Knipptasch file." ) );
        }

        if( stream.attributes().value( "version" ) != "1.2" ) {
            throw StorageFileVersionException(
                QT_TR_NOOP( "The file is not an Knipptasch version 1.2 file." ) );
        }

        while( stream.readNextStartElement() ) {
            if( stream.name() == "account" ) {
                readAccount( stream, acc );
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException( QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
     }

    file.close();
    acc->setModified( false );
}


void XmlReader::readAccount(QXmlStreamReader &stream, Account *acc)
{
    Q_ASSERT( acc );
    Q_ASSERT( stream.name().toString().toLower() == "account" );

    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            if( token == "name" ) {
                acc->setName( stream.readElementText() );
            }
            else if( token == "number" ) {
                acc->setNumber( stream.readElementText() );
            }
            else if( token == "iban" ) {
                acc->setIban( stream.readElementText() );
            }
            else if( token == "description" ) {
                acc->setDescription( stream.readElementText() );
            }
            else if( token == "opening" ) {
                QDate date;
                Money money;

                if( stream.attributes().hasAttribute( "date" ) ) {
                    date = QDate::fromString(
                                stream.attributes().value( "date" ).toString(),
                                Qt::ISODate
                            );
                }

                bool ok;
                money.setCents( stream.readElementText().toLongLong( &ok ) );

                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), stream );
                }
            }
            else if( token == "limit" ) {
                QPair<QPair<bool, Money>, QPair<bool, Money> > v = readLimit( stream );

                acc->setMaximumBalanceEnabled( v.first.first );
                acc->setMinimumBalance( v.first.second );
                acc->setMaximumBalanceEnabled( v.second.first );
                acc->setMaximumBalance( v.second.second );
            }
            else if( token == "owner" ) {
                acc->setOwner( stream.readElementText() );
            }
            else if( token == "institution" ) {
                if( stream.attributes().hasAttribute( "bic" ) ) {
                    acc->setBic( stream.attributes().value( "bic" ).toString() );
                }
                acc->setInstitution( stream.readElementText() );
            }
            else if( token == "categories" ) {
                readCategories( stream, acc );
            }
            else if( token == "postings" ) {
                readPostings( stream, acc );
            }
            else if( token == "flag" ) {
                readFlag( stream, acc );
            }
            else if( token == "attribute" ) {
                readAttribute( stream, acc );
            }
            else if( token == "attachment" ) {
                readAttachment( stream, acc );
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( token == "account" ) {
                break;
            }
        }
    }
}


void XmlReader::readCategories(QXmlStreamReader &stream, Account *acc)
{
    Q_ASSERT( acc );
    Q_ASSERT( stream.name().toString().toLower() == "categories" );

    //FIXME
/*
    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            if( token == "" ) {
            }
            else if( token == "" ) {
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                            QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( stream.name().toString().toLower() == "categories" ) {
                break;
            }
        }
    }
*/
}


void XmlReader::readPostings(QXmlStreamReader &stream, Account *acc)
{
    Q_ASSERT( acc );
    Q_ASSERT( stream.name().toString().toLower() == "postings" );

    //FIXME
/*
    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            if( token == "" ) {
            }
            else if( token == "" ) {
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                            QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( stream.name().toString().toLower() == "postings" ) {
                break;
            }
        }
    }
*/
}


void XmlReader::readFlag(QXmlStreamReader &stream, Object *object)
{
    Q_ASSERT( object );
    Q_ASSERT( stream.name().toString().toLower() == "flag" );

    object->insertFlag( stream.readElementText().toAscii() );
}


void XmlReader::readAttribute(QXmlStreamReader &stream, Object *object)
{
    Q_ASSERT( object );
    Q_ASSERT( stream.name().toString().toLower() == "attribute" );

    QByteArray key;
    QVariant value;

    while( !stream.atEnd() ) {
        stream.readNext();

        if( stream.isStartElement() ) {
            if( stream.name().toString().toLower() == "key" ) {
                key = stream.readElementText().toAscii();
            }
            else if(  stream.name().toString().toLower() == "value" ) {
                value = readVariant( stream );
            }
            else {
                if( m_errorOnUnknownElements ) {
                throw StorageParserException(
                            QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( stream.name().toString().toLower() == "attribute" ) {
                break;
            }
        }
    }

    object->insertAttribute( key, value );
}


void XmlReader::readAttachment(QXmlStreamReader &stream, Object *object)
{
    Q_ASSERT( stream.name().toString().toLower() == "attachment" );

    bool in = stream.attributes().hasAttribute( "inline" )
             ? stringToBool( stream, stream.attributes().value( "inline" ).toString() )
             : false;

    //FIXME: Potential memory leak if one of the next function throws an exception
    Attachment *attachment = new Attachment;

    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            if( token == "title" ) {
                attachment->setTitle( stream.readElementText() );
            }
            else if( token == "documentType" ) {
                attachment->setDocumentType( stream.readElementText() );
            }
            else if( token == "documentNumber" ) {
                attachment->setDocumentNumber( stream.readElementText() );
            }
            else if( token == "description" ) {
                attachment->setDescription( stream.readElementText() );
            }
            else if( token == "url" ) {
                attachment->setUrl( QUrl( stream.readElementText(), QUrl::TolerantMode ) );
            }
            else if( token == "mimeType" ) {
                attachment->setMimeType( stream.readElementText() );
            }
            else if( token == "base64" ) {
                attachment->setData( QByteArray::fromBase64( stream.readElementText().toAscii() ) );
            }
            else if( token == "flag" ) {
                readFlag( stream, attachment );
            }
            else if( token == "attribute" ) {
                readAttribute( stream, attachment );
            }
            else if( token == "attachment" ) {
                readAttachment( stream, attachment );
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() && token == "attachment" ) {
            break;
        }
    }

    attachment->setInline( in );
    attachment->setModified( false );
    object->insertAttachment( attachment );
}


QVariant XmlReader::readVariant(QXmlStreamReader &xml)
{
    while( !xml.atEnd() ) {
        xml.readNext();
        if( xml.isStartElement() ) {
            const QString name = xml.name().toString().trimmed().toLower();

            if( name == "nil" ) {
                return QVariant();
            }
            else if( name == "boolean" || name == "bool" ) {
                return stringToBool( xml, xml.readElementText() );
            }
            else if( name == "base64" ) {
                return qVariantFromValue<QByteArray>(
                    QByteArray::fromBase64( xml.readElementText().toAscii() ) );
            }
            else if( name == "color" ) {
                return readColor( xml );
            }
            else if( name == "date" ) {
                return QDate::fromString( xml.readElementText(), Qt::ISODate );
            }
            else if( name == "datetime" ) {
                return QDateTime::fromString( xml.readElementText(), Qt::ISODate );
            }
            else if( name == "double" ) {
                bool ok;
                double value = xml.readElementText().toDouble( &ok );
                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), xml );
                }

                return qVariantFromValue<double>( value );
            }
            else if( name == "map" ) {
                const QString maptype = xml.attributes().value( "type" )
                                                        .toString().toLower();

                if( maptype.isEmpty() || maptype == "map" ) {
                    return readMap( xml );
                }
                else if( maptype == "hash" ) {
                    return readHash( xml );
                }

                throw StorageParserAttributeException(
                                QT_TR_NOOP( "Attribute Error" ), "type", xml );
            }
            else if( name == "image" ) {
                const QString imagetype = xml.attributes().value( "type" )
                                                          .toString().toLower();

                const QImage img = QImage::fromData( QByteArray::fromBase64(
                                            xml.readElementText().toAscii() ) );

                if( imagetype.isEmpty() || imagetype == "image" ) {
                    return img;
                }
                else if( imagetype == "pixmap" ) {
                    return QPixmap::fromImage( img );
                }

                throw StorageParserAttributeException(
                                QT_TR_NOOP( "Attribute Error" ), "type", xml );
            }
            else if( name == "int" || name == "i4" || name == "integer" ) {
                bool ok;
                int value = xml.readElementText().toInt( &ok );
                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), xml );
                }

                return qVariantFromValue<int>( value );
            }
            else if( name == "list" ) {
                const QString listtype = xml.attributes().value( "type" )
                                                         .toString().toLower();

                if( listtype.isEmpty() || listtype == "list" ) {
                    return readList( xml );
                }
                else if( listtype == "stringlist" || listtype == "string" ) {
                    return readStringList( xml );
                }

                throw StorageParserAttributeException(
                                QT_TR_NOOP( "Attribute Error" ), "type", xml );
            }
            else if( name == "long" || name == "longlong" ) {
                bool ok;
                qlonglong value = xml.readElementText().toLongLong( &ok );
                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), xml );
                }

                return qVariantFromValue<qlonglong>( value );
            }
            else if( name == "regexp" ) {
                QRegExp::PatternSyntax syntax = QRegExp::RegExp;
                Qt::CaseSensitivity cs = Qt::CaseSensitive;
                bool greedy = true;

                if( xml.attributes().hasAttribute( "syntax" ) ) {
                    const QString s = xml.attributes().value( "syntax" )
                                                      .toString().toLower();

                    if( s == "regex" || s == "default" ) {
                         syntax = QRegExp::RegExp;
                    }
                    else if( s == "regex2" ) {
                        syntax = QRegExp::RegExp2;
                    }
                    else if( s == "wildcard" ) {
                        syntax = QRegExp::Wildcard;
                    }
                    else if( s == "wildcardunix" || s == "wildcard_unix" ) {
                        syntax = QRegExp::WildcardUnix;
                    }
                    else if( s == "fixed" ) {
                        syntax = QRegExp::FixedString;
                    }
                    else if( s == "w3c_xml_schema" || s == "w3cxmlschema") {
                        syntax = QRegExp::W3CXmlSchema11;
                    }
                    else {
                        throw StorageParserAttributeException(
                              QT_TR_NOOP( "Attribute Error" ), "syntax", xml );
                    }
                }

                if( xml.attributes().hasAttribute( "caseSensitiv" ) ) {
                    cs = stringToBool( xml, xml.attributes()
                                    .value( "caseSensitiv" ).toString() )
                                     ? Qt::CaseSensitive : Qt::CaseInsensitive;
                }

                if( xml.attributes().hasAttribute( "greedy" ) ) {
                    greedy = stringToBool( xml, xml.attributes()
                                                .value( "greedy" ).toString() );
                }

                QRegExp exp( xml.readElementText(), cs, syntax );
                exp.setMinimal( !greedy );

                return exp;
            }
            else if( name == "string" ) {
                return xml.readElementText();
            }
            else if( name == "time" ) {
                return QTime::fromString( xml.readElementText(), Qt::ISODate );
            }
            else if( name == "uint" || name == "uinteger"  || name == "unsigned" ) {
                bool ok;
                quint32 value = xml.readElementText().toUInt( &ok );
                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), xml );
                }

                return qVariantFromValue<quint32>( value );
            }
            else if( name == "ulong" || name == "ulonglong" ) {
                bool ok;
                qulonglong value = xml.readElementText().toULongLong( &ok );
                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), xml );
                }

                return qVariantFromValue<qulonglong>( value );
            }
            else if( name == "url" ) {
                return QUrl( xml.readElementText() );
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), xml );
                }

                xml.skipCurrentElement();
            }
        }
    }
    return QVariant();
}


QVariant XmlReader::readHash(QXmlStreamReader &stream)
{
    Q_ASSERT( stream.name().toString().toLower() == "map" );

    QVariantHash map;

    QString key;
    QVariant value;

    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            if( token == "item" ) {
                stream.readNextStartElement();
                if( stream.name().toString().toLower() == "key" ) {
                    key = stream.readElementText();
                }
                else if(  stream.name().toString().toLower() == "value" ) {
                    value = readVariant( stream );
                }
                else {
                    if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                    }

                    stream.skipCurrentElement();
                }
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( token == "map" ) {
                break;
            }
            else if( token == "item" ) {
                map.insert( key, value );

                key.clear();
                value.clear();
            }
        }
    }

    return map;
}


QVariant XmlReader::readMap(QXmlStreamReader &stream)
{
    Q_ASSERT( stream.name().toString().toLower() == "map" );

    QVariantMap map;

    QString key;
    QVariant value;

    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            if( token == "item" ) {
                stream.readNextStartElement();
                if( stream.name().toString().toLower() == "key" ) {
                    key = stream.readElementText();
                }
                else if(  stream.name().toString().toLower() == "value" ) {
                    value = readVariant( stream );
                }
                else {
                    if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                    }

                    stream.skipCurrentElement();
                }
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( token == "map" ) {
                break;
            }
            else if( token == "item" ) {
                map.insert( key, value );

                key.clear();
                value.clear();
            }
        }
    }

    return map;
}


QVariant XmlReader::readList(QXmlStreamReader &stream)
{
    Q_ASSERT( stream.name().toString().toLower() == "list" );

    QVariantList list;

    while( !stream.atEnd() ) {
        stream.readNext();

        if( stream.isStartElement() ) {
            list.append( readVariant( stream ) );
        }
        else if( stream.isEndElement() ) {
            if( stream.name().toString().toLower() == "list" ) {
                break;
            }
        }
    }

    return list;
}


QVariant XmlReader::readStringList(QXmlStreamReader &stream)
{
    Q_ASSERT( stream.name().toString().toLower() == "list" );

    QStringList list;

    while( !stream.atEnd() ) {
        stream.readNext();

        if( stream.isStartElement() ) {
            if( stream.name().toString().toLower() == "string" ) {
                list.append( stream.readElementText() );
            }
            else {
                if( m_errorOnUnknownElements ) {
                    throw StorageParserException(
                                QT_TR_NOOP( "Unknown Element Error" ), stream );
                }

                stream.skipCurrentElement();
            }
        }
        else if( stream.isEndElement() ) {
            if( stream.name().toString().toLower() == "list" ) {
                break;
            }
        }
    }

    return list;
}


QColor XmlReader::readColor(QXmlStreamReader &stream) const
{
    Q_ASSERT( stream.name().toString().toLower() == "color" );

    bool ok;

    int r = stream.attributes().value( "red" ).toString().toInt( &ok );
    if( !ok ) {
        throw StorageParserAttributeException(
                            QT_TR_NOOP( "Attribute Error" ), "red", stream );
    }

    int g = stream.attributes().value( "green" ).toString().toInt( &ok );
    if( !ok ) {
        throw StorageParserAttributeException(
                            QT_TR_NOOP( "Attribute Error" ), "green", stream );
    }

    int b = stream.attributes().value( "blue" ).toString().toInt( &ok );
    if( !ok ) {
        throw StorageParserAttributeException(
                            QT_TR_NOOP( "Attribute Error" ), "green", stream );
    }

    int a = 255;
    if( stream.attributes().hasAttribute( "alpha" ) ) {
        a = stream.attributes().value( "alpha" ).toString().toInt( &ok );

        if( !ok ) {
            throw StorageParserAttributeException(
                            QT_TR_NOOP( "Attribute Error" ), "alpha", stream );
        }
    }

    return QColor( r, g, b, a );
}


QPair<QPair<bool,Money>, QPair<bool, Money> > XmlReader::readLimit(QXmlStreamReader &stream) const
{
    Q_ASSERT( stream.name().toString().toLower() == "limit" );

    QPair<bool, Money> min;
    QPair<bool, Money> max;

    min.first = false;
    max.first = false;

    while( !stream.atEnd() ) {
        stream.readNext();
        const QString token = stream.name().toString().toLower();

        if( stream.isStartElement() ) {
            bool enabled;
            Money money;

            if( token == "minimum" || token == "maximim" ) {
                if( stream.attributes().hasAttribute( "enabled" ) ) {
                    enabled = stringToBool( stream, stream.attributes().value( "enabled" ).toString() );
                }
                bool ok;
                money.setCents( stream.readElementText().toLongLong( &ok ) );

                if( !ok ) {
                    throw StorageParserException( QT_TR_NOOP( "Parser Error" ), stream );
                }

                if( token == "minimum" ) {
                    min.first = enabled;
                    min.second = money;
                }
                else {
                    max.first = enabled;
                    max.second = money;
                }
            }
            else {
                throw StorageParserException( QT_TR_NOOP( "Unknown Element Error" ), stream );
            }
        }
        else if( stream.isEndElement() ) {
            if( token == "limit" ) {
                break;
            }
        }
    }

    QPair<QPair<bool,Money>, QPair<bool, Money> > re;
    re.first = min;
    re.second = max;

    return re;
}


bool XmlReader::stringToBool(QXmlStreamReader &xml, const QString &str) const
{
    const QString in = str.trimmed().toLower();

    if( in == "1" || in == "yes" || in == "true" ) {
        return true;
    }
    else if( in == "0" || in == "no" || in == "false" ) {
        return false;
    }

    throw StorageParserException( QT_TR_NOOP( "Illegal Value" ), xml );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
