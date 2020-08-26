#ifndef ENGINE_PLUGIN_H
#define ENGINE_PLUGIN_H

#include <QQmlExtensionPlugin>

class EnginePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // ENGINE_PLUGIN_H
