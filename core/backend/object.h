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
#ifndef OBJECT_H
#define OBJECT_H

#include "knipptasch_core_export.h"

#include <QSet>
#include <QHash>
#include <QByteArray>
#include <QVariant>


class Attachment;
class Account;

#define ASSERT_LIMITED_VARIANT( x )                                          \
    {                                                                        \
        QSet<QVariant::Type> allowedQVariantTypes;                           \
        allowedQVariantTypes << QVariant::Invalid                            \
                             << QVariant::Bool                               \
                             << QVariant::ByteArray                          \
                             << QVariant::Color                              \
                             << QVariant::Date                               \
                             << QVariant::DateTime                           \
                             << QVariant::Double                             \
                             << QVariant::Hash                               \
                             << QVariant::Image                              \
                             << QVariant::Int                                \
                             << QVariant::List                               \
                             << QVariant::LongLong                           \
                             << QVariant::Map                                \
                             << QVariant::Pixmap                             \
                             << QVariant::RegExp                             \
                             << QVariant::String                             \
                             << QVariant::StringList                         \
                             << QVariant::Time                               \
                             << QVariant::UInt                               \
                             << QVariant::ULongLong                          \
                             << QVariant::Url;                               \
        Q_ASSERT( x.isNull() || allowedQVariantTypes.contains( x.type() ) ); \
    } (void) 0


/**
 * @class Object
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class KNIPPTASCH_CORE_EXPORT Object : public QObject
{
    Q_OBJECT

    public:
        virtual bool isModified() const;
        virtual void setModified(bool b = true);

        QByteArray hash() const;

        bool hasFlags() const;
        int countFlags() const;
        bool containsFlag(const QByteArray &flag) const;
        QSet<QByteArray> flags() const;
        void insertFlag(const QByteArray &flag);
        void insertFlags(const QSet<QByteArray> &flags);
        void setFlags(const QSet<QByteArray> &flags);
        void removeFlag(const QByteArray &flag);
        void removeFlags(const QSet<QByteArray> &flags);
        void clearFlags();

        bool hasAttributes() const;
        int countAttributes() const;
        bool containsAttribute(const QByteArray &name) const;
        QSet<QByteArray> attributeNames() const;
        QHash<QByteArray, QVariant> attributes() const;
        QVariant attribute(const QByteArray &name, const QVariant &defaultKey = QVariant()) const;
        void insertAttribute(const QByteArray &name, const QVariant &value);
        void setAttributes(const QHash<QByteArray, QVariant> &attributes);
        void removeAttribute(const QByteArray &name);
        void clearAttributes();

        bool hasAttachments() const;
        int countAttachments() const;
        Attachment* attachment(int index);
        const Attachment* attachment(int index) const;
        Attachment* takeAttachment(int index);
        void insertAttachment(Attachment *attachment);
        void removeAttachment(int index);
        void clearAttachments();

        virtual QDataStream& serialize(QDataStream &stream) const;
        virtual QDataStream& deserialize(const Account *account, QDataStream &stream);

    signals:
        void stateChanged();
        void flagsChanged();
        void attributesChanged();
        void attachmentsChanged();

    protected:
        Object(QObject *parent = 0);
        virtual ~Object();

    private:
        bool m_modified;
        QSet<QByteArray> m_flags;
        QHash<QByteArray, QVariant> m_attributes;
        QList<Attachment*> m_attachments;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
