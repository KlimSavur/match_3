#include "engine_plugin.h"
#include "json_loader.h"
#include "bubblesmodel.h"
#include <QQmlEngine>
#include <qqml.h>

void EnginePlugin::registerTypes(const char *uri)
{
    // @uri Engine
    qmlRegisterType<BubblesModel>(uri, 1, 0, "BubblesModel");
    qmlRegisterSingletonType<JSON_loader>(uri, 1, 0, "JSON_loader", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(scriptEngine)
        JSON_loader* p = JSON_loader::getInstace();
        engine->setObjectOwnership(p, QQmlEngine::CppOwnership);
        return p;
    });

}

