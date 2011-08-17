#include "actionpanelplugin.h"

#include <QtCore/QtPlugin>

#include <QSint>

using namespace QSint;

ActionPanelPlugin::ActionPanelPlugin(QObject *parent) :
    QObject(parent)
{
    m_initialized = false;
}

void ActionPanelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ActionPanelPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ActionPanelPlugin::createWidget(QWidget *parent)
{
    return new ActionPanel(parent);
}

QString ActionPanelPlugin::name() const
{
    return QLatin1String("QSint::ActionPanel");
}

QString ActionPanelPlugin::group() const
{
    return QLatin1String("QSint Widgets");
}

QIcon ActionPanelPlugin::icon() const
{
    return QIcon(":/images/ActionPanel.png");
}

QString ActionPanelPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ActionPanelPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ActionPanelPlugin::isContainer() const
{
    return true;
}

QString ActionPanelPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QSint::ActionPanel\" name=\"ActionPanel\">\n</widget>\n");
}

QString ActionPanelPlugin::includeFile() const
{
    return QLatin1String("QSint");
}
