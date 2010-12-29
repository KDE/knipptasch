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
#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "object.h"

#include <QDate>

class QUrl;


/**
 * @class Attachment
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class Attachment : public Object
{
    Q_OBJECT

    public:
        Attachment(QObject *parent = 0);
        ~Attachment();

        bool isModified() const;
        void setModified(bool state = true);

        bool isEmpty() const;

        bool isInline() const;
        void setInline(bool b);

        QString title() const;
        void setTitle(const QString &str);

        QString documentType() const;
        void setDocumentType(const QString &str);

        QString documentNumber() const;
        void setDocumentNumber(const QString &str);

        QString description() const;
        void setDescription(const QString &str);

        QUrl url() const;
        void setUrl(const QUrl &url);

        QString mimeType() const;
        void setMimeType(const QString &mime);

        bool hasData() const;
        QByteArray data() const;
        void setData(const QByteArray &data);
        void clearData();

        virtual QDataStream& serialize(QDataStream &stream) const;
        virtual QDataStream& deserialize(const Account *account, QDataStream &stream);

    signals:
        void valueChanged();

    private:
        class Private;
        Private * const d;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
