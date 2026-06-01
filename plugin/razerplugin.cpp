#include "razerplugin.h"
#include "razerbattery.h"

#include <qqml.h>

void RazerPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<RazerBattery>(uri, 1, 0, "RazerBattery");
}
