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
#ifndef STANDARDACTION_H
#define STANDARDACTION_H

#include <KAction>
#include <QObject>


namespace StandardAction
{
    KAction* open(const QObject* recvr, const char* slot, QObject* parent);
    KAction* openNew(const QObject* recvr, const char* slot, QObject* parent);
    KAction* save(const QObject* recvr, const char* slot, QObject* parent);
    KAction* saveAs(const QObject* recvr, const char* slot, QObject* parent);
    KAction* print(const QObject* recvr, const char* slot, QObject* parent);
    KAction* close(const QObject* recvr, const char* slot, QObject* parent);
    KAction* quit(const QObject* recvr, const char* slot, QObject* parent);
    KAction* selectAll(const QObject* recvr, const char* slot, QObject* parent);
    KAction* preferences(const QObject* recvr, const char* slot, QObject* parent);
    KAction* showStatusbar(const QObject* recvr, const char* slot, QObject* parent);
    KAction* aboutApp(const QObject* recvr, const char* slot, QObject* parent);
    KAction* help(const QObject* recvr, const char* slot, QObject* parent);
    KAction* undo(const QObject* recvr, const char* slot, QObject* parent);
    KAction* redo(const QObject* recvr, const char* slot, QObject* parent);
    KAction* cut(const QObject* recvr, const char* slot, QObject* parent);
    KAction* copy(const QObject* recvr, const char* slot, QObject* parent);
    KAction* paste(const QObject* recvr, const char* slot, QObject* parent);
    KAction* find(const QObject* recvr, const char* slot, QObject* parent);
    KAction* findNext(const QObject* recvr, const char* slot, QObject* parent);
    KAction* findPrev(const QObject* recvr, const char* slot, QObject* parent);
    KAction* showMenubar(const QObject* recvr, const char* slot, QObject* parent);
}

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
