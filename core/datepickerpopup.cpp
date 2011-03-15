/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * Based on KDateEdit (part of libkdepim)
 *   Copyright (c) 2004 Bram Schoenmakers <bramschoenmakers@kde.nl>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "datepickerpopup.h"

#if defined(HAVE_KDE)
#include <KDatePicker>
#include <KLocale>
#else
#include <QCalendarWidget>
#endif

#include <QDateTime>
#include <QWidgetAction>



namespace Knipptasch
{
    


class DatePickerAction : public QWidgetAction
{
    public:
#if defined(HAVE_KDE)
        DatePickerAction( KDatePicker *widget, QObject *parent )
#else
        DatePickerAction( QCalendarWidget *widget, QObject *parent )
#endif
            : QWidgetAction( parent ),
              mDatePicker( widget ),
              mOriginalParent( widget->parentWidget() ) {
        }

    protected:
        QWidget *createWidget( QWidget *parent ) {
            mDatePicker->setParent( parent );
            return mDatePicker;
        }

        void deleteWidget( QWidget *widget ) {
            if( widget != mDatePicker ) {
                return;
            }

            mDatePicker->setParent( mOriginalParent );
        }

    private:
#if defined(HAVE_KDE)
        KDatePicker *mDatePicker;
#else
        QCalendarWidget *mDatePicker;
#endif

        QWidget *mOriginalParent;
};


class DatePickerPopup::Private
{
    public:
        Private( DatePickerPopup *qq )
            : q( qq ),
              mDatePicker( 0 ) {
        }

        void buildMenu();

        void slotDateChanged( const QDate &dt );
        void slotToday();
        void slotTomorrow();
        void slotNextWeek();
        void slotNextMonth();
        void slotNoDate();

        DatePickerPopup *q;

#if defined(HAVE_KDE)
        KDatePicker *mDatePicker;
#else
        QCalendarWidget *mDatePicker;
#endif
        Modes mModes;
};


void DatePickerPopup::Private::buildMenu()
{
    if( q->isVisible() ) {
        return;
    }

    q->clear();

    if( mModes & DatePicker ) {
        q->addAction( new DatePickerAction( mDatePicker, q ) );

        if(( mModes & NoDate ) || ( mModes & Words ) ) {
            q->addSeparator();
        }
    }

    if( mModes & Words ) {
        q->addAction( tr( "&Today" ), q, SLOT( slotToday() ) );
        q->addAction( tr( "To&morrow" ), q, SLOT( slotTomorrow() ) );
        q->addAction( tr( "Next &Week" ), q, SLOT( slotNextWeek() ) );
        q->addAction( tr( "Next M&onth" ), q, SLOT( slotNextMonth() ) );

        if( mModes & NoDate ) {
            q->addSeparator();
        }
    }

    if( mModes & NoDate ) {
        q->addAction( tr( "No Date" ), q, SLOT( slotNoDate() ) );
    }
}


void DatePickerPopup::Private::slotDateChanged( const QDate &date )
{
    emit q->dateChanged( date );
    q->hide();
}


void DatePickerPopup::Private::slotToday()
{
    emit q->dateChanged( QDate::currentDate() );
}


void DatePickerPopup::Private::slotTomorrow()
{
    emit q->dateChanged( QDate::currentDate().addDays( 1 ) );
}


void DatePickerPopup::Private::slotNoDate()
{
    emit q->dateChanged( QDate() );
}


void DatePickerPopup::Private::slotNextWeek()
{
    emit q->dateChanged( QDate::currentDate().addDays( 7 ) );
}


void DatePickerPopup::Private::slotNextMonth()
{
    emit q->dateChanged( QDate::currentDate().addMonths( 1 ) );
}



DatePickerPopup::DatePickerPopup( Modes modes, const QDate &date, QWidget *parent )
    : QMenu( parent ),
      d( new Private( this ) )
{
    d->mModes = modes;

#if defined(HAVE_KDE)
    d->mDatePicker = new KDatePicker( this );
    d->mDatePicker->setCloseButton( false );

    connect( d->mDatePicker, SIGNAL( dateEntered( const QDate & ) ),
             this, SLOT( slotDateChanged( const QDate & ) ) );
    connect( d->mDatePicker, SIGNAL( dateSelected( const QDate & ) ),
             this, SLOT( slotDateChanged( const QDate & ) ) );
#else
    d->mDatePicker = new QCalendarWidget( this );

    connect( d->mDatePicker, SIGNAL( activated( const QDate & ) ),
             this, SLOT( slotDateChanged( const QDate & ) ) );
    connect( d->mDatePicker, SIGNAL( clicked( const QDate & ) ),
             this, SLOT( slotDateChanged( const QDate & ) ) );
#endif

    setDate( date );
    d->buildMenu();
}


DatePickerPopup::~DatePickerPopup()
{
    delete d;
}


void DatePickerPopup::setDate( const QDate &date )
{
#if defined(HAVE_KDE)
    d->mDatePicker->setDate( date );
#else
    d->mDatePicker->setSelectedDate( date );
#endif
}


void DatePickerPopup::slotDateChanged( const QDate &date )
{
    d->slotDateChanged( date );
}


void DatePickerPopup::slotToday()
{
    d->slotToday();
}


void DatePickerPopup::slotTomorrow()
{
    d->slotTomorrow();
}


void DatePickerPopup::slotNextWeek()
{
    d->slotNextWeek();
}


void DatePickerPopup::slotNextMonth()
{
    d->slotNextMonth();
}


void DatePickerPopup::slotNoDate()
{
    d->slotNoDate();
}


} // EndNamspace Knipptasch

//#include "datepickerpopup.moc"

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
