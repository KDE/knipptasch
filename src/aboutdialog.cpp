/*
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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "compat/iconloader.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTimer>



AboutDialog::AboutDialog(QWidget* parent) 
  : QDialog( parent ),
    ui( new Ui::AboutDialog )
{
    ui->setupUi( this );

    setWindowTitle( tr( "About - %1" ).arg( QCoreApplication::applicationName() ) );
    ui->appNameLabel->setText( QCoreApplication::applicationName() );

    QIcon windowIcon = DesktopIcon( QCoreApplication::applicationName().toLower() );
    if( windowIcon.isNull() ) {
        windowIcon = qApp->windowIcon();
    }
    ui->appIconLabel->setPixmap( windowIcon.pixmap( 64, 64 ) );
    ui->appSloganLabel->setText( tr( "<b>Version: %1</b><br>Using Qt Application Framework %2")
                                .arg( KNIPPTASCH_VERSION ).arg( QT_VERSION_STR ) );

    const QString aboutText =
        QString( "<br/>%1<br/>" ).arg( tr( "Personal Finance Manager" ) ) +
        QString( "<br/>%1<br/>" ).arg( tr( "(c) 2007-2010 Stefan B&#x00F6;hmann" ) ) +
        QString( "<br/>%1<br/>" ).arg( QString("<a href=\"%1\">%1</a>").arg("http://www.hilefoks.org") ) +
        QString( "<br/><a href=\"%2\">%1</a><br/>" )
            .arg( tr( "License: GNU General Public License Version 2") )
            .arg( "http://www.gnu.org/licenses/gpl-2.0.txt" );

    ui->aboutLabel->setText( aboutText );

    
    QString authors =
        QString( "%1 <p style=\"margin:0px; margin-left:15px\"><a href=\"mailto:%2\">%2</a></p>" )
            .arg( tr( "Stefan B&#x00F6;hmann" ) )
            .arg( "kde@hilefoks.org" );

    ui->authorLabel->setText(
        QString( "%1<br/><br/>" )
            .arg( tr("Please use <a href=\"%1\">%2</a> to report bugs.")
            .arg( "http://bugs.kde.org" ).arg( "bugs.kde.org" ) )
        + authors
    );
    
    connect( ui->aboutLabel, SIGNAL( linkActivated(const QString&) ), this, SLOT( slotLinkClicked(const QString&) ) );
    connect( ui->authorLabel, SIGNAL( linkActivated(const QString&) ), this, SLOT( slotLinkClicked(const QString&) ) );

    QTimer::singleShot( 250, this, SLOT( slotLoadData() ) );
}


AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::slotLinkClicked(const QString &url)
{
    QDesktopServices::openUrl( QUrl( url ) );
}


void AboutDialog::slotLoadData()
{
    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

    QFile file( ":/COPYING" );
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString license;
    while( !file.atEnd() ) {
        license.append( file.readLine() );
        license.append( "<br/>" );
    }
    file.close();

    ui->licenseBrowser->setText( license );

    QApplication::restoreOverrideCursor();
}

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
