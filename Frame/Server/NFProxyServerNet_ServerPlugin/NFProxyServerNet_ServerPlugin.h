// -------------------------------------------------------------------------
//    @FileName			:    NFGameServerNet_ServerPlugin.h
//    @Author           :    Ark Game Tech
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFGameServerNet_ServerPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_PROXYSERVERNET_SERVERPLUGIN_H
#define NF_PROXYSERVERNET_SERVERPLUGIN_H

///
#include "SDK/Interface/NFIPlugin.h"
#include "SDK/Interface/NFIPluginManager.h"

#ifdef NFPROXYSERVERNET_SERVERPLUGIN_EXPORTS
#define NFPROXYSERVERNET_SERVERPLUGIN_API __declspec(dllexport)
#else
#define NFPROXYSERVERNET_SERVERPLUGIN_API __declspec(dllimport)
#endif

class NFProxyServerNet_ServerPlugin : public NFIPlugin
{
public:
    NFProxyServerNet_ServerPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif