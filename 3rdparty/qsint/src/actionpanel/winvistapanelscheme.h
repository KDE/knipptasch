#ifndef WINVISTAPANELSCHEME_H
#define WINVISTAPANELSCHEME_H


#include "actionpanelscheme.h"


namespace QSint
{


class WinVistaPanelScheme : public ActionPanelScheme
{
public:
    WinVistaPanelScheme(QObject *parent=0);

    static ActionPanelScheme* defaultScheme()
    {
        static WinVistaPanelScheme scheme;
        return &scheme;
    }
};


}


#endif // WINVISTAPANELSCHEME_H
