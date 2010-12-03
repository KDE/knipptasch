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
#include "category.h"

#include "money.h"


struct Category::Private
{
    Private()
      : enableminlimit( false ),
        enablemaxlimit( false ),
        minrule( Monthly ),
        maxrule( Monthly ),
        modified( false )
    {
    }

    ~Private()
    {
        qDeleteAll( categories );
    }

    QList<Category*> categories;

    QString name;
    QColor color;
    bool enableminlimit;
    bool enablemaxlimit;
    Money minlimit;
    Money maxlimit;
    ForwardRule minrule;
    ForwardRule maxrule;
    bool modified;
};




Category::Category()
  : d( new Category::Private )
{
}


Category::~Category()
{
    delete d;
}


bool Category::isModified() const
{
    if( d->modified || Object::isModified() ) {
        return true;
    }

    foreach(const Category *c, d->categories) {
        Q_ASSERT( c );
        if( c->isModified() ) {
            d->modified = true;
            return true;
        }
    }

    return false;
}


void Category::setModified(bool state)
{
    if( state ) {
        d->modified = true;
    }
    else {
        foreach(Category *c, d->categories) {
            c->setModified( false );
        }

        Object::setModified( false );
        d->modified = false;
    }
}


QString Category::name() const
{
    Q_ASSERT( d );

    return d->name;
}


void Category::setName(const QString &name)
{
    Q_ASSERT( d );

    if( d->name != name ) {
        d->name = name;
        setModified();
    }
}


QColor Category::color() const
{
    Q_ASSERT( d );

    return d->color;
}


void Category::setColor(const QColor &color)
{
    Q_ASSERT( d );

    if( d->color != color ) {
        d->color = color;
        setModified();
    }
}


bool Category::maximumLimitEnabled() const
{
    Q_ASSERT( d );

    return d->enablemaxlimit;
}


void Category::setMaximumLimitEnabled(bool b)
{
    Q_ASSERT( d );

    if( d->enablemaxlimit != b ) {
        d->enablemaxlimit = b;
        setModified();
    }
}


Money Category::maximumLimit() const
{
    Q_ASSERT( d );

    return d->maxlimit;
}


void Category::setMaximumLimit(const Money &m)
{
    Q_ASSERT( d );

    if( d->maxlimit != m ) {
        d->maxlimit = m;
        setModified();
    }
}


Category::ForwardRule Category::maximumUnit() const
{
    Q_ASSERT( d );

    return d->maxrule;
}


void Category::setMaximumUnit(Category::ForwardRule rule)
{
    Q_ASSERT( d );

    if( d->maxrule != rule ) {
        d->maxrule = rule;
        setModified();
    }
}


bool Category::minimumLimitEnabled() const
{
    Q_ASSERT( d );

    return d->enableminlimit;
}


void Category::setMinimumLimitEnabled(bool b)
{
    Q_ASSERT( d );

    if( d->enableminlimit != b ) {
        d->enableminlimit = b;
        setModified();
    }
}


Money Category::minimumLimit() const
{
    Q_ASSERT( d );

    return d->minlimit;
}


void Category::setMinimumLimit(const Money &m)
{
    Q_ASSERT( d );

    if( d->minlimit != m ) {
        d->minlimit = m;
        setModified();
    }
}


Category::ForwardRule Category::minimumUnit() const
{
    Q_ASSERT( d );

    return d->minrule;
}


void Category::setMinimumUnit(Category::ForwardRule rule)
{
    Q_ASSERT( d );

    if( d->minrule != rule ) {
        d->minrule = rule;
        setModified();
    }
}


bool Category::hasCategories() const
{
    Q_ASSERT( d );

    return !d->categories.isEmpty();
}


int Category::countCategories() const
{
    Q_ASSERT( d );

    return d->categories.size();
}


const Category* Category::category(int index) const
{
    Q_ASSERT( d );
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->categories.size() );

    return d->categories.at( index );
}


Category* Category::category(int index)
{
    Q_ASSERT( d );
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->categories.size() );

    return d->categories[ index ];
}


Category* Category::takeCategory(int index)
{
    Q_ASSERT( d );
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->categories.size() );

    Category *c = d->categories.takeAt( index );
    setModified();

    return c;
}


void Category::addCategory(Category* c)
{
    Q_ASSERT( d );
    Q_ASSERT( c );

    d->categories.append( c );
    setModified();
}


void Category::removeCategory(int index)
{
    Q_ASSERT( index >= 0 );
    Q_ASSERT( index < d->categories.size() );

    d->categories.removeAt( index );
    setModified();
}


void Category::clearCategories()
{
    if( !d->categories.isEmpty() ) {
        d->categories.clear();
        setModified();
    }
}


const Category* Category::findCategoryByHash(const QByteArray &h) const
{
    if( hash() == h ) {
        return this;
    }

    foreach(const Category *c, d->categories) {
        if( c->hash() == h ) {
            return c;
        }
    }

    return 0;
}


Category* Category::findCategoryByHash(const QByteArray &h)
{
    if( hash() == h ) {
        return this;
    }

    foreach(Category *c, d->categories) {
        if( c->hash() == h ) {
            return c;
        }
    }

    return 0;
}


QDataStream& Category::serialize(QDataStream &stream) const
{
    Object::serialize( stream );

    stream << d->name;
    stream << d->color;
    stream << d->enableminlimit;
    stream << d->minlimit;
    stream << static_cast<quint32>( d->minrule );
    stream << d->enablemaxlimit;
    stream << d->maxlimit;
    stream << static_cast<quint32>( d->maxrule );

    stream << static_cast<quint32>( d->categories.size() );
    foreach(Category *c, d->categories) {
        c->serialize( stream );
    }

    return stream;
}


QDataStream& Category::deserialize(const Account *account, QDataStream &stream)
{
    quint32 v;

    Object::deserialize( account, stream );

    stream >> d->name;
    stream >> d->color;
    stream >> d->enableminlimit;
    stream >> d->minlimit;
    stream >> v;
    d->minrule = static_cast<ForwardRule>( v );
    stream >> d->enablemaxlimit;
    stream >> d->maxlimit;
    stream >> v;
    d->maxrule = static_cast<ForwardRule>( v );

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i) {
        Category *c = new Category;
        c->deserialize( account, stream );
        addCategory( c );
    }

    return stream;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
