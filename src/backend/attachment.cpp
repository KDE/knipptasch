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
#include "attachment.h"

#include <QUrl>


struct Attachment::Private
{
    Private()
      : modified( false ),
        inline_data( false )
    {
    }

    bool modified;
    bool inline_data;

    QString title;
    QString documentTyp;
    QString documentNumber;
    QString description;
    QString mimetype;

    QUrl url;

    QByteArray data;
};



Attachment::Attachment()
  : d( new Attachment::Private )
{
}


Attachment::~Attachment()
{
    delete d;
}


bool Attachment::isModified() const
{
    if( d->modified || Object::isModified() ) {
        return true;
    }

    return false;
}


void Attachment::setModified(bool state)
{
    if( state ) {
        d->modified = true;
    }
    else {
        Object::setModified( false );
        d->modified = false;
    }
}


bool Attachment::isEmpty() const
{
    if( isInline() ) {
        return d->url.isValid() && !d->data.isEmpty();
    }

    return d->url.isValid();
}


bool Attachment::isInline() const
{
    return d->inline_data;
}


void Attachment::setInline(bool b)
{
    if( b != d->inline_data ) {
        d->inline_data= b;
        setModified();
    }
}


QString Attachment::title() const
{
    return d->title;
}


void Attachment::setTitle(const QString &str)
{
    if( d->title != str ) {
        d->title = str;
        setModified();
    }
}


QString Attachment::documentType() const
{
    return d->documentTyp;
}


void Attachment::setDocumentType(const QString &str)
{
    if( d->documentTyp != str ) {
        d->documentTyp = str;
        setModified();
    }
}


QString Attachment::documentNumber() const
{
    return d->documentNumber;
}


void Attachment::setDocumentNumber(const QString &str)
{
    if( d->documentNumber != str ) {
        d->documentNumber = str;
        setModified();
    }
}


QString Attachment::description() const
{
    return d->description;
}


void Attachment::setDescription(const QString &str)
{
    if( d->description != str ) {
        d->description = str;
        setModified();
    }
}


QUrl Attachment::url() const
{
    return d->url;
}


void Attachment::setUrl(const QUrl &url)
{
    if( d->url != url ) {
        d->url = url;
        setModified();
    }
}


QString Attachment::mimeType() const
{
    return d->mimetype;
}


void Attachment::setMimeType(const QString &mime)
{
    if( d->mimetype != mime ) {
        d->mimetype = mime;
        setModified();
    }
}


bool Attachment::hasData() const
{
    return !d->data.isEmpty();
}


QByteArray Attachment::data() const
{
    return d->data;
}


void Attachment::setData(const QByteArray &data)
{
    if( d->data != data ) {
        d->data = data;
        setModified();
    }

    Q_ASSERT( d->data == data );
}


void Attachment::clearData()
{
    if( !d->data.isEmpty() ) {
        d->data.clear();
        setModified();
    }
}


QDataStream& Attachment::serialize(QDataStream &stream) const
{
    Object::serialize( stream );

    stream << d->inline_data;
    stream << d->title;
    stream << d->documentTyp;
    stream << d->documentNumber;
    stream << d->description;
    stream << d->mimetype;
    stream << d->url;
    stream << d->data;

    return stream;
}


QDataStream& Attachment::deserialize(QDataStream &stream)
{
    Object::deserialize( stream );

    stream >> d->inline_data;
    stream >> d->title;
    stream >> d->documentTyp;
    stream >> d->documentNumber;
    stream >> d->description;
    stream >> d->mimetype;
    stream >> d->url;
    stream >> d->data;

    return stream;
}


QDataStream& operator<<(QDataStream &stream, const Attachment &att)
{
    return att.serialize( stream );
}


QDataStream& operator>>(QDataStream &stream, Attachment &att)
{
    return att.deserialize( stream );
}




// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
