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
#include "storage2.h"

#include "account.h"
#include "posting.h"
#include "money.h"

#include <QBuffer>
#include <QFile>
#include <QFileInfo>

#include <QDebug>
#include <qxmlstream.h>
#include "category.h"
#include <QImage>
#include <QPixmap>
#include <QUrl>
#include "storageexception.h"

#define FixStringAndThenMakeTranslable QString



struct Storage2::Private
{
};



Storage2::Storage2()
  : d( 0 )
{
}


Storage2::~Storage2()
{
    delete d;
}


void Storage2::write(Account *acc, const QString &filename)
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

    delete d;
    d = new Storage2::Private;

    initWriter( acc );

    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );

    QXmlStreamWriter stream( &buffer );
    stream.setAutoFormatting( true );
    stream.setAutoFormattingIndent( 4 );
    stream.writeStartDocument( "1.0" );

    stream.writeStartElement("knipptasch");
    stream.writeAttribute( "version", "1.2" );
    stream.writeAttribute( "href", "http://projects.kde.org/knipptasch" );

    writeAccount( stream, acc );

    stream.writeEndElement(); // knipptasch
    stream.writeEndDocument();

    buffer.close();

    //TODO write to a temporary file first or make a backup or...
    qint64 size = file.write( byteArray );
    if( size <= 0 || size != byteArray.size() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << filename << "could not be written ("
                 << size << "out of" << byteArray.size() << ").";

        throw StorageFileException(
                QObject::tr( "The file given could not be written; check "
                              "whether it exists or is writeable for the "
                              "current user." ) );
    }

    acc->setModified( false );
}


void Storage2::writeAccount(QXmlStreamWriter &stream, const Account *acc)
{
    stream.writeStartElement( "account" );
    stream.writeAttribute( "level", QString::number( acc->securityLevel() ) );

    stream.writeTextElement( "number", acc->number() );
    stream.writeTextElement( "iban", acc->iban() );
    stream.writeTextElement( "name", acc->name() );
    stream.writeTextElement( "description", acc->description() );
    stream.writeStartElement( "opening" );
        stream.writeAttribute( "date", acc->openingDate().toString( Qt::ISODate ) );
        stream.writeAttribute( "balance", QString::number( acc->openingBalance() ) );
    stream.writeEndElement(); //opening

    stream.writeStartElement( "limit" );
        stream.writeStartElement( "minimum" );
            stream.writeAttribute( "enabled", acc->minimumBalanceEnabled() ? "yes" : "no" );
            stream.writeAttribute( "value", QString::number( acc->minimumBalance() ) );
        stream.writeEndElement(); //minimum

        stream.writeStartElement( "maximum" );
            stream.writeAttribute( "enabled", acc->maximumBalanceEnabled() ? "yes" : "no" );
            stream.writeAttribute( "value", QString::number( acc->maximumBalance() ) );
        stream.writeEndElement(); //maximum
    stream.writeEndElement(); //limit

    stream.writeTextElement( "owner", acc->owner() );
    stream.writeStartElement( "institution" );
        stream.writeAttribute( "bic", acc->bic() );
        stream.writeCharacters( acc->institution() );
    stream.writeEndElement(); //institution

    writeObjectData( stream, acc );

    // write categories...
    if( acc->rootCategory()->countCategories() <= 0 ) {
        stream.writeEmptyElement( "categories" );
    }
    else {
        stream.writeStartElement( "categories" );
        for(int i = 0; i < acc->rootCategory()->countCategories(); ++i) {
            writeCategory( stream, acc->rootCategory()->category( i ) );
        }
        stream.writeEndElement();
    }

    // write postings...
    if( acc->countPostings() <= 0 ) {
        stream.writeEmptyElement( "postings" );
    }
    else {
        stream.writeStartElement( "postings" );
        for(int i = 0; i < acc->countPostings(); ++i) {
            writePosting( stream, acc->posting( i ) );
        }
        stream.writeEndElement();
    }

    stream.writeEndElement(); // account
}


void Storage2::initWriter(Account *acc)
{

}


void Storage2::writeCategory(QXmlStreamWriter &stream, const Category *category)
{
    stream.writeStartElement( "category" );
    writeObjectData( stream, category );

    //TODO

    for(int i = 0; i < category->countCategories(); ++i ) {
        writeCategory( stream, category->category( i ) );
    }
    stream.writeEndElement(); // category
}


void Storage2::writeBasePosting(QXmlStreamWriter &stream, const BasePosting *posting)
{
    writeObjectData( stream, posting );

    //TODO
}

void Storage2::writePosting(QXmlStreamWriter &stream, const Posting *posting)
{
    stream.writeStartElement( "posting" );
    writeBasePosting( stream, posting );

    for(int i = 0; i < posting->countSubPostings(); ++i ) {
        writeCategory( stream, posting->subPosting( i ) );
    }
    stream.writeEndElement(); // posting
}


void Storage2::writeSubPosting(QXmlStreamWriter &stream, const SubPosting *posting)
{
    stream.writeStartElement( "posting" );
    writeBasePosting( stream, posting );

    //TODO

    stream.writeEndElement(); // posting

}


void Storage2::writeObjectData(QXmlStreamWriter &stream, const Object *object)
{
    Q_ASSERT( object );

    const QSet<QByteArray> flags = object->flags();

    for(QSet<QByteArray>::const_iterator it = flags.constBegin(); it != flags.constEnd(); ++it) {
        stream.writeTextElement( "flag", (*it) );
    }


    const QHash<QByteArray, QVariant> attributes = object->attributes();
    for(QHash<QByteArray, QVariant>::const_iterator it = attributes.constBegin(); it != attributes.constEnd(); ++it) {
        stream.writeStartElement( "attribute" );
        stream.writeAttribute( "name", it.key() );
        writeVariant( stream, it.value() );
        stream.writeEndElement();
    }

    for(int i = 0; i < object->countAttachments(); ++i ) {
        writeAttachment( stream, object->attachment( i ) );
    }
}


void Storage2::writeAttachment(QXmlStreamWriter &stream, const Attachment *attachment)
{
    stream.writeStartElement( "attachment" );

    //TODO

    stream.writeEndElement(); // attachment
}



void Storage2::writeVariant(QXmlStreamWriter &stream, const QVariant &value)
{
    ASSERT_LIMITED_VARIANT( value );

    if( value.isNull() || value.type() == QVariant::Invalid ) {
        stream.writeEmptyElement( "nil" );
    }
    else if( value.type() == QVariant::Bool ) {
        stream.writeTextElement( "boolean", value.toBool() ? "true" : "false" );
    }
    else if( value.type() == QVariant::ByteArray ) {
        stream.writeTextElement( "base64", value.toByteArray().toBase64() );
    }
    else if( value.type() == QVariant::Color ) {
        int r, g, b, a;
        QColor c =  value.value<QColor>();
        c.getRgb( &r, &g, &b, &a );

        stream.writeEmptyElement( "color" );
        stream.writeAttribute( "red", QString::number( r ) );
        stream.writeAttribute( "green", QString::number( g ) );
        stream.writeAttribute( "blue", QString::number( b ) );
        if( a < 255 ) {
            stream.writeAttribute( "alpha", QString::number( a ) );
        }
    }
    else if( value.type() == QVariant::Date ) {
        stream.writeTextElement( "date", value.toDate().toString( Qt::ISODate ) );
    }
    else if( value.type() == QVariant::DateTime ) {
        stream.writeTextElement( "datetime", value.toDateTime().toString( Qt::ISODate ) );
    }
    else if( value.type() == QVariant::Double ) {
        stream.writeTextElement( "double", QString::number( value.toDouble() ) );
    }
    else if( value.type() == QVariant::Hash ) {
        QHash<QString, QVariant> hash = value.toHash();
        QHashIterator<QString, QVariant> it( hash );

        stream.writeStartElement( "map" );
        stream.writeAttribute( "type", "hash" );
        while( it.hasNext() ) {
            it.next();
            stream.writeStartElement( "value" );
            stream.writeAttribute( "name", it.key() );
            writeVariant( stream, it.value() );
            stream.writeEndElement();
        }
        stream.writeEndElement();
    }
    else if( value.type() == QVariant::Image ) {
        QByteArray ba;
        QBuffer buffer( &ba );
        buffer.open( QIODevice::WriteOnly );
        value.value<QImage>().save( &buffer, "PNG" );
        buffer.close();

        stream.writeTextElement( "image", ba.toBase64() );
    }
    else if( value.type() == QVariant::Int ) {
        stream.writeTextElement( "int", QString::number( value.toInt() ) );
    }
    else if( value.type() == QVariant::List ) {
        stream.writeStartElement( "list" );
        const QVariantList list = value.toList();
        foreach(const QVariant &var, list) {
            writeVariant( stream, var );
        }
        stream.writeEndElement();
    }
    else if( value.type() == QVariant::LongLong ) {
        stream.writeTextElement( "long", QString::number( value.toLongLong() ) );
    }
    else if( value.type() == QVariant::Map ) {
        QMap<QString, QVariant> map = value.toMap();
        QMapIterator<QString, QVariant> it( map );

        stream.writeStartElement( "map" );
        while( it.hasNext() ) {
            it.next();
            stream.writeStartElement( "value" );
            stream.writeAttribute( "name", it.key() );
            writeVariant( stream, it.value() );
            stream.writeEndElement();
        }
        stream.writeEndElement();
    }
    else if( value.type() == QVariant::Pixmap ) {
        QByteArray ba;
        QBuffer buffer( &ba );
        buffer.open( QIODevice::WriteOnly );
        value.value<QPixmap>().save( &buffer, "PNG" );
        buffer.close();

        stream.writeStartElement( "image" );
        stream.writeAttribute( "type", "pixmap" );
        stream.writeCharacters( ba.toBase64() );
        stream.writeEndElement();
    }
    else if( value.type() == QVariant::RegExp ) {
        const QRegExp exp = value.toRegExp();

        stream.writeStartElement( "regexp" );
        switch( exp.patternSyntax() ) {
        case QRegExp::RegExp:
            //stream.writeAttribute( "syntax", "regex" );
            break;
        case QRegExp::RegExp2:
            stream.writeAttribute( "syntax", "regex2" );
            break;
        case QRegExp::Wildcard:
            stream.writeAttribute( "syntax", "wildcard" );
            break;
        case QRegExp::WildcardUnix:
            stream.writeAttribute( "syntax", "wildcardUnix" );
            break;
        case QRegExp::FixedString:
            stream.writeAttribute( "syntax", "fixed" );
            break;
        case QRegExp::W3CXmlSchema11:
            stream.writeAttribute( "syntax", "W3C_XML_Schema" );
            break;
        }

        if( exp.caseSensitivity() == Qt::CaseInsensitive ) {
            stream.writeAttribute( "caseSensitiv", "no" );
        }

        if( exp.isMinimal() ) {
            stream.writeAttribute( "greedy", "no" );
        }

        stream.writeCharacters( exp.pattern() );
        stream.writeEndElement();
    }
    else if( value.type() == QVariant::String ) {
        stream.writeTextElement( "string", value.toString() );
    }
    else if( value.type() == QVariant::StringList ) {
        const QStringList list = value.toStringList();

        stream.writeStartElement( "list" );
        stream.writeAttribute( "type", "stringlist" );
        foreach(const QString &str, list) {
            stream.writeTextElement( "string", str );
        }
        stream.writeEndElement(); // list
    }
    else if( value.type() == QVariant::Time ) {
        stream.writeTextElement( "time", value.toTime().toString( Qt::ISODate ) );
    }
    else if( value.type() == QVariant::UInt ) {
        stream.writeTextElement( "uint", QString::number( value.toUInt() ) );
    }
    else if( value.type() == QVariant::ULongLong ) {
        stream.writeTextElement( "ulong", QString::number( value.toULongLong() ) );
    }
    else if( value.type() == QVariant::Url ) {
        stream.writeTextElement( "url", value.toUrl().toString() );
    }
}



QVariant Storage2::readVariant(QXmlStreamReader &xml)
{
    while( !xml.atEnd() ) {
        xml.readNext();
        if( xml.isStartElement() ) {
            const QString name = xml.name().toString().trimmed().toLower();

            if( name == "nil" ) {
                return QVariant();
            }
            else if( name == "boolean" ) {
                return readBoolFromString( xml, xml.readElementText() );
            }
            else if( name == "base64" ) {
                return qVariantFromValue<QByteArray>( QByteArray::fromBase64( xml.readElementText().toAscii() ) );
            }
            else if( name == "color" ) {
                bool ok;
                int r = xml.attributes().value( "red" ).toString().toInt( &ok );
                if( !ok ) {
                    throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "red", xml );
                }

                int g = xml.attributes().value( "green" ).toString().toInt( &ok );
                if( !ok ) {
                    throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "green", xml );
                }

                int b = xml.attributes().value( "blue" ).toString().toInt( &ok );
                if( !ok ) {
                    throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "green", xml );
                }

                int a = 255;
                if( xml.attributes().hasAttribute( "alpha" ) ) {
                    a = xml.attributes().value( "alpha" ).toString().toInt( &ok );
                    if( !ok ) {
                        throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "alpha", xml );
                    }
                }

                return QColor( r, g, b, a );
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
                    throw StorageParserException( FixStringAndThenMakeTranslable( "Parser Error" ), xml );
                }

                return qVariantFromValue<double>( value );
            }
            else if( name == "map" ) {
                const QString maptype = xml.attributes().value( "type" ).toString().trimmed().toLower();

                if( maptype.isEmpty() || maptype == "map" ) {
                    return readMap( xml );
                }
                else if( maptype == "hash" ) {
                    return readHash( xml );
                }

                throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "type", xml );
            }
            else if( name == "image" ) {
                const QString imagetype = xml.attributes().value( "type" ).toString().trimmed().toLower();

                const QImage img = QImage::fromData( QByteArray::fromBase64( xml.readElementText().toAscii() ) );

                if( imagetype.isEmpty() || imagetype == "image" ) {
                    return img;
                }
                else if( imagetype == "pixmap" ) {
                    return QPixmap::fromImage( img );
                }

                throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "type", xml );
            }
            else if( name == "int" || name == "i4" || name == "integer" ) {
                bool ok;
                int value = xml.readElementText().toInt( &ok );
                if( !ok ) {
                    throw StorageParserException( FixStringAndThenMakeTranslable( "Parser Error" ), xml );
                }

                return qVariantFromValue<int>( value );
            }
            else if( name == "list" ) {
                const QString listtype = xml.attributes().value( "type" ).toString().trimmed().toLower();

                if( listtype.isEmpty() || listtype == "list" ) {
                    return readList( xml );
                }
                else if( listtype == "stringlist" || listtype == "string" ) {
                    return readStringList( xml );
                }

                throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "type", xml );
            }
            else if( name == "long" || name == "longlong" ) {
                bool ok;
                qlonglong value = xml.readElementText().toLongLong( &ok );
                if( !ok ) {
                    throw StorageParserException( FixStringAndThenMakeTranslable( "Parser Error" ), xml );
                }

                return qVariantFromValue<qlonglong>( value );
            }
            else if( name == "regexp" ) {
                QRegExp::PatternSyntax syntax = QRegExp::RegExp;
                Qt::CaseSensitivity cs = Qt::CaseSensitive;
                bool greedy = true;

                if( xml.attributes().hasAttribute( "syntax" ) ) {
                    const QString s = xml.attributes().value( "syntax" ).toString().trimmed().toLower();

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
                        throw StorageParserAttributeException( FixStringAndThenMakeTranslable( "Attribute Error" ), "syntax", xml );
                    }
                }

                if( xml.attributes().hasAttribute( "caseSensitiv" ) ) {
                    cs = readBoolFromString( xml, xml.attributes().value( "caseSensitiv" ).toString() )
                            ? Qt::CaseSensitive
                            : Qt::CaseInsensitive;
                }

                if( xml.attributes().hasAttribute( "greedy" ) ) {
                    greedy = readBoolFromString( xml, xml.attributes().value( "greedy" ).toString() );
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
                    throw StorageParserException( FixStringAndThenMakeTranslable( "Parser Error" ), xml );
                }

                return qVariantFromValue<quint32>( value );
            }
            else if( name == "ulong" || name == "ulonglong" ) {
                bool ok;
                qulonglong value = xml.readElementText().toULongLong( &ok );
                if( !ok ) {
                    throw StorageParserException( FixStringAndThenMakeTranslable( "Parser Error" ), xml );
                }

                return qVariantFromValue<qulonglong>( value );
            }
            else if( name == "url" ) {
                return QUrl( xml.readElementText() );
            }

            throw StorageParserException( FixStringAndThenMakeTranslable( "Unknown Element" ), xml );
        }
    }
    return QVariant();
}


QVariant Storage2::readHash(QXmlStreamReader &xml)
{
    if( xml.name().toString() != "map" ) {
        throw StorageParserException( FixStringAndThenMakeTranslable( "Unknown Element" ), xml );
    }
/*
    while( !xml.atEnd() ) {
        xml.readNext();
        if( xml.isStartElement() ) {
            const QString name = xml.name().toString().trimmed().toLower();
*/
    return QVariant();
}


QVariant Storage2::readMap(QXmlStreamReader &stream)
{
    return QVariant();
}


QVariant Storage2::readList(QXmlStreamReader &stream)
{
    return QVariant();
}


QVariant Storage2::readStringList(QXmlStreamReader &stream)
{
    return QVariant();
}


bool Storage2::readBoolFromString(QXmlStreamReader &xml, const QString &str)
{
    const QString in = str.trimmed().toLower();

    if( in == "1" || in == "yes" || in == "true" ) {
        return true;
    }
    else if( in == "0" || in == "no" || in == "false" ) {
        return false;
    }

    throw StorageParserException( FixStringAndThenMakeTranslable( "Illegal Value" ), xml );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
