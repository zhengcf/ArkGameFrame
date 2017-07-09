// -------------------------------------------------------------------------
//    @FileName			:    NFGameServerNet_ServerPlugin.h
//    @Author           :    Ark Game Tech
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFGameServerNet_ServerPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_GAMESERVERNET_SERVERPLUGIN_H
#define NF_GAMESERVERNET_SERVERPLUGIN_H

///
#include "SDK/Interface/AFIPlugin.h"
#include "SDK/Interface/AFIPluginManager.h"

#ifdef NFGAMESERVERNET_SERVERPLUGIN_EXPORTS
#define NFGAMESERVERNET_SERVERPLUGIN_API __declspec(dllexport)
#else
#define NFGAMESERVERNET_SERVERPLUGIN_API __declspec(dllimport)
#endif

class NFGameServerNet_ServerPlugin : public AFIPlugin
{
public:
    NFGameServerNet_ServerPlugin(AFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif