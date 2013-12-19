
#include "EnginePlugin.h"
#include "DynamicLib.h"

namespace Demi
{
    typedef void(*DLL_START_PLUGIN)(void);
    typedef void(*DLL_STOP_PLUGIN)(void);

    void DiPlugin::LoadPlugin(const DiString& pgName)
    {
        DiDynLib* lib = DiDynLib::LoadLib(pgName);
        if (lib)
        {
            DLL_START_PLUGIN entryFunc = (DLL_START_PLUGIN)lib->GetSymbol("PluginBegin");

            if (!entryFunc)
            {
                DI_WARNING("Cannot find the beginning function of the plugin : %s", pgName.c_str());
            }

            entryFunc();
        }
    }

    void DiPlugin::UnloadPlugin(const DiString& pgName)
    {
        if (!DiDynLib::HasLib(pgName))
        {
            DI_WARNING("The plugin : %s has not loaded yet.", pgName.c_str());
            return;
        }

        DiDynLib* lib = DiDynLib::LoadLib(pgName);
        DI_ASSERT(lib);
        DLL_START_PLUGIN endFunc = (DLL_START_PLUGIN)lib->GetSymbol("PluginEnd");

        if (!endFunc)
        {
            DI_WARNING("Cannot find the ending function of the plugin : %s", pgName.c_str());
        }

        endFunc();
    }
}