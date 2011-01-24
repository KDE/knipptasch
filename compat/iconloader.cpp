/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 * Copyright 2010 by Volker Lanz <vl@fidra.de>
 * Copyright 2005-09 by the Quassel Project
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
#include "iconloader.h"

#if !defined(HAVE_KDE)

#include "utils.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QPixmap>
#include <QFile>


static int groupSize[] = { 48, 22, 22, 16, 32, 22 };  // default sizes taken from Oxygen


IconLoader* IconLoader::global()
{
    static IconLoader* instance = NULL;

    if (instance == NULL)
    {
        instance = new IconLoader(NULL);
        instance->setTheme( "oxygen" );
    }

    return instance;
}


IconLoader::IconLoader(QObject* parent)
  : QObject(parent)
{
}


void IconLoader::setTheme(const QString& theme)
{
    m_Theme = theme;

    // check which dirs could contain themed icons
    m_ThemedIconDirNames.clear();
    m_PlainIconDirNames.clear();

    // First, look for a system theme
    // This is supposed to only work on Unix, though other platforms might set $XDG_DATA_DIRS if they please.
    QStringList iconDirNames = QString(qgetenv("XDG_DATA_DIRS")).split(':', QString::SkipEmptyParts);

    if (!iconDirNames.isEmpty())
        for (int i = 0; i < iconDirNames.count(); i++)
            iconDirNames[i].append(QString("/icons/"));

#if defined(Q_OS_UNIX)
    else
        iconDirNames << "/usr/share/icons/";

    // Add our prefix too
    QString appDir = QCoreApplication::applicationDirPath();
    int binpos = appDir.lastIndexOf("/bin");

    if (binpos >= 0)
    {
        appDir.replace(binpos, 4, "/share");
        appDir.append("/icons/");

        if (!iconDirNames.contains(appDir))
            iconDirNames.append(appDir);
    }
#endif

    // Now look for an icons/ subdir in our data paths
    foreach (const QString& dir, findDataDirPaths())
        iconDirNames << dir + "icons/";

    // Add our resource path too
    iconDirNames << ":/icons/";

    // Ready do add theme names
    foreach (const QString& dir, iconDirNames)
    {
        QString path = dir + theme + '/';

        if (QFile::exists(path))
            m_ThemedIconDirNames << path;
    }
}


QPixmap IconLoader::loadIcon(const QString& name, IconLoader::Group group, int size)
{
    if (group < 0 || group >= LastGroup)
    {
        qWarning() << "Invalid icon group!";
        return QPixmap();
    }

    if (size == 0)
        size = groupSize[group];

    const QString path = findIconPath(name, size);

    if (path.isEmpty())
        return QPixmap();

// 		qDebug() << "load icon: " << path;
    return QPixmap(path);
}


QString IconLoader::findIconPath(const QString& name, int size) const
{
    foreach (const QString& basedir, m_ThemedIconDirNames)
    {
        const QDir sizedir(QString("%1/%2x%2").arg(basedir).arg(size));

        if (sizedir.exists())
        {
            // ignore context, i.e. scan all subdirs
            const QStringList contextdirs = sizedir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

            foreach (const QString& dir, contextdirs)
            {
                const QString path = QString("%1/%2/%3.png").arg(sizedir.absolutePath(), dir, name);
                if (QFile::exists(path))
                    return path;
            }
        }
    }

    foreach (const QString& dir, m_PlainIconDirNames)
    {
        const QString path = QString("%1/%2").arg(dir, name);
        if (QFile::exists(path))
            return path;
    }

    qWarning() << "Icon not found:" << name << size;

    return QString();
}



QPixmap DesktopIcon(const QString& name, int force_size)
{
    return IconLoader::global()->loadIcon(name, IconLoader::Desktop, force_size);
}

QPixmap BarIcon(const QString& name, int force_size)
{
    return IconLoader::global()->loadIcon(name, IconLoader::Toolbar, force_size);
}

QPixmap MainBarIcon(const QString& name, int force_size)
{
    return IconLoader::global()->loadIcon(name, IconLoader::MainToolbar, force_size);
}

QPixmap SmallIcon(const QString& name, int force_size)
{
    return IconLoader::global()->loadIcon(name, IconLoader::Small, force_size);
}

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
