#include "qsintplugin.h"

#include "scrollpannerplugin.h"
#include "colorgridplugin.h"
#include "colorbuttonplugin.h"
#include "actionboxplugin.h"
#include "actionlabelplugin.h"
#include "actionpanelplugin.h"
#include "actiongroupplugin.h"

QSintPlugin::QSintPlugin(QObject *parent) :
    QObject(parent)
{
    widgets.append(new ActionPanelPlugin(this));
    widgets.append(new ActionGroupPlugin(this));
    widgets.append(new ActionLabelPlugin(this));
    widgets.append(new ActionBoxPlugin(this));
    widgets.append(new ColorGridPlugin(this));
    widgets.append(new ColorButtonPlugin(this));
    widgets.append(new ScrollPannerPlugin(this));
}

Q_EXPORT_PLUGIN2(qsintplugin, QSintPlugin)
