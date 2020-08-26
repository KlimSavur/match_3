#include "engine_plugin.h"

#include "bubblesmodel.h"

#include <qqml.h>

void EnginePlugin::registerTypes(const char *uri)
{
    // @uri Engine
    qmlRegisterType<BublesModel>(uri, 1, 0, "BublesModel");
}

