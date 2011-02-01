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
#ifndef KNIPPTASCH_CORE_DATEPICKERPOPUP_H
#define KNIPPTASCH_CORE_DATEPICKERPOPUP_H

#include <QtCore/QDate>
#include <QtGui/QMenu>


/**
 * @short This menu helps the user to select a date quickly.
 *
 * This menu helps the user to select a date quickly. It offers various
 * modes of selecting, e.g. with a DatePicker or with words like "Tomorrow".
 *
 * The available modes are:
 *
 * @li NoDate: A menu-item with "No Date". If chosen, the datepicker will emit
 *     a null QDate.
 * @li DatePicker: Shows a DatePicker-widget.
 * @li Words: Shows items like "Today", "Tomorrow" or "Next Week".
 *
 * @author Bram Schoenmakers <bram_s@softhome.net>
 */
class DatePickerPopup: public QMenu
{
    Q_OBJECT

    public:
        /**
         * Describes the available selection modes.
         */
        enum Mode
        {
            NoDate = 1,     ///< A menu-item with "No Date". Will always return an invalid date.
            DatePicker = 2, ///< A menu-item with a DatePicker.
            Words = 4       ///< A menu-item with list of words that describe a date.
        };

        /**
         * Describes the a set of combined modes.
         */
        Q_DECLARE_FLAGS( Modes, Mode )

        /**
         * Creates a new date picker popup.
         *
         * @param modes The selection modes that shall be offered
         * @param date The initial date of date picker widget.
         * @param parent The parent object.
         */
        explicit DatePickerPopup(Modes modes = DatePicker,
                                 const QDate &date = QDate::currentDate(),
                                 QWidget *parent = 0);

        /**
         * Destroys the date picker popup.
         */
        ~DatePickerPopup();

    public Q_SLOTS:
        /**
         * Sets the current @p date.
         */
        void setDate(const QDate &date);

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the user has selected a new date.
         *
         * @param date The new date.
         */
        void dateChanged(const QDate &date);

    private slots:
        void slotDateChanged(const QDate &dt);
        void slotToday();
        void slotTomorrow();
        void slotNextWeek();
        void slotNextMonth();
        void slotNoDate();

    private:
        //@cond PRIVATE
        class Private;
        Private* const d;
        //@endcond
};


Q_DECLARE_OPERATORS_FOR_FLAGS( DatePickerPopup::Modes )

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
