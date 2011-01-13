/*
* Copyright 2010 by Volker Lanz <vl@fidra.de>
* Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
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
#ifndef UTILS_H
#define UTILS_H

#include <QtGlobal>
#include <QString>
#include <QFileDialog>

class QDateTime;
class QDate;
class QTime;
class QWidget;



QString getOpenFileName(QWidget *parent, const QString &caption = QString(), const QString &dir = QString(), const QString &filterQt = QString(), const QString &filterKDE = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
QString getSaveFileName(QWidget *parent, const QString &caption = QString(), const QString &dir = QString(), const QString &filterQt = QString(), const QString &filterKDE = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);


QStringList findDataDirPaths();
QString findExistingFilePath(const QString& fileName);
QString findFilePath(const QString& fileName);

QString formatDateTime(const QDateTime&);
QString formatShortDate(const QDate& d);
QString formatLongDate(const QDate& d);
QString formatTime(const QTime& t);
QDate readDate(const QString& s, bool* ok = NULL);
QString formatNumber(qint32 i);
QString formatNumber(double num, int precision = -1);

QString formatMoney(double num, int precision = -1);

Qt::DayOfWeek configuredWeekStartDay();
QString decimalSymbol();

QDateTime parseDateTimeByPattern(const QString &str, const QString &pattern);

bool copyFile(QWidget* parent, const QString& src, const QString& dest);


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

