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
#ifndef XMLWRITER_H
#define XMLWRITER_H

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


/**
 * @class XmlWriter
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class XmlWriter
{
    public:
        XmlWriter();
        ~XmlWriter();

        void write(const Account *acc, const QString &filename);

        bool errorOnUnknownElement() const
        {
            return m_errorOnUnknownElements;
        }

        void setErrorOnUnknownElement(bool enabled = true)
        {
            m_errorOnUnknownElements = enabled;
        }

        bool isPasswordProtected() const;
        QByteArray password() const;
        void setPassword(const QByteArray &password);

    private:
        void writeAccount(QXmlStreamWriter &stream, const Account *acc);

        void writeCategory(QXmlStreamWriter &stream, const Category *category);
        void writePosting(QXmlStreamWriter &stream, const Posting *posting);
        void writeBasePosting(QXmlStreamWriter &stream, const BasePosting *posting);
        void writeSubPosting(QXmlStreamWriter &stream, const SubPosting *posting);

        void writeObjectData(QXmlStreamWriter &stream, const Object *object);
        void writeAttachment(QXmlStreamWriter &stream, const Attachment *attachment);

        void writeVariant(QXmlStreamWriter &stream, const QVariant &variant);

        void writeColor(QXmlStreamWriter &stream, const QColor &color) const;

        void writeLimit(QXmlStreamWriter &stream, bool minEnabled,
                        const Money &min, bool maxEnabled, const Money &max) const;

        quint32 categoryIdentifier(const Category *category);
        quint32 postingIdentifier(const Posting *posting);

    private:
        bool m_errorOnUnknownElements;

        class Private;
        Private *d;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
