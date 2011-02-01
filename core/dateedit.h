/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * Based on KDateEdit (part of libkdepim)
 *   Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>
 *   Copyright (c) 2002,2010 David Jarvie <djarvie@kde.org>
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
#ifndef KNIPPTASCH_KDEPIM_KDATEEDIT_H
#define KNIPPTASCH_KDEPIM_KDATEEDIT_H

#include "knipptasch_core_export.h"

#include <QtCore/QDate>


#if defined( HAVE_KDE )
#include <KComboBox>
#else
#include <QComboBox>
#endif


class QEvent;
class QMouseEvent;
class QStatusBar;


/**
  @short A date editing widget that consists of an editable combo box.

  The combo box contains the date in text form, and clicking the combo
  box arrow will display a 'popup' style date picker.

  This widget also supports advanced features like allowing the user
  to type in the day name to get the date. The following keywords
  are supported (in the native language): tomorrow, yesterday, today,
  monday, tuesday, wednesday, thursday, friday, saturday, sunday.
  The user may also use the up and down arrow on the keyboard to
  increment and decrement the date.

  Upper and/or lower limits may be set on the range of dates which the
  user is allowed to enter.

  @image html kdateedit.png "This is how it looks"

  @author Cornelius Schumacher <schumacher@kde.org>
  @author Mike Pilone <mpilone@slac.com>
  @author David Jarvie <djarvie@kde.org>
  @author Tobias Koenig <tokoe@kde.org>
  @author Stefan Böhmann <kde@hilefoks.org>
*/

#if defined( HAVE_KDE )
class KNIPPTASCH_CORE_EXPORT DateEdit : public KComboBox
#else
class KNIPPTASCH_CORE_EXPORT DateEdit : public QComboBox
#endif
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate USER true)

    public:
        /**
         * Creates a new date edit.
         *
         * @param parent The parent widget.
         */
        explicit DateEdit(QWidget *parent = 0);

        /**
         * Creates a new date edit.
         *
         * @param dateFormat own date format string
         * @param parent The parent widget.
         */
        explicit DateEdit(const QString &dateFormat, QWidget *parent = 0);

        /**
         * Destroys the date edit.
         */
        virtual ~DateEdit();

        /**
         * Returns the date entered.
         *
         * @note This date could be invalid, you have to check validity yourself.
         */
        QDate date() const;

        /**
         * Sets the earliest date which can be entered.
         *
         * @param date Earliest date allowed, or invalid to remove the minimum limit.
         * @param errorMsg Error message to be displayed when a date earlier than
         *                 @p date is entered. Set to QString() to use the default
         *                 error message.
         *
         * @see setDateRange()
         */
        void setMinimumDate(const QDate &date, const QString &errorMsg = QString());

        /**
         * Returns the earliest date which can be entered.
         * If there is no minimum date, returns an invalid date.
         */
        QDate minimumDate() const;

        /**
         * Sets the latest date which can be entered.
         *
         * @param date Latest date allowed, or invalid to remove the maximum limit.
         * @param errorMsg Error message to be displayed when a date later than
         *                 @p date is entered. Set to QString() to use the default
         *                 error message.
         *
         * @see setDateRange()
         */
        void setMaximumDate(const QDate& date, const QString& errorMsg = QString());

        /**
         * Returns the latest date which can be entered.
         * If there is no maximum date, returns an invalid date.
         */
        QDate maximumDate() const;

        /**
         * Sets the range of dates which can be entered.
         *
         * @param earliest Earliest date allowed, or invalid to remove the minimum limit.
         * @param latest   Latest date allowed, or invalid to remove the maximum limit.
         * @param earlyErrorMsg Error message to be displayed when a date earlier than
         *                  @p earliest is entered. Set to QString() to use the default
         *                  error message.
         * @param lateErrorMsg Error message to be displayed when a date later than
         *                  @p latest is entered. Set to QString() to use the default
         *                  error message.
         *
         * @see setMinimumDate(), setMaximumDate()
         */
        void setDateRange(const QDate &earliest, const QDate &latest,
                            const QString &earlyErrorMsg = QString(),
                            const QString &lateErrorMsg = QString());

        /**
         * Sets whether the widget is read-only for the user. If read-only, the
         * date pop-up is inactive, and the displayed date cannot be edited.
         *
         * @param readOnly True to set the widget read-only, false to set it read-write.
         */
        void setReadOnly(bool readOnly);

        /**
         * Returns whether the widget is read-only.
         */
        bool isReadOnly() const;

        /**
         * Shows the date picker popup menu.
         */
        virtual void showPopup();

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the user has entered a new date.
         * When the user changes the date by editing the line edit field,
         * the signal is not emitted until focus leaves the line edit field.
         * The passed date can be invalid.
         */
        void dateEntered(const QDate &date);

        /**
         * This signal is emitted whenever date is changed.
         * Unlike dateEdited(), this signal is also emitted when the date is changed
         * programmatically.
         *
         * The passed date can be invalid.
         */
        void dateChanged(const QDate &date);

        /**
         * This signal is emitted whenever the date is edited by the user.
         * Unlike dateChanged(), this signal is not emitted when the date is changed
         * programmatically.
         *
         * The passed date can be invalid.
         */
        void dateEdited(const QDate &date);

    public Q_SLOTS:
        /**
         * Sets the date.
         *
         * @param date The new date to display. This date must be valid or
         *             it will not be set
         */
        void setDate(const QDate &date);

    protected:
        virtual bool eventFilter(QObject *obj, QEvent *ev);
        virtual void mousePressEvent(QMouseEvent *ev);
        virtual void focusOutEvent(QFocusEvent *ev);
        virtual void keyPressEvent(QKeyEvent *ev);

        /**
         * Sets the date, without altering the display.
         * This method is used internally to set the widget's date value.
         * As a virtual method, it allows derived classes to perform additional
         * validation on the date value before it is set. Derived classes should
         * return true if QDate::isValid(@p date) returns false.
         *
         * @param date The new date to set.
         * @return True if the date was set, false if it was considered invalid
         *         and remains unchanged.
         */
        virtual bool assignDate(const QDate &date);

    private slots:
        void lineEnterPressed();
        void slotTextChanged(const QString &str);
        void dateSelected(const QDate &dt);

    private:
        //@cond PRIVATE
        class Private;
        Private* const d;
        //@endcond
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
