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
#include "standardaction.h"

#include <QObject>


#if defined(HAVE_KDE)

#include <kstandardaction.h>
#include <ktoggleaction.h>


namespace StandardAction
{

    KAction* open(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::open(recvr, slot, parent);
    }

    KAction* openNew(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::openNew(recvr, slot, parent);
    }

    KAction* save(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::save(recvr, slot, parent);
    }

    KAction* saveAs(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::saveAs(recvr, slot, parent);
    }

    KAction* print(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::print(recvr, slot, parent);
    }

    KAction* close(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::close(recvr, slot, parent);
    }

    KAction* quit(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::quit(recvr, slot, parent);
    }

    KAction* selectAll(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::selectAll(recvr, slot, parent);
    }

    KAction* preferences(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::preferences(recvr, slot, parent);
    }

    KAction* showStatusbar(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::showStatusbar(recvr, slot, parent);
    }

    KAction* aboutApp(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::aboutApp(recvr, slot, parent);
    }

    KAction* help(const QObject* recvr, const char* slot, QObject* parent)
    {
        return KStandardAction::help(recvr, slot, parent);
    }

    KAction* undo(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::undo(recvr, slot, parent);
    }

    KAction* redo(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::redo(recvr, slot, parent);
    }

    KAction* cut(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::cut(recvr, slot, parent);
    }

    KAction* copy(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::copy(recvr, slot, parent);
    }

    KAction* paste(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::paste(recvr, slot, parent);
    }

    KAction* find(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::find(recvr, slot, parent);
    }

    KAction* findNext(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::findNext(recvr, slot, parent);
    }

    KAction* findPrev(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::findPrev(recvr, slot, parent);
    }

    KAction* showMenubar(const QObject* recvr, const char* slot, QObject* parent)
    {
    return KStandardAction::showMenubar(recvr, slot, parent);
    }
}


#else //HAVE_KDE

#include "actioncollection.h"
#include "iconloader.h"

#include <kicon.h>
#include <kaction.h>

#include <QKeySequence>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>


enum StandardActionId
{
    idOpen,
    idOpenNew,
    idSave,
    idSaveAs,
    idClose,
    idQuit,
    idSelectAll,
    idPreferences,
    idShowStatusbar,
    idAboutApp,
    idHelp,
    idPrint,
    idUndo,
    idRedo,
    idCut,
    idCopy,
    idPaste,
    idFind,
    idFindNext,
    idFindPrev,
    idShowMenubar
};

struct StandardActionInfo
{
    StandardActionId id;
    QKeySequence accel;
    const char* name;
    QString label;
    QString tooltip;
    KIcon icon;
    bool checkable;
};

static KAction* makeAction(StandardActionId id, const QObject* recv, const char* slot, QObject* parent)
{
    static const StandardActionInfo actionInfo[] =
    {
        { idOpen, QKeySequence::Open, "file_open", QObject::tr("&Open..."), QObject::tr("Open file"), BarIcon("document-open"), false },
        { idOpenNew, QKeySequence::New, "file_new", QObject::tr("&New"), QObject::tr("New file"), BarIcon("document-new"), false },
        { idSave, QKeySequence::Save, "file_save", QObject::tr("&Save"), QObject::tr("Save file"), BarIcon("document-save"), false },
        { idSaveAs, QKeySequence::SaveAs, "file_save_as", QObject::tr("Save &As..."), QObject::tr("Save file"), BarIcon("document-save-as"), false },
        { idPrint, QKeySequence::Print, "file_print", QObject::tr("&Print"), QObject::tr("&Print..."), BarIcon("document-print"), false },
        { idClose, QKeySequence::Close, "file_close", QObject::tr("&Close"), QObject::tr("Close file"), BarIcon("window-close"), false },
        { idQuit, QKeySequence::Quit, "file_quit", QObject::tr("&Quit"), QObject::tr("Quit the application"), BarIcon("application-exit"), false },
        { idSelectAll, QKeySequence::SelectAll, "edit_select_all", QObject::tr("Select &All"), QObject::tr("Select all entries"), BarIcon("edit-select-all"), false },
        { idPreferences, QKeySequence::Preferences, "options_configure", QObject::tr("&Configure Knipptasch ..."), QObject::tr("Modify the application's settings"), BarIcon("configure"), false },
        { idShowStatusbar, QKeySequence(), "options_show_statusbar", QObject::tr("Show St&atusbar"), QObject::tr("Show the status bar"), KIcon(), true },
        { idAboutApp, QKeySequence(), "help_about_app", QObject::tr("&About %1").arg( QCoreApplication::applicationName() ), QString(), BarIcon("help-about"), false },
        { idHelp, QKeySequence::HelpContents, "help", QObject::tr("%1 &Handbook").arg( QCoreApplication::applicationName() ), QString(), BarIcon("help-contents"), false },
        { idUndo, QKeySequence::Undo, "edit_undo", QObject::tr("&Undo"), QObject::tr("Undo operation"), BarIcon("edit-undo"), false },
        { idRedo, QKeySequence::Redo, "edit_redo", QObject::tr("Re&do"), QObject::tr("Redo operation"), BarIcon("edit-redo"), false },
        { idCut, QKeySequence::Cut, "edit_cut", QObject::tr("Cu&t"), QObject::tr("Cut selection"), BarIcon("edit-cut"), false },
        { idCopy, QKeySequence::Copy, "edit_copy", QObject::tr("&Copy"), QObject::tr("Copy selection to clipboard"), BarIcon("edit-copy"), false },
        { idPaste, QKeySequence::Paste, "edit_paste", QObject::tr("&Paste"), QObject::tr("Paste clipboard contents"), BarIcon("edit-paste"), false },
        { idFind, QKeySequence::Find, "edit_find", QObject::tr("&Find..."), QObject::tr("Search entries"), BarIcon("edit-find"), false },
        { idFindNext, QKeySequence::FindNext, "edit_find_next", QObject::tr("Find &Next"), QObject::tr("Find next match"), BarIcon("go-down-search"), false },
        { idFindPrev, QKeySequence::FindPrevious, "edit_find_prev", QObject::tr("Find Pre&vious"), QObject::tr("Find previous match"), BarIcon("go-up-search"), false },
        { idShowMenubar, QKeySequence(Qt::CTRL + Qt::Key_M), "options_show_menubar", QObject::tr("Show &Menubar"), QObject::tr("Toggle the menu bar visibility"), BarIcon("show-menu"), true },
    };

    KAction* rval = NULL;
    const StandardActionInfo* info = NULL;

    for (quint32 i = 0; i < sizeof(actionInfo) / sizeof(actionInfo[0]); i++)
        if (actionInfo[i].id == id)
        {
            info = &actionInfo[i];
            break;
        }

    if (info != NULL)
    {
        rval = new KAction(info->icon, info->label, parent);
        rval->setObjectName(info->name);
        rval->setToolTip(info->tooltip);
        rval->setStatusTip(info->tooltip);
        rval->setCheckable(info->checkable);

        if (!info->accel.isEmpty())
            rval->setShortcut(info->accel);

        if (recv != NULL && slot != NULL)
            QObject::connect(rval, SIGNAL(triggered()), recv, slot);

        ActionCollection* collection = qobject_cast<ActionCollection *>(parent);
        if (rval && collection)
            collection->addAction(rval->objectName(), rval);
    }
    else
        qWarning() << "could not find standard action with id " << id;

    return rval;
}


namespace StandardAction
{

    KAction* open(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idOpen, recvr, slot, parent);
    }

    KAction* openNew(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idOpenNew, recvr, slot, parent);
    }

    KAction* save(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idSave, recvr, slot, parent);
    }

    KAction* saveAs(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idSaveAs, recvr, slot, parent);
    }

    KAction* print(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idPrint, recvr, slot, parent);
    }

    KAction* close(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idClose, recvr, slot, parent);
    }

    KAction* quit(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idQuit, recvr, slot, parent);
    }

    KAction* selectAll(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idSelectAll, recvr, slot, parent);
    }

    KAction* preferences(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idPreferences, recvr, slot, parent);
    }

    KAction* showStatusbar(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idShowStatusbar, recvr, slot, parent);
    }

    KAction* aboutApp(const QObject* recvr, const char* slot, QObject* parent)
    {
        KAction* rval = makeAction(idAboutApp, recvr, slot, parent);
        rval->setIcon(QApplication::windowIcon());
        return rval;
    }

    KAction* help(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idHelp,recvr, slot, parent);
    }

    KAction* undo(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idUndo,recvr, slot, parent);
    }

    KAction* redo(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idRedo, recvr, slot, parent);
    }

    KAction* cut(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idCut, recvr, slot, parent);
    }

    KAction* copy(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idCopy, recvr, slot, parent);
    }

    KAction* paste(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idPaste, recvr, slot, parent);
    }

    KAction* find(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idFind, recvr, slot, parent);
    }

    KAction* findNext(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idFindNext, recvr, slot, parent);
    }

    KAction* findPrev(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idFindPrev, recvr, slot, parent);
    }

    KAction* showMenubar(const QObject* recvr, const char* slot, QObject* parent)
    {
        return makeAction(idShowMenubar, recvr, slot, parent);
    }
}


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
