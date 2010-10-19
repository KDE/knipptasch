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
#include "recentfilemenu.h"
#include "preferences.h"

#include <QFileInfo>



RecentFileMenu::RecentFileMenu(QWidget * parent)
  : QMenu( parent )
{
    connect(this, SIGNAL( triggered( QAction* ) ), this, SLOT( slotMenuActions( QAction* ) ) );

    updateActions();
}


RecentFileMenu::RecentFileMenu(const QString & title, QWidget * parent)
  : QMenu( title, parent )
{
    connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( slotMenuAction( QAction* ) ) );

    updateActions();
}


QStringList RecentFileMenu::files() const
{
    const int max = Preferences::self()->recentFilesListMaximumCount();
    return m_fileList.mid( 0, max );
}


void RecentFileMenu::addFile(const QString &fileName)
{
    m_fileList.prepend( fileName );

    updateActions();
}


void RecentFileMenu::removeFile(const QString &fileName)
{
    m_fileList.removeAll( fileName );

    updateActions();
}


void RecentFileMenu::slotMenuAction(QAction* action)
{
    if( action->data().isValid() ) {
        emit openFile( action->data().toString() );
    }
}


void RecentFileMenu::clearMenu()
{
    m_fileList.clear();

    updateActions();
}


void RecentFileMenu::updateActions()
{
    QStringList fileList = files();
    Preferences::self()->setRecentFilesList( fileList );

    clear();

    for(int i = 0; i < fileList.size(); ++i) {
        const QString f = fileList.at( i );

        QAction* recentFileAct = addAction( tr( "%2 [%3]" ).arg( QFileInfo( f ).fileName() ).arg( f ) );
        recentFileAct->setData( f );
    }

    addSeparator();
    addAction( tr( "Clear List" ), this, SLOT( clearMenu() ) );

    setEnabled( !fileList.isEmpty() );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

