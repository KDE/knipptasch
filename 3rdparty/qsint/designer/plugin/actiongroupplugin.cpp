#include "actiongroupplugin.h"

#include <QtCore/QtPlugin>

#include <QSint>

using namespace QSint;

ActionGroupPlugin::ActionGroupPlugin(QObject *parent) :
    QObject(parent)
{
    m_initialized = false;
}

void ActionGroupPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ActionGroupPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ActionGroupPlugin::createWidget(QWidget *parent)
{
    return new ActionGroup(parent);
}

QString ActionGroupPlugin::name() const
{
    return QLatin1String("QSint::ActionGroup");
}

QString ActionGroupPlugin::group() const
{
    return QLatin1String("QSint Widgets");
}

QIcon ActionGroupPlugin::icon() const
{
    return QIcon(":/images/ActionGroup.png");
}

QString ActionGroupPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ActionGroupPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ActionGroupPlugin::isContainer() const
{
    return true;
}

QString ActionGroupPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QSint::ActionGroup\" name=\"ActionGroup\">\n</widget>\n");
}

QString ActionGroupPlugin::includeFile() const
{
    return QLatin1String("QSint");
}
