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
#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>
#include <QtGlobal>
#include <QPair>

class Object;
class Account;
class Category;
class Attachment;
class Money;
class BasePosting;
class SubPosting;
class Posting;

class QVariant;
class QColor;
class QByteArray;
class QXmlStreamWriter;
class QXmlStreamReader;


class XmlReader
{
    public:
        XmlReader();
        ~XmlReader();

        void read(Account *acc, const QString &filename);

        bool isPasswordProtected() const;
        QByteArray password() const;
        void setPassword(const QByteArray &password);

    private:
        void readAccount(QXmlStreamReader &stream, Account *acc);

        void readCategories(QXmlStreamReader &stream, Account *acc);
        void readPostings(QXmlStreamReader &stream, Account *acc);

        void readFlag(QXmlStreamReader &stream, Object *object);
        void readAttribute(QXmlStreamReader &stream, Object *object);

        void readAttachment(QXmlStreamReader &stream, Object *object);

        QVariant readVariant(QXmlStreamReader &stream);
        QVariant readHash(QXmlStreamReader &stream);
        QVariant readMap(QXmlStreamReader &stream);
        QVariant readList(QXmlStreamReader &stream);
        QVariant readStringList(QXmlStreamReader &stream);

        QColor readColor(QXmlStreamReader &stream) const;

        QPair<QPair<bool,Money>, QPair<bool, Money> > readLimit(QXmlStreamReader &stream) const;

        bool stringToBool(QXmlStreamReader &stream, const QString &str) const;

    private:
        bool m_errorOnUnknownElements;

        class Private;
        Private *d;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
