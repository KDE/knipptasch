/*
 * Copyright 2007-2010 by Stefan Böhmann <kde@hilefoks.org>
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

#include "mainwindow.h"

#if defined(HAVE_KDE)
#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KUrl>
#else
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDir>
#include <QUrl>
#include "compat/utils.h"
#endif

#if defined(WITH_QCA2)
#include <QtCrypto>
#endif

#include <QList>
#include <QDebug>



int main(int argc, char* argv[])
{
    QList<QUrl> files;

#if defined(HAVE_KDE)
    KAboutData* aboutData = new KAboutData(
        "knipptasch",
        NULL,
        ki18n( "Knipptasch" ),
        KNIPPTASCH_VERSION,
        ki18n( "KDE Personal Finance Manager" ),
        KAboutData::License_GPL,
        ki18n( "(c) 2010 Stefan Böhmann" )
    );

    aboutData->addAuthor( ki18n( "Stefan Böhmann" ), KLocalizedString(), "kde@hilefoks.org" );
    aboutData->setHomepage( "http://www.hilefoks.org" );

    KCmdLineArgs::init( argc, argv, aboutData );

    KCmdLineOptions options;
    options.add( "+[url]", ki18n( "File to open" ) );
    KCmdLineArgs::addCmdLineOptions( options );

    KApplication app;
    KGlobal::locale()->insertCatalog( "knipptasch_qt" );

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    for(int i = 0; i < args->count(); i++) {
        files.append( args->url( i ) );
    }
    args->clear();

#else
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName( "Hilefoks" );
    QCoreApplication::setOrganizationDomain( "hilefoks.org" );
    QCoreApplication::setApplicationName( "Knipptasch" );
    QCoreApplication::setApplicationVersion( KNIPPTASCH_VERSION );

    QTranslator t1;
    const QString locale = QLocale::system().name();
    const QString qtLocaleName = "qt_" + locale;

    if( t1.load( qtLocaleName, QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ) ) {
        qApp->installTranslator( &t1 );
    }
    else {
        qDebug() << "loading Qt translations for locale "
                 << qtLocaleName
                 << " from dir "
                 << QLibraryInfo::location( QLibraryInfo::TranslationsPath )
                 << " failed.";
    }

    QTranslator t2;
    QString translationDir;
    QStringList dataDirs = findDataDirPaths();
    foreach(const QString& dir, dataDirs) {
        translationDir = dir + QDir::separator() + "translations" + QDir::separator();
        if( QFile::exists( translationDir ) ) {
            break;
        }
    }

    if( t2.load( locale, translationDir ) ) {
        qApp->installTranslator( &t2 );
    }
    else {
        qDebug() << "loading app translations for locale "
                 << locale
                 << " from dir "
                 << translationDir
                 << " failed.";
    }
#endif

#if defined(WITH_QCA2)
    QCA::Initializer init;

    if( !QCA::isSupported( "aes256-cbc-pkcs7" ) ) {
        qDebug() << "AES256 (CBC mode with PKCS#7 padding) not supported!";
    }
#endif

    MainWindow* mainWindow = new MainWindow;
    mainWindow->show();

    QApplication::processEvents();
    mainWindow->openFiles( files );

    return app.exec();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
