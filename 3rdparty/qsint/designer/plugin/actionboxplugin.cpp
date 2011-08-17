#include "actionboxplugin.h"

#include <QtCore/QtPlugin>

#include <QSint>

using namespace QSint;

ActionBoxPlugin::ActionBoxPlugin(QObject *parent) :
    QObject(parent)
{
    m_initialized = false;
}

void ActionBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ActionBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ActionBoxPlugin::createWidget(QWidget *parent)
{
    return new ActionBox(parent);
}

QString ActionBoxPlugin::name() const
{
    return QLatin1String("QSint::ActionBox");
}

QString ActionBoxPlugin::group() const
{
    return QLatin1String("QSint Widgets");
}

QIcon ActionBoxPlugin::icon() const
{
    return QIcon(":/images/ActionBox.png");
}

QString ActionBoxPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ActionBoxPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ActionBoxPlugin::isContainer() const
{
    return true;
}

QString ActionBoxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QSint::ActionBox\" name=\"ActionBox\">\n</widget>\n");
}

QString ActionBoxPlugin::includeFile() const
{
    return QLatin1String("QSint");
}
