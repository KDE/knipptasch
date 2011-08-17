#include "recentfiles.h"

namespace QSint
{


#define MAX_COUNT 10

RecentFiles::RecentFiles(QWidget *parent) :
    QObject(parent),
    maxCount(MAX_COUNT)
{
    recentMenu = new QMenu(parent);
    //connect(recentMenu, SIGNAL(triggered(QAction*)), this, SLOT(chosen(QAction*)));
}

void RecentFiles::add(const QString &filename, const QIcon &icon)
{
    QString name = QDir::toNativeSeparators(filename);

    for (int i = 0; i < recentList.count(); i++)
    {
        if (recentList.at(i).first == name)
        {
            recentList.move(i, 0);
            updateMenu();
            return;
        }
    }

    recentList.prepend(QPair<QString,QIcon>(name, icon));

    while (recentList.count() > maxCount)
        recentList.removeLast();

    updateMenu();
}

void RecentFiles::remove(const QString &filename)
{
    QString name = QDir::toNativeSeparators(filename);

    for (int i = 0; i < recentList.count(); i++)
    {
        if (recentList.at(i).first == name)
        {
            recentList.removeAt(i);
            updateMenu();
            return;
        }
    }
}

void RecentFiles::clear()
{
    recentList.clear();
    updateMenu();
}

void RecentFiles::updateMenu()
{
    recentMenu->clear();
    populateMenu(recentMenu);
}

void RecentFiles::populateMenu(QMenu *menu)
{
    if (!menu)
        return;

    disconnect(menu, SIGNAL(triggered(QAction*)), this, SLOT(chosen(QAction*)));
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(chosen(QAction*)));

    for (int i = 0; i < recentList.count(); i++)
    {
        menu->addAction(recentList.at(i).second, recentList.at(i).first);
    }
}

void RecentFiles::setMaxFileCount(int max)
{
  maxCount = max <= 0 ? MAX_COUNT : max;
  if (max >= recentList.count())
    return;

  while (recentList.count() > maxCount)
      recentList.removeLast();

  updateMenu();
}

void RecentFiles::chosen(QAction* act)
{
    QString name = act->text();

    for (int i = 0; i < recentList.count(); i++)
    {
        if (recentList.at(i).first == name)
        {
            recentList.move(i, 0);
            updateMenu();
            emit activated(name);
            break;
        }
    }
}

void RecentFiles::readConfig(QSettings &set)
{
    int cnt = set.beginReadArray("RecentFiles");
    maxCount = set.value("maxCount", maxCount).toInt();
    for (int i = 0; i < cnt; i++)
    {
        set.setArrayIndex(i);
        recentList.append(QPair<QString,QIcon>(set.value("file").toString(), QIcon()));
    }
    set.endArray();

    updateMenu();
}

void RecentFiles::writeConfig(QSettings &set)
{
    set.beginWriteArray("RecentFiles", recentList.count());
    set.setValue("maxCount", maxCount);
    for (int i = 0; i < recentList.count(); i++)
    {
        set.setArrayIndex(i);
        set.setValue("file", recentList.at(i).first);
    }
    set.endArray();
}


} // namespace
