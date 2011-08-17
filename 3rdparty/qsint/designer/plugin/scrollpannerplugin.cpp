#include "scrollpannerplugin.h"

#include <QtCore/QtPlugin>

#include <QSint>

using namespace QSint;

ScrollPannerPlugin::ScrollPannerPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ScrollPannerPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ScrollPannerPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ScrollPannerPlugin::createWidget(QWidget *parent)
{
    return new ScrollPanner(parent);
}

QString ScrollPannerPlugin::name() const
{
    return QLatin1String("QSint::ScrollPanner");
}

QString ScrollPannerPlugin::group() const
{
    return QLatin1String("QSint Widgets");
}

QIcon ScrollPannerPlugin::icon() const
{
    return QIcon(":/images/ScrollPanner.png");
}

QString ScrollPannerPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ScrollPannerPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ScrollPannerPlugin::isContainer() const
{
    return false;
}

QString ScrollPannerPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QSint::ScrollPanner\" name=\"scrollPanner\">\n</widget>\n");
}

QString ScrollPannerPlugin::includeFile() const
{
    return QLatin1String("QSint");
}
