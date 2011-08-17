#include "colorgridplugin.h"

#include <QtCore/QtPlugin>

#include <QSint>

using namespace QSint;

ColorGridPlugin::ColorGridPlugin(QObject *parent) :
    QObject(parent)
{
    m_initialized = false;
}

void ColorGridPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ColorGridPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ColorGridPlugin::createWidget(QWidget *parent)
{
    return new ColorGrid(parent);
}

QString ColorGridPlugin::name() const
{
    return QLatin1String("QSint::ColorGrid");
}

QString ColorGridPlugin::group() const
{
    return QLatin1String("QSint Widgets");
}

QIcon ColorGridPlugin::icon() const
{
    return QIcon(":/images/ColorGrid.png");
}

QString ColorGridPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ColorGridPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ColorGridPlugin::isContainer() const
{
    return false;
}

QString ColorGridPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QSint::ColorGrid\" name=\"ColorGrid\">\n</widget>\n");
}

QString ColorGridPlugin::includeFile() const
{
    return QLatin1String("QSint");
}


