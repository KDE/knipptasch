#ifndef QSINTPLUGIN_H
#define QSINTPLUGIN_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class QSintPlugin: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    QSintPlugin(QObject *parent = 0);

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const
    {
        return widgets;
    }

private:
    QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // QSINTPLUGIN_H
