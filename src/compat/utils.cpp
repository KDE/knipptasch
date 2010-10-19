/*
 * Copyright 2010 by Volker Lanz <vl@fidra.de>
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * parseDateTimeByPattern() is
 *   Copyright 2009 by Tobias Koenig <tokoe@kde.org>
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
#include "utils.h"

#include <QLocale>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

#if defined(HAVE_KDE)
#include <KGlobal>
#include <KLocale>
#include <KStandardDirs>
#include <KIO/NetAccess>
#include <KIO/CopyJob>
#include <KIO/FileCopyJob>
#include <KIO/JobUiDelegate>
#endif

#include <QDebug>



bool copyFile(QWidget* parent, const QString& src, const QString& dest)
{
#if defined(HAVE_KDE)
    KIO::CopyJob* job = KIO::copy( src, dest, KIO::HideProgressInfo );
    job->setDefaultPermissions( true );

    KIO::NetAccess::synchronousRun( job, parent );

    if( job->error() ) {
        job->ui()->showErrorMessage();
        return false;
    }
#else
    if( QFile::exists( dest ) ) {
        QFile::remove( dest );
    }

    if ( !QFile::copy( src, dest ) ) {
        QMessageBox::information( // krazy:exclude=qclasses
            parent,
            QObject::tr("Could not copy file '%1' to '%2'.").arg( src ).arg( dest ),
            QObject::tr( "Error Copying File" )
        );

        return false;
    }
#endif

    return true;
}


#if !defined(HAVE_KDE)
QString defaultDataWriteDir()
{
    QString rval;

#if defined(Q_OS_WIN32)
    rval = QString(qgetenv("APPDATA")) + QDir::separator() + QCoreApplication::organizationName() + QDir::separator() + QCoreApplication::applicationName() + QDir::separator();
#elif defined(Q_OS_MAC)
    rval = QDir::homePath() + "/Library/Application Support/" + QCoreApplication::applicationName() + QDir::separator();
#else // UNIX
    QStringList dataDirPaths = QString(qgetenv("XDG_DATA_HOME")).split(':', QString::SkipEmptyParts);

    if (dataDirPaths.isEmpty())
        dataDirPaths << QDir::homePath() + QDir::separator() + ".local/share/apps/";

    rval = dataDirPaths[0] + QDir::separator() + QCoreApplication::applicationName().toLower() + QDir::separator();
#endif

    return QDir::cleanPath(rval);
}
#endif


QString findFilePath(const QString& fileName)
{
#if defined(HAVE_KDE)
    const QString dir = KStandardDirs::locateLocal("data", QString(), true);
    return dir + QDir::separator() + QCoreApplication::applicationName().toLower() + QDir::separator() + fileName;
#else
    QStringList dataDirs = findDataDirPaths();

    foreach (const QString& dir, dataDirs)
        if (QFile::exists(dir + QDir::separator() + fileName))
            return dir + QDir::separator() + fileName;

    return defaultDataWriteDir() + QDir::separator() + fileName;
#endif
}


QString findExistingFilePath(const QString& fileName)
{
    QStringList dirs = findDataDirPaths();
    foreach(const QString& s, dirs) {
        if (QFile::exists(s + QDir::separator() + fileName)) {
            return s + QDir::separator() + fileName;
        }
    }
    return QString();
}


QStringList findDataDirPaths()
{
#if defined(HAVE_KDE)
    QStringList rval = KGlobal::dirs()->resourceDirs( "data" );

    for(int i = 0; i < rval.size(); i++) {
        rval[ i ].append( QDir::separator() + QCoreApplication::applicationName().toLower() + QDir::separator() );
    }

    return rval;

#else

    QStringList dataDirPaths = QString(qgetenv("XDG_DATA_HOME")).split(':', QString::SkipEmptyParts);

    if (dataDirPaths.isEmpty())
        dataDirPaths << QDir::homePath() + QDir::separator() + ".local/share/apps/";

    dataDirPaths << QString(qgetenv("XDG_DATA_DIRS")).split(':', QString::SkipEmptyParts);

#if defined(Q_OS_WIN32)
    dataDirPaths
        << QString(qgetenv("APPDATA")) + QDir::separator() + QCoreApplication::organizationName() + QDir::separator() + QCoreApplication::applicationName()
        << QCoreApplication::applicationDirPath();
#elif defined(Q_OS_MAC)
    dataDirPaths
        << QDir::homePath() + "/Library/Application Support/" + QCoreApplication::applicationName() + QDir::separator()
        << QCoreApplication::applicationDirPath();
#else // UNIX
    dataDirPaths << QString("/usr/share/apps/") + QCoreApplication::applicationName().toLower() + QDir::separator();
#endif

    QString appDir = QCoreApplication::applicationDirPath();
    int binpos = appDir.lastIndexOf("/bin");

    if (binpos >= 0) {
        appDir.replace(binpos, 4, "/share/apps/");
        appDir.append(QCoreApplication::applicationName().toLower() + QDir::separator());
        dataDirPaths << appDir;
    }

    // add resource path
    dataDirPaths << ":/data/";

    // clean up
    QStringList::Iterator i = dataDirPaths.begin();

    while (i != dataDirPaths.end()) {
        *i = QDir::cleanPath(*i);

        if (!i->endsWith(QDir::separator()) && !i->endsWith('/'))
            i->append(QDir::separator());

        if( !QFile::exists( *i ) ) {
            i = dataDirPaths.erase(i);
        }
        else
            i++;
    }

    dataDirPaths.removeDuplicates();

    return dataDirPaths;
#endif
}


Qt::DayOfWeek configuredWeekStartDay()
{
#if defined(HAVE_KDE)
    return static_cast<Qt::DayOfWeek>( KGlobal::locale()->weekStartDay() );
#else
    return static_cast<Qt::DayOfWeek>(1);
#endif
}


QString formatNumber(qint32 i)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatNumber(i);
#else
    return QLocale().toString(i);
#endif
}


QString formatNumber(double num, int precision)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatNumber(num, precision);
#else
    return QLocale().toString(num, 'f', precision >= 0 ? precision : 2 );
#endif
}


QString formatMoney(double num, int precision)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatMoney( num, QString(), precision );
#else
    return QObject::tr( "%1 $" ).arg( formatNumber( num, precision >= 0 ? precision : 2 ) );
#endif
}


QString decimalSymbol()
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->decimalSymbol();
#else
    return QLocale().decimalPoint();
#endif
}


QDateTime parseDateTimeByPattern(const QString &dateStr, const QString &pattern)
{
    int year, month, day, hour, minute, second;
    year = month = day = hour = minute = second = 0;

    int currPos = 0;
    for(int i = 0; i < pattern.length(); ++i) {
        if( pattern[ i ] == 'y' ) { // 19YY
            if( currPos + 1 < dateStr.length() ) {
                year = 1900 + dateStr.mid( currPos, 2 ).toInt();
                currPos += 2;
            }
            else {
                return QDateTime();
            }
        }
        else if( pattern[ i ] == 'Y' ) { // YYYY
            if ( currPos + 3 < dateStr.length() ) {
                year = dateStr.mid( currPos, 4 ).toInt();
                currPos += 4;
            }
            else {
                return QDateTime();
            }
        }
        else if( pattern[ i ] == 'm' ) { // M or MM
            if( currPos + 1 < dateStr.length() ) {
                if( dateStr[ currPos ].isDigit() ) {
                    if( dateStr[ currPos + 1 ].isDigit() ) {
                        month = dateStr.mid( currPos, 2 ).toInt();
                        currPos += 2;
                        continue;
                    }
                }
            }

            if( currPos < dateStr.length() ) {
                if( dateStr[ currPos ].isDigit() ) {
                month = dateStr.mid( currPos, 1 ).toInt();
                currPos++;
                continue;
                }
            }

            return QDateTime();
        }
        else if( pattern[ i ] == 'M' ) { // 0M or MM
            if( currPos + 1 < dateStr.length() ) {
                month = dateStr.mid( currPos, 2 ).toInt();
                currPos += 2;
            }
            else {
                return QDateTime();
            }
        }
        else if( pattern[ i ] == 'd' ) { // D or DD
            if( currPos + 1 < dateStr.length() ) {
                if( dateStr[ currPos ].isDigit() ) {
                    if( dateStr[ currPos + 1 ].isDigit() ) {
                        day = dateStr.mid( currPos, 2 ).toInt();
                        currPos += 2;
                        continue;
                    }
                }
            }

            if( currPos < dateStr.length() ) {
                if( dateStr[ currPos ].isDigit() ) {
                    day = dateStr.mid( currPos, 1 ).toInt();
                    currPos++;
                    continue;
                }
            }

            return QDateTime();
        }
        else if( pattern[ i ] == 'D' ) { // 0D or DD
            if( currPos + 1 < dateStr.length() ) {
                day = dateStr.mid( currPos, 2 ).toInt();
                currPos += 2;
            }
            else {
                return QDateTime();
            }
        }
        else if( pattern[ i ] == 'H' ) { // 0H or HH
            if( currPos + 1 < dateStr.length() ) {
                hour = dateStr.mid( currPos, 2 ).toInt();
                currPos += 2;
            }
            else {
                return QDateTime();
            }
        }
        else if( pattern[ i ] == 'I' ) { // 0I or II
            if( currPos + 1 < dateStr.length() ) {
                minute = dateStr.mid( currPos, 2 ).toInt();
                currPos += 2;
            }
            else {
                return QDateTime();
            }
        }
        else if( pattern[ i ] == 'S' ) { // 0S or SS
            if( currPos + 1 < dateStr.length() ) {
                second = dateStr.mid( currPos, 2 ).toInt();
                currPos += 2;
            }
            else {
                return QDateTime();
            }
        }
        else {
            currPos++;
        }
    }

    return QDateTime( QDate( year, month, day ), QTime( hour, minute, second ) );
}


QString formatDateTime(const QDateTime& d)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatDateTime(d);
#else
    return QLocale().toString(d, QLocale::ShortFormat);
#endif
}


QString formatShortDate(const QDate& d)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatDate(d, KLocale::ShortDate);
#else
    return QLocale().toString(d, QLocale::ShortFormat);
#endif
}


QString formatLongDate(const QDate& d)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatDate(d);
#else
    return QLocale().toString(d, QLocale::LongFormat);
#endif
}


QString formatTime(const QTime& t)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->formatTime(t);
#endif
    return QLocale().toString(t);
}


QDate readDate(const QString& s, bool* ok)
{
#if defined(HAVE_KDE)
    return KGlobal::locale()->readDate(s, ok);
#else
    QDate rval;
    QList<Qt::DateFormat> formats;
    formats << Qt::TextDate << Qt::ISODate << Qt::SystemLocaleDate << Qt::SystemLocaleShortDate
            << Qt:: SystemLocaleLongDate << Qt::DefaultLocaleShortDate << Qt::DefaultLocaleLongDate;

    foreach (Qt::DateFormat f, formats) { // krazy:exclude=foreach
        rval = QDate::fromString(s, f);

        if (rval.isValid())
            break;
    }

    if( ok != NULL ) {
        *ok = rval.isValid();
    }

    return rval;
#endif
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
