/*
 * Copyright 2009-2010 by Stefan Böhmann <kde@hilefoks.org>
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
#include "welcomewidget.h"
#include "ui_welcomewidget.h"

#include "preferences.h"

#include "compat/iconloader.h"

#include <QPainter>
#include <QImage>

#if defined(HAVE_KDE)
#include <KStandardDirs>
#include <KComponentData>
#include <KAboutData>
#endif

#include <QFileInfo>
#include <QListWidget>


WelcomeWidget::WelcomeWidget(QWidget *parent)
  : QFrame( parent ),
    ui( new Ui::WelcomeWidget )
{
    ui->setupUi( this );

#if defined(HAVE_KDE)
    ui->appNameLabel->setText( KGlobal::mainComponent().aboutData()->programName() );
#else
    ui->appNameLabel->setText( QCoreApplication::applicationName() );
#endif

    ui->newButton->setIcon( BarIcon("document-new") );
    ui->openButton->setIcon( BarIcon("document-open") );

    connect( ui->newButton, SIGNAL( clicked() ), this, SIGNAL( createFileClicked() ) );
    connect( ui->openButton, SIGNAL( clicked() ), this, SIGNAL( openFileClicked() ) );

    connect( ui->list, SIGNAL( itemActivated(QListWidgetItem*) ), this, SLOT( slotRecentFileClicked(QListWidgetItem*) ) );
    slotUpdateList();
}


WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}


void WelcomeWidget::showEvent(QShowEvent *event)
{
    slotUpdateList();

    QFrame::showEvent( event );
}


void WelcomeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );

    QPainter painter( this );

#if defined(HAVE_KDE)
    painter.drawImage( geometry(), QImage( KStandardDirs::locate( "appdata", "welcome_background.png" ) ) );
#else
    painter.drawImage( geometry(), QImage( "://welcome_background.png" ) );
#endif
}


void WelcomeWidget::slotUpdateList()
{
    ui->list->clear();

    foreach(const QString &str, Preferences::self()->recentFilesList() ) {
        QFileInfo finfo( str );
        QListWidgetItem *item = new QListWidgetItem( ui->list );
        item->setText( QString( "%1 [%2]" ).arg( finfo.fileName() ).arg( str ) );
        item->setData( Qt::EditRole, str );
    }

}


void WelcomeWidget::slotRecentFileClicked(QListWidgetItem *item)
{
    if( item ) {
        emit openFileClicked( item->data( Qt::EditRole ).toString() );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
