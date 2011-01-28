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
#include "posting.h"
#include "subposting.h"
#include "money.h"

#include <QDataStream>


struct Posting::Private
{
    Private()
      : modified( false )
    {
    }

    ~Private()
    {
        while( !postings.isEmpty() ) {
            delete postings.takeFirst();
        }
    }

    QList<SubPosting*> postings;
    mutable bool modified;
};




Posting::Posting(QObject *parent)
  : BasePosting( parent ),
    d( new Posting::Private )
{
}


Posting::~Posting()
{
    delete d;
}


bool Posting::isModified() const
{
    if( d->modified || BasePosting::isModified() ) {
        return true;
    }

    foreach(const SubPosting *p, d->postings) {
        if( p && p->isModified() ) {
            d->modified = true;
            return true;
        }
    }

    return false;
}


void Posting::setModified(bool state)
{
    if( state ) {
        d->modified = true;
    }
    else {
        foreach(SubPosting *p, d->postings) {
            p->setModified( false );
        }

        d->modified = false;
    }

    BasePosting::setModified( state );
}


bool Posting::hasSubPostings() const
{
    return countSubPostings() > 0;
}


int Posting::countSubPostings() const
{
    return d->postings.size();
}


SubPosting* Posting::subPosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    return d->postings.at( index );
}


const SubPosting* Posting::subPosting(int index) const
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    return d->postings.at( index );
}


SubPosting* Posting::takeSubPosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    SubPosting *p = d->postings.takeAt( index );
    disconnect( p, 0, this, 0 );

    setModified();
    emit subPostingChanged();

    return p;
}


void Posting::addSubPosting(SubPosting *p)
{
    Q_ASSERT( p );

    d->postings.append( p );
    connect( p, SIGNAL( valueChanged() ), this, SIGNAL( subPostingChanged() ) );
    connect( p, SIGNAL( categoryChanged()), this, SIGNAL( subPostingChanged() ) );

    setModified();
    emit subPostingChanged();
}


void Posting::removeSubPosting(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->postings.size() );

    SubPosting *p = d->postings.takeAt( index );
    disconnect( p, 0, this, 0 );

    setModified();
    emit subPostingChanged();
}


void Posting::clearSubPostings()
{
    if( d->postings.isEmpty() ) {
        return;
    }

    while( !d->postings.isEmpty() ) {
        SubPosting *p = d->postings.takeFirst();
        disconnect( p, 0, this, 0 );
    }

    setModified();
    emit subPostingChanged();
}


QDataStream& Posting::serialize(QDataStream &stream) const
{
    BasePosting::serialize( stream );

    stream << static_cast<quint32>( d->postings.size() );
    foreach(SubPosting *p, d->postings) {
        p->serialize( stream );
    }

    return stream;
}


QDataStream& Posting::deserialize(const Account *account, QDataStream &stream)
{
    BasePosting::deserialize( account, stream );

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i) {
        SubPosting *p = new SubPosting;
        p->deserialize( account, stream );
        addSubPosting( p );
    }

    return stream;
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
