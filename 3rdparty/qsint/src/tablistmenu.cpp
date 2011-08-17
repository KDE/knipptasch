#include "tablistmenu.h"


namespace QSint
{


TabListMenu::TabListMenu(QWidget *parent) :
    QMenu(parent),
    m_tabs(0),
    m_icons(true)
{
    connect(this, SIGNAL(aboutToShow()), this, SLOT(createMenu()));
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(switchTab(QAction*)));
}

void TabListMenu::setTabs(QTabWidget *tabs)
{
    clear();
    m_tabs = tabs;
}

void TabListMenu::createMenu()
{
    clear();
    doCreateMenu(m_tabs);
}

void TabListMenu::doCreateMenu(QTabWidget *tabs)
{
    if (!tabs || !tabs->count())
        return;

    QAction *a;
    int crnt = tabs->currentIndex();

    for (int i = 0; i < tabs->count(); i++)
    {
        a = addAction(tabs->tabText(i));

        doCreateMenuItem(i, crnt == i, a, tabs);
    }
}

void TabListMenu::doCreateMenuItem(int index, bool current, QAction *item, QTabWidget *tabs)
{
    if (m_icons)
        item->setIcon(tabs->tabIcon(index));

    item->setData(index);

    item->setCheckable(true);

    if (current)
        item->setChecked(true);
}

void TabListMenu::switchTab(QAction *from)
{
    doSwitchTab(from, m_tabs);
}

void TabListMenu::doSwitchTab(QAction *from, QTabWidget *tabs)
{
    if (!from || !tabs)
        return;

    int idx = from->data().toInt();
    tabs->setCurrentIndex(idx);
}


} // namespace QSint

