// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyConfigModule.h
//    @Author           :    Ark Game Tech
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyConfigModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_CONFIG_MODULE_H
#define NFC_PROPERTY_CONFIG_MODULE_H

#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "SDK/Interface/NFIPropertyConfigModule.h"
#include "SDK/Interface/NFIClassModule.h"
#include "SDK/Interface/NFIElementModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFCPropertyConfigModule
    : public NFIPropertyConfigModule
{
public:
    NFCPropertyConfigModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCPropertyConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual int CalculateBaseValue(const int nJob, const int nLevel, const std::string& strProperty);
    virtual bool LegalLevel(const int nJob, const int nLevel);

protected:
    void Load();

private:
    //
    //diffent job, diffrent PropertyID[Level->EffectData]
    NFMapEx<int, NFMapEx<int, std::string> > mhtCoefficienData;

    NFIClassModule* m_pClassModule;
    NFIElementModule* m_pElementModule;
};


#endif