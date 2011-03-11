/*
 * Copyright 2007-2010  Stefan Böhmann <kde@hilefoks.org>
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
#ifndef MONEY_H
#define MONEY_H

#include "knipptasch_core_export.h"

#include <QtGlobal>
#include <QDataStream>
#include <QHash>
#include <QVariant>


/**
 * @class Money
 * @brief
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class KNIPPTASCH_CORE_EXPORT Money
{
    public:
        /**
         * Constructs a Money initialized with 0.0.
         */
        Money()
            : m_cents( 0 ) {
        }

        /**
         * Constructs a Money initialized with the value of @param d
         */
        Money( double d )
            : m_cents( qRound64( 100 * d ) ) {
        }


        /**
         * Constructs a Money initialized with the value of @param m
         */
        Money( const Money &m )
            : m_cents( m.cents() ) {
        }


        Money &operator=( const Money &m ) {
            m_cents = m.cents();
            return *this;
        }


        Money &operator=( double d ) {
            m_cents = qRound64( 100 * d );
            return *this;
        }


        operator double() const {
            return m_cents / 100.0;
        }


        qint64 cents() const {
            return m_cents;
        }


        void setCents( qint64 value ) {
            m_cents = value;
        }


        Money abs() const {
            if( m_cents >= 0.0 ) {
                return *this;
            } else {
                return Money( *this ) * -1.0;
            }
        }


        Money &operator+=( const Money &m ) {
            m_cents += m.cents();

            return *this;
        }


        Money &operator+=( double d ) {
            m_cents += qRound64( 100 * d );

            return *this;
        }


        Money &operator-=( const Money &m ) {
            m_cents -= m.cents();

            return *this;
        }


        Money &operator-=( double d ) {
            m_cents -= qRound64( 100 * d );

            return *this;
        }


        Money &operator*=( double d ) {
            m_cents = qRound64( m_cents * d );

            return *this;
        }


        Money &operator*=( int i ) {
            m_cents *= i;

            return *this;
        }


        Money &operator/=( double d ) {
            m_cents = qRound64( m_cents / d );

            return *this;
        }


        Money &operator/=( int i ) {
            return operator/=( static_cast<double>( i ) );
        }


        Money &operator++() {
            m_cents += 100;

            return *this;
        }


        Money operator++( int ) {
            Money m( *this );
            operator++();

            return m;
        }


        Money &operator--() {
            m_cents -= 100;

            return *this;
        }


        Money operator--( int ) {
            Money m( *this );
            operator--();

            return m;
        }

        QString toString() const {
            return QString( "%L1" ).arg( m_cents / 100.0 , 0, 'f', 2 );
        }

    private:
        qint64 m_cents;
};


inline Money operator+( const Money &m1, const Money &m2 )
{
    return Money( m1 ) += m2;
}


inline Money operator+( double d, const Money &m )
{
    return m + Money( d );
}


inline Money operator+( const Money &m, double d )
{
    return d + m;
}

inline Money operator-( const Money &m1, const Money &m2 )
{
    return Money( m1 ) -= m2;
}


inline Money operator-( double d, const Money &m )
{
    return m - Money( d );
}


inline Money operator-( const Money &m, double d )
{
    return d - m;
}


inline Money operator*( const Money &m, double d )
{
    return Money( m ) *= d;
}


inline Money operator*( double d, const Money &m )
{
    return m * d;
}


inline Money operator*( const Money &m, int i )
{
    return Money( m ) *= i;
}


inline Money operator*( int i, const Money &m )
{
    return m * i;
}


inline Money operator/( const Money &m, double d )
{
    return Money( m ) /= d;
}


inline Money operator/( const Money &m, int i )
{
    return Money( m ) /= i;
}


inline bool operator==( const Money &m1, const Money &m2 )
{
    return m1.cents() == m2.cents();
}


inline bool operator==( const Money &m, double d )
{
    return m == Money( d );
}


inline bool operator==( double d, const Money &m )
{
    return m == d;
}


inline bool operator!=( const Money &m1, const Money &m2 )
{
    return !( m1 == m2 );
}


inline bool operator!=( const Money &m, double d )
{
    return !( m == d );
}


inline bool operator!=( double d, const Money &m )
{
    return !( d == m );
}


inline bool operator<( const Money &m1, const Money &m2 )
{
    return m1.cents() < m2.cents();
}


inline bool operator<( const Money &m, double d )
{
    return m < Money( d );
}


inline bool operator<( double d, const Money &m )
{
    return m < d;
}


inline int operator>( const Money &m1, const Money &m2 )
{
    return m1.cents() > m2.cents();
}


inline int operator>( const Money &m, double d )
{
    return m > Money( d );
}


inline int operator>( double d, const Money &m )
{
    return m > d;
}

inline int operator<=( const Money &m1, const Money &m2 )
{
    return m1 < m2 || m1 == m2;
}


inline int operator<=( const Money &m, double d )
{
    return m < d || m == d;
}


inline int operator<=( double d, const Money &m )
{
    return d < m || d == m;
}


inline int operator>=( const Money &m1, const Money &m2 )
{
    return m1 > m2 || m1 == m2;
}


inline int operator>=( const Money &m, double d )
{
    return m > d || m == d;
}


inline int operator>=( double d, const Money &m )
{
    return d > m || d == m;
}


inline uint qHash( const Money &m )
{
    return qHash( m.cents() );
}


inline QDataStream &operator<<( QDataStream &stream, const Money &m )
{
    stream << m.cents();

    return stream;
}


inline QDataStream &operator>>( QDataStream &stream, Money &m )
{
    qint64 i;
    stream >> i;
    m.setCents( i );

    return stream;
}

Q_DECLARE_METATYPE( Money )


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
