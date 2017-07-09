#include "SDK/Interface/AFIPlugin.h"
#include "SDK/Interface/AFIPluginManager.h"

#ifdef NFLOGPLUGIN_EXPORTS
#define NFLOGPLUGIN_API __declspec(dllexport)
#else
#define NFLOGPLUGIN_API __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class UtilityPlugin : public AFIPlugin
{
public:
    UtilityPlugin(AFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};