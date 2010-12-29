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
#ifndef POSTING_H
#define POSTING_H

#include "baseposting.h"

class SubPosting;


/**
 * @class Posting
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class Posting : public BasePosting
{
    Q_OBJECT

    public:
        Posting(QObject *parent = 0);
        ~Posting();

        bool isModified() const;
        void setModified(bool state = true);

        bool hasSubPostings() const;
        int countSubPostings() const;
        SubPosting* subPosting(int index);
        const SubPosting* subPosting(int index) const;
        SubPosting* takeSubPosting(int index);
        void addSubPosting(SubPosting *posting);
        void removeSubPosting(int index);
        void clearSubPostings();

        virtual QDataStream& serialize(QDataStream &stream) const;
        virtual QDataStream& deserialize(const Account *account, QDataStream &stream);

    signals:
        void subPostingChanged();

    private:
        class Private;
        Private * const d;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
