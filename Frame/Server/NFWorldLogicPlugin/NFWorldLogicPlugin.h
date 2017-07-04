// -------------------------------------------------------------------------
//    @FileName			:    NFWorldLogicPlugin.h
//    @Author           :    Ark Game Tech
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFWorldLogicPluginModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLDLOGICPLUGIN_MODULE_H
#define NFC_WORLDLOGICPLUGIN_MODULE_H

#include "SDK/Interface/NFPlatform.h"
#include "SDK/Interface/NFIPlugin.h"
#include "SDK/Interface/NFIPluginManager.h"

#ifdef NFWORLDLOGICPLUGIN_EXPORTS
#define NFWORLDLOGICPLUGIN_API __declspec(dllexport)
#else
#define NFWORLDLOGICPLUGIN_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFWorldLogicPlugin : public NFIPlugin
{
public:
    NFWorldLogicPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif