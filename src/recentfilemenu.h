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
#ifndef RECENTFILEMENU_H
#define RECENTFILEMENU_H

#include <QMenu>
#include <QStringList>


class RecentFileMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit RecentFileMenu(QWidget *parent = 0);
        explicit RecentFileMenu(const QString &title, QWidget *parent = 0);

    public slots:
        QStringList files() const;
        void addFile(const QString &fileName);
        void removeFile(const QString &fileName);
        void clearMenu();

    signals:
        void openFile(const QString &fileName);

    private slots:
        void slotMenuAction(QAction *action);
        void updateActions();

    private:
        QStringList m_fileList;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
