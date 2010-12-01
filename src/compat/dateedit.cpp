/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * Based on KDateEdit (part of libkdepim)
 *   Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>
 *   Copyright (c) 2002,2010 David Jarvie <djarvie@kde.org>
 *   Copyright (c) 2003-2004 Reinhold Kainhofer <reinhold@kainhofer.com>
 *   Copyright (c) 2004 Tobias Koenig <tokoe@kde.org>
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
#include "dateedit.h"

#include "datepickerpopup.h"

#include "utils.h"

#if defined(HAVE_KDE)
#include <KMessageBox>
#include <KCalendarSystem>
#include <KGlobal>
#include <KGlobalSettings>
#include <KLocale>
#include <KLocalizedString>
#else
#include <QMessageBox>
#include <QDesktopWidget>
#endif

#include <QtCore/QEvent>
#include <QtCore/QDate>
#include <QtGui/QAbstractItemView>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QMouseEvent>
#include <QtGui/QValidator>


class DateValidator : public QValidator
{
    public:
        DateValidator(const QStringList &keywords, QWidget *parent)
          : QValidator( parent ),
            mKeywords( keywords )
        {
        }

        virtual State validate(QString &str, int &value) const
        {
            Q_UNUSED( value );

            int length = str.length();

            // empty string is intermediate so one can clear the edit line and start from scratch
            if( length <= 0 ) {
                return Intermediate;
            }

            if( mKeywords.contains( str.toLower() ) ) {
                return Acceptable;
            }

            bool ok = false;
            readDate( str, &ok );

            return ok ? Acceptable : Intermediate;
        }

    private:
        QStringList mKeywords;
};


class DateEdit::Private
{
    public:
        Private(DateEdit *qq)
          : q( qq ),
            mReadOnly( false ),
            mDiscardNextMousePress( false )
        {
        }

        QDate parseDate(bool *replaced = 0) const;
        void updateView();
        void setupKeywords();
        bool newDateEntered(const QDate &newDate);

    public:
        // slots
        void lineEnterPressed();
        void slotTextChanged(const QString &str);
        void dateSelected(const QDate &dt);

        DateEdit *q;
        DatePickerPopup *mPopup;

        QDate mDate;
        QDate mMinDate;               // minimum allowed date, or invalid for no minimum
        QDate mMaxDate;               // maximum allowed date, or invalid for no maximum
        QString mMinDateErrString;    // error message when entered date < mMinDate
        QString mMaxDateErrString;    // error message when entered date > mMaxDate
        bool mReadOnly;
        bool mTextChanged;
        bool mDiscardNextMousePress;

        QMap<QString, int> mKeywordMap;
};


QDate DateEdit::Private::parseDate(bool *replaced) const
{
    const QString text = q->currentText();

    if( replaced ) {
        (*replaced) = false;
    }

    QDate result;
    if( text.isEmpty() ) {
        result = QDate();
    }
    else if( mKeywordMap.contains( text.toLower() ) ) {
        const QDate today = QDate::currentDate();
        int i = mKeywordMap[ text.toLower() ];

        if ( i >= 100 ) {
            /* A day name has been entered. Convert to offset from today.
             * This uses some math tricks to figure out the offset in days
             * to the next date the given day of the week occurs. There
             * are two cases, that the new day is >= the current day, which means
             * the new day has not occurred yet or that the new day < the current day,
             * which means the new day is already passed (so we need to find the
             * day in the next week).
             */
            i -= 100;

            const int currentDay = today.dayOfWeek();
            if( i >= currentDay ) {
                i -= currentDay;
            }
            else {
                i += 7 - currentDay;
            }
        }

        result = today.addDays( i );
        if( replaced ) {
            (*replaced) = true;
        }
    }
    else {
        result = readDate( text );
    }

    return result;
}


void DateEdit::Private::updateView()
{
    QString dateString;
    if( mDate.isValid() ) {
        dateString = formatShortDate( mDate );
    }

    // We do not want to generate a signal here,
    // since we explicitly setting the date
    const bool blocked = q->signalsBlocked();
    q->blockSignals( true );
    q->removeItem( 0 );
    q->insertItem( 0, dateString );
    q->blockSignals( blocked );
}


void DateEdit::Private::setupKeywords()
{
    // Create the keyword list. This will be used to match against when the user
    // enters information.
    mKeywordMap.insert( tr( "tomorrow" ), 1 );
    mKeywordMap.insert( tr( "today" ), 0 );
    mKeywordMap.insert( tr( "yesterday" ), -1 );

    QString dayName;
    for ( int i = 1; i <= 7; ++i ) {
#if defined(HAVE_KDE)
        dayName = KGlobal::locale()->calendar()->weekDayName( i ).toLower();
#else
        dayName = QDate::longDayName( i ).toLower();
#endif
        mKeywordMap.insert( dayName, i + 100 );
    }
}


// Check a new date against any minimum or maximum date.
bool DateEdit::Private::newDateEntered(const QDate &newDate)
{
    QDate date = newDate;
    if( newDate.isValid() ) {
        QString errString;
        QString message;
        if( mMinDate.isValid()  &&  newDate < mMinDate ) {
            message = tr( "Date cannot be earlier than %1" )
                            .arg( mMinDate == QDate::currentDate()
                                    ? tr( "today" )
                                    : formatShortDate( mMinDate ) );

            errString = mMinDateErrString;
            date = mMinDate;
        }
        else if( mMaxDate.isValid()  &&  newDate > mMaxDate ) {
            message = tr( "Date cannot be later than %1" )
                            .arg( mMinDate == QDate::currentDate()
                                    ? tr( "today" )
                                    : formatShortDate( mMinDate ) );

            errString = mMaxDateErrString;
            date = mMaxDate;
        }

        if( !message.isEmpty() ) {
            q->assignDate( date );
            updateView();

#if defined(HAVE_KDE)
            KMessageBox::sorry( q, errString );
#else
            QMessageBox::information( q, tr( "Sorry" ), errString );
#endif
        }
    }

    emit q->dateChanged( date );
    emit q->dateEdited( date );
    emit q->dateEntered( date );

    return date.isValid();
}


void DateEdit::Private::lineEnterPressed()
{
    bool replaced = false;
    const QDate date = parseDate( &replaced );

    if( q->assignDate( date ) ) {
        if( replaced ) {
            updateView();
        }

        newDateEntered( date );
    }
}


void DateEdit::Private::slotTextChanged(const QString &str)
{
    Q_UNUSED( str );
    const QDate date = parseDate();

    if( q->assignDate( date ) ) {
        emit q->dateEdited( date );
        emit q->dateChanged( date );
    }

    mTextChanged = true;
}


void DateEdit::Private::dateSelected(const QDate &date)
{
    if( q->assignDate( date ) ) {
        updateView();
        if( newDateEntered( date ) ) {
            mPopup->hide();
        }
    }
}


DateEdit::DateEdit(QWidget *parent)
  : QComboBox( parent ),
    d( new Private( this ) )
{
    // need at least one entry for popup to work
    setMaxCount( 1 );
    setEditable( true );

    const QString today = formatShortDate( d->mDate );

    addItem( today );
    setCurrentIndex( 0 );

    connect( lineEdit(), SIGNAL( returnPressed() ),
            this, SLOT( lineEnterPressed() ) );
    connect( this, SIGNAL( editTextChanged( const QString& ) ),
             this, SLOT( slotTextChanged( const QString& ) ) );

    d->mPopup = new DatePickerPopup(
                            DatePickerPopup::NoDate
                          | DatePickerPopup::DatePicker
                          | DatePickerPopup::Words,
                        QDate::currentDate(), this );

    d->mPopup->hide();
    d->mPopup->installEventFilter( this );

    connect( d->mPopup, SIGNAL( dateChanged(const QDate&) ), this, SLOT( dateSelected(const QDate&) ) );

    // handle keyword entry
    d->setupKeywords();
    lineEdit()->installEventFilter( this );

    setValidator( new DateValidator( d->mKeywordMap.keys(), this ) );

    d->mTextChanged = false;
}


DateEdit::~DateEdit()
{
    delete d;
}


void DateEdit::setDate(const QDate &date)
{
    assignDate( date );
    d->updateView();

    if( date.isValid() ) {
        emit dateChanged( date );
    }
}


QDate DateEdit::date() const
{
    return d->mDate;
}


void DateEdit::setMinimumDate(const QDate &minDate, const QString &errorMsg)
{
    d->mMinDate = minDate;

    if( d->mMinDate.isValid() && date().isValid() && date() < d->mMinDate ) {
        setDate( d->mMinDate );
    }

    d->mMinDateErrString = errorMsg;
}


QDate DateEdit::minimumDate() const
{
    return d->mMinDate;
}


void DateEdit::setMaximumDate(const QDate& maxDate, const QString& errorMsg)
{
    d->mMaxDate = maxDate;

    if( d->mMaxDate.isValid() && date().isValid() && date() > d->mMaxDate ) {
        setDate( d->mMaxDate );
    }

    d->mMaxDateErrString = errorMsg;
}


QDate DateEdit::maximumDate() const
{
    return d->mMaxDate;
}


void DateEdit::setDateRange(const QDate& earliest, const QDate& latest,
                            const QString& earlyErrorMsg, const QString& lateErrorMsg)
{
    setMinimumDate( earliest, earlyErrorMsg );
    setMaximumDate( latest, lateErrorMsg );
}


void DateEdit::setReadOnly(bool readOnly)
{
    d->mReadOnly = readOnly;
    lineEdit()->setReadOnly( readOnly );
}


bool DateEdit::isReadOnly() const
{
    return d->mReadOnly;
}


void DateEdit::showPopup()
{
    if( d->mReadOnly ) {
        return;
    }

    const QRect desk =
#if defined(HAVE_KDE)
            KGlobalSettings::desktopGeometry( this );
#else
            QApplication::desktop()->screenGeometry( this );
#endif

    QPoint popupPoint = mapToGlobal( QPoint( 0, 0 ) );

    const int dateFrameHeight = d->mPopup->sizeHint().height();

    if( popupPoint.y() + height() + dateFrameHeight > desk.bottom() ) {
        popupPoint.setY( popupPoint.y() - dateFrameHeight );
    }
    else {
        popupPoint.setY( popupPoint.y() + height() );
    }

    const int dateFrameWidth = d->mPopup->sizeHint().width();
    if( popupPoint.x() + dateFrameWidth > desk.right() ) {
        popupPoint.setX( desk.right() - dateFrameWidth );
    }

    if( popupPoint.x() < desk.left() ) {
        popupPoint.setX( desk.left() );
    }

    if( popupPoint.y() < desk.top() ) {
        popupPoint.setY( desk.top() );
    }

    if( d->mDate.isValid() ) {
        d->mPopup->setDate( d->mDate );
    }
    else {
        d->mPopup->setDate( QDate::currentDate() );
    }

    d->mPopup->popup( popupPoint );

    // The combo box is now shown pressed. Make it show not pressed again
    // by causing its (invisible) list box to emit a 'selected' signal.
    // First, ensure that the list box contains the date currently displayed.
    const QDate date = d->parseDate();
    assignDate( date );
    d->updateView();

    // Now, simulate an Enter to unpress it
    QAbstractItemView *lb = view();
    if( lb ) {
        lb->setCurrentIndex( lb->model()->index( 0, 0 ) );
        QKeyEvent *keyEvent = new QKeyEvent( QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier );
        QApplication::postEvent( lb, keyEvent );
    }
}


void DateEdit::focusOutEvent(QFocusEvent *event)
{
    if( d->mTextChanged ) {
        d->lineEnterPressed();
        d->mTextChanged = false;
    }

    QComboBox::focusOutEvent( event );
}


void DateEdit::keyPressEvent(QKeyEvent *event)
{
    int step = 0;

    if( event->key() == Qt::Key_Up ) {
        step = 1;
    }
    else if( event->key() == Qt::Key_Down ) {
        step = -1;
    }

    if( step && !d->mReadOnly ) {
        QDate date = d->parseDate();
        if( date.isValid() ) {
            date = date.addDays( step );
            if( assignDate( date ) ) {
                d->updateView();
                d->newDateEntered( date );
            }
        }
    }

    QComboBox::keyPressEvent( event );
}


bool DateEdit::eventFilter(QObject *object, QEvent *event)
{
    if( object == lineEdit() ) {
        // We only process the focus out event if the text has changed
        // since we got focus
        if( (event->type() == QEvent::FocusOut) && d->mTextChanged ) {
            d->lineEnterPressed();
            d->mTextChanged = false;
        }
        else if ( event->type() == QEvent::KeyPress ) {
            // Up and down arrow keys step the date
            QKeyEvent *keyEvent = (QKeyEvent *)event;

            if( keyEvent->key() == Qt::Key_Return ) {
                d->lineEnterPressed();
                return true;
            }
        }
    }
    else {
        // It's a date picker event
        switch( event->type() ) {
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseButtonPress:
            {
                QMouseEvent *mouseEvent = (QMouseEvent*)event;
                if( !d->mPopup->rect().contains( mouseEvent->pos() ) ) {
                    const QPoint globalPos = d->mPopup->mapToGlobal( mouseEvent->pos() );

                    if( QApplication::widgetAt( globalPos ) == this ) {
                        // The date picker is being closed by a click on the
                        // DateEdit widget. Avoid popping it up again immediately.
                        d->mDiscardNextMousePress = true;
                    }
                }
            }
            default:
                break;
        }
    }

    return false;
}


void DateEdit::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton && d->mDiscardNextMousePress ) {
        d->mDiscardNextMousePress = false;
        return;
    }

    QComboBox::mousePressEvent( event );
}


bool DateEdit::assignDate(const QDate &date)
{
    d->mDate = date;
    d->mTextChanged = false;

    return true;
}


void DateEdit::lineEnterPressed()
{
    d->lineEnterPressed();
}


void DateEdit::slotTextChanged(const QString &str)
{
    d->slotTextChanged( str );
}


void DateEdit::dateSelected(const QDate &dt)
{
    d->dateSelected( dt );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
