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
#include "xmlwriter.h"

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



struct XmlWriter::Private
{
    QVector<const Category*> categories;
    QVector<const Posting*> postings;
};



XmlWriter::XmlWriter()
  : d( 0 )
{
}


XmlWriter::~XmlWriter()
{
    delete d;
}


void XmlWriter::write(const Account *acc, const QString &filename)
{
    Q_ASSERT( acc );

    if( filename.isEmpty() ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__ << " - Filename is empty.";

        throw StorageFileException(
              QT_TR_NOOP( "The file given could not be written; check whether "
                          "it exists or is writeable for the current user." ) );
    }

    QFile file( filename );
    if( !file.open( QFile::WriteOnly ) ) {
        qDebug() << Q_FUNC_INFO << ':' << __LINE__
                 << " - File" << filename << "could not be opened.";

        throw StorageFileException(
              QT_TR_NOOP( "The file given could not be written; check whether "
                          "it exists or is writeable for the current user." ) );
    }

    delete d;
    d = new XmlWriter::Private;

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
              QT_TR_NOOP( "The file given could not be written; check whether "
                          "it exists or is writeable for the current user." ) );
    }

    file.close();
}


void XmlWriter::writeAccount(QXmlStreamWriter &stream, const Account *acc)
{
    stream.writeStartElement( "account" );

    //FIXME
    //stream.writeAttribute( "level", QString::number( acc->securityLevel() ) );

    stream.writeTextElement( "name", acc->name() );

    if( !acc->number().isEmpty() ) {
        stream.writeTextElement( "number", acc->number() );
    }

    if( !acc->iban().isEmpty() ) {
        stream.writeTextElement( "iban", acc->iban() );
    }

    if( !acc->description().isEmpty() ) {
        stream.writeTextElement( "description", acc->description() );
    }

    stream.writeStartElement( "opening" );
        stream.writeAttribute( "date", acc->openingDate().toString( Qt::ISODate ) );
        stream.writeCharacters( QString::number( acc->openingBalance().cents() ) );
    stream.writeEndElement(); //opening

    writeLimit( stream, acc->minimumBalanceEnabled(), acc->minimumBalance(),
                acc->maximumBalanceEnabled(), acc->maximumBalance() );

    if( !acc->owner().isEmpty() ) {
        stream.writeTextElement( "owner", acc->owner() );
    }

    if( !acc->bic().isEmpty() || !acc->institution().isEmpty() ) {
        if( acc->institution().isEmpty() ) {
            stream.writeEmptyElement( "institution" );
        }
        else {
            stream.writeStartElement( "institution" );
        }

        if( !acc->bic().isEmpty() ) {
            stream.writeAttribute( "bic", acc->bic() );
        }

        if( !acc->institution().isEmpty() ) {
            stream.writeCharacters( acc->institution() );
            stream.writeEndElement(); //institution
        }
    }

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


void XmlWriter::writeCategory(QXmlStreamWriter &stream, const Category *category)
{
    Q_ASSERT( category );

    stream.writeStartElement( "category" );
    stream.writeAttribute( "id", QString::number( categoryIdentifier( category ) ) );

    stream.writeTextElement( "name", category->name() );
    writeColor( stream, category->color() );
    writeLimit( stream,
                category->minimumLimitEnabled(), category->minimumLimit(),
                category->maximumLimitEnabled(), category->maximumLimit() );

    writeObjectData( stream, category );

    for(int i = 0; i < category->countCategories(); ++i ) {
        writeCategory( stream, category->category( i ) );
    }

    stream.writeEndElement(); // category
}


void XmlWriter::writeBasePosting(QXmlStreamWriter &stream, const BasePosting *posting)
{
    Q_ASSERT( posting );

    writeObjectData( stream, posting );

    stream.writeTextElement( "maturity", posting->maturity().toString( Qt::ISODate ) );
    stream.writeTextElement( "text", posting->postingText() );
    stream.writeTextElement( "amount", QString::number( posting->amount().cents() ) );

    if( posting->valueDate().isValid() ) {
        stream.writeTextElement( "valuedate", posting->valueDate().toString( Qt::ISODate ) );
    }

    if( posting->page() != 0 ) {
        stream.writeTextElement( "page", QString::number( posting->page() ) );
    }

    if( !posting->description().isEmpty() ) {
        stream.writeTextElement( "description", posting->description() );
    }

    if( !posting->voucher().isEmpty() ) {
        stream.writeTextElement( "voucher", posting->voucher() );
    }

    if( posting->warranty().isValid() ) {
        stream.writeTextElement( "warranty", posting->warranty().toString( Qt::ISODate ) );
    }

    if( !posting->methodOfPayment().isEmpty() ) {
        stream.writeTextElement( "methodOfPayment", posting->methodOfPayment() );
    }

    if( !posting->payee().isEmpty() ) {
        stream.writeTextElement( "payee", posting->payee() );
    }

    if( posting->category() ) {
        stream.writeEmptyElement( "category" );
        stream.writeAttribute( "ref", QString::number( categoryIdentifier( posting->category() ) ) );
    }
}


void XmlWriter::writePosting(QXmlStreamWriter &stream, const Posting *posting)
{
    Q_ASSERT( posting );

    stream.writeStartElement( "posting" );
    stream.writeAttribute( "id", QString::number( postingIdentifier( posting ) ) );

    writeBasePosting( stream, posting );

    for(int i = 0; i < posting->countSubPostings(); ++i ) {
        writeSubPosting( stream, posting->subPosting( i ) );
    }
    stream.writeEndElement(); // posting
}


void XmlWriter::writeSubPosting(QXmlStreamWriter &stream, const SubPosting *posting)
{
    Q_ASSERT( posting );

    stream.writeStartElement( "posting" );
    writeBasePosting( stream, posting );
    stream.writeEndElement(); // posting
}


void XmlWriter::writeObjectData(QXmlStreamWriter &stream, const Object *object)
{
    Q_ASSERT( object );

    // Flags
    const QSet<QByteArray> flags = object->flags();
    for(QSet<QByteArray>::const_iterator it = flags.constBegin(); it != flags.constEnd(); ++it) {
        stream.writeTextElement( "flag", (*it) );
    }

    // Attributes
    const QHash<QByteArray, QVariant> attributes = object->attributes();
    for(QHash<QByteArray, QVariant>::const_iterator it = attributes.constBegin(); it != attributes.constEnd(); ++it) {
        stream.writeStartElement( "attribute" );
        stream.writeTextElement( "key", it.key() );
        stream.writeStartElement( "value" );
        writeVariant( stream, it.value() );
        stream.writeEndElement(); // value
        stream.writeEndElement(); // attribute
    }

    // Attachments
    for(int i = 0; i < object->countAttachments(); ++i ) {
        writeAttachment( stream, object->attachment( i ) );
    }
}


void XmlWriter::writeAttachment(QXmlStreamWriter &stream, const Attachment *attachment)
{
    Q_ASSERT( attachment );

    stream.writeStartElement( "attachment" );
    stream.writeAttribute( "inline", attachment->isInline() ? "yes" : "no" );

    stream.writeTextElement( "title", attachment->title() );
    stream.writeTextElement( "documentType", attachment->documentType() );
    stream.writeTextElement( "documentNumber", attachment->documentNumber() );
    stream.writeTextElement( "description", attachment->description() );
    stream.writeTextElement( "url", attachment->url().toString() );
    stream.writeTextElement( "mimeType", attachment->mimeType() );
    stream.writeTextElement( "base64", attachment->data().toBase64() );

    writeObjectData( stream, attachment );

    stream.writeEndElement(); // attachment
}


void XmlWriter::writeVariant(QXmlStreamWriter &stream, const QVariant &value)
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
        writeColor( stream, value.value<QColor>() );
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
            stream.writeStartElement( "item" );
            stream.writeTextElement( "key", it.key() );
            stream.writeStartElement( "value" );
            writeVariant( stream, it.value() );
            stream.writeEndElement(); // value
            stream.writeEndElement(); // item
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
            stream.writeStartElement( "item" );
            stream.writeTextElement( "key", it.key() );
            stream.writeStartElement( "value" );
            writeVariant( stream, it.value() );
            stream.writeEndElement(); // value
            stream.writeEndElement(); // item
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


void XmlWriter::writeColor(QXmlStreamWriter &stream, const QColor &color) const
{
    if( !color.isValid() ) {
        return;
    }

    int r, g, b, a;
    color.getRgb( &r, &g, &b, &a );

    stream.writeEmptyElement( "color" );
    stream.writeAttribute( "red", QString::number( r ) );
    stream.writeAttribute( "green", QString::number( g ) );
    stream.writeAttribute( "blue", QString::number( b ) );
    if( a < 255 ) {
        stream.writeAttribute( "alpha", QString::number( a ) );
    }
}


void XmlWriter::writeLimit(QXmlStreamWriter &stream, bool minEnabled,
                    const Money &min, bool maxEnabled, const Money &max) const
{
    if( !minEnabled && !maxEnabled && min.cents() == 0 && max.cents() == 0 ) {
        return;
    }

    stream.writeStartElement( "limit" );
    if( minEnabled || min.abs().cents() > 0 ) {
        stream.writeStartElement( "minimum" );
        stream.writeAttribute( "enabled", minEnabled ? "yes" : "no" );
        stream.writeCharacters( QString::number( min ) );
        stream.writeEndElement(); //minimum
    }

    if( maxEnabled || max.abs().cents() > 0 ) {
        stream.writeStartElement( "maximum" );
        stream.writeAttribute( "enabled", maxEnabled ? "yes" : "no" );
        stream.writeCharacters( QString::number( max ) );
        stream.writeEndElement(); //maximum
    }
    stream.writeEndElement(); //limit
}


quint32 XmlWriter::categoryIdentifier(const Category *category)
{
    Q_ASSERT( category );

    if( !d->categories.contains( category ) ) {
        d->categories.append( category );
    }

    Q_ASSERT( d->categories.lastIndexOf( category ) >= 0 );
    return d->categories.lastIndexOf( category );
}


quint32 XmlWriter::postingIdentifier(const Posting *posting)
{
    Q_ASSERT( posting );

    if( !d->postings.contains( posting ) ) {
        d->postings.append( posting );
    }

    Q_ASSERT( d->postings.lastIndexOf( posting ) >= 0 );
    return d->postings.lastIndexOf( posting );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
