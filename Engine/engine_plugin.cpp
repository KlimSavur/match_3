#include "engine_plugin.h"

#include "bubblesmodel.h"

#include <qqml.h>

void EnginePlugin::registerTypes(const char *uri)
{
    // @uri Engine
    qmlRegisterType<BubblesModel>(uri, 1, 0, "BubblesModel");
}

