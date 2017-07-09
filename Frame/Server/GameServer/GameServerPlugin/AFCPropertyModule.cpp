// -------------------------------------------------------------------------
//    @FileName         :    AFCPropertyModule.cpp
//    @Author               :    Ark Game Tech
//    @Date                 :    2013-07-05
//    @Module               :    AFCPropertyModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "AFCPropertyModule.h"
#include "SDK/Core/AFTime.h"

bool AFCPropertyModule::Init()
{

    return true;
}

bool AFCPropertyModule::Shut()
{
    return true;
}

bool AFCPropertyModule::Execute()
{
    return true;
}

bool AFCPropertyModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<AFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<AFIElementModule>();
    m_pClassModule = pPluginManager->FindModule<AFIClassModule>();
    m_pPropertyConfigModule = pPluginManager->FindModule<AFIPropertyConfigModule>();
    m_pLevelModule = pPluginManager->FindModule<AFILevelModule>();

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &AFCPropertyModule::OnObjectClassEvent);

    return true;
}

int AFCPropertyModule::GetPropertyValue(const AFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType)
{
    if(NFPropertyGroup::NPG_ALL != eGroupType)
    {
        return m_pKernelModule->GetRecordInt(self, NFrame::Player::R_CommPropertyValue(), eGroupType, strPropertyName);
    }

    return m_pKernelModule->GetPropertyInt(self, strPropertyName);
}

int AFCPropertyModule::SetPropertyValue(const AFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue)
{
    if(NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<AFIObject> pObject = m_pKernelModule->GetObject(self);
        if(nullptr != pObject)
        {
            NF_SHARE_PTR<AFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if(nullptr != pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt(eGroupType, strPropertyName, nValue);
            }
        }

        //return m_pKernelModule->SetRecordInt( self, mstrCommPropertyName, eGroupType, *pTableCol, nValue );
    }

    //��̬����û�У������õ�����ֵ
    m_pKernelModule->SetPropertyInt(self, strPropertyName, nValue);

    return 0;
}


int AFCPropertyModule::AddPropertyValue(const AFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue)
{
    if(NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<AFIObject> pObject = m_pKernelModule->GetObject(self);
        if(nullptr != pObject)
        {
            NF_SHARE_PTR<AFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if(nullptr != pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName);

                return pRecord->SetInt(eGroupType, strPropertyName, nPropertyValue + nValue);
            }
        }
    }

    return 0;
}

int AFCPropertyModule::SubPropertyValue(const AFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue)
{
    if(NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<AFIObject> pObject = m_pKernelModule->GetObject(self);
        if(nullptr != pObject)
        {
            NF_SHARE_PTR<AFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if(nullptr != pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName);

                return pRecord->SetInt(eGroupType, strPropertyName, nPropertyValue - nValue);
            }
        }
    }

    return 0;
}

int AFCPropertyModule::OnObjectLevelEvent(const AFGUID& self, const std::string& strPropertyName, const AFIDataList::TData& oldVar, const AFIDataList::TData& newVar)
{
    RefreshBaseProperty(self);

    FullHPMP(self);
    FullSP(self);

    return 0;
}

int AFCPropertyModule::OnRecordPropertyEvent(const AFGUID& self, const RECORD_EVENT_DATA& xEventData, const AFIDataList::TData& oldVar, const AFIDataList::TData& newVar)
{
    //������ֵ
    const std::string& strRecordName = xEventData.strRecordName;
    const int nOpType = xEventData.nOpType;
    const int nRow = xEventData.nRow;
    const int nCol = xEventData.nCol;

    int nAllValue = 0;
    NF_SHARE_PTR<AFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
    for(int i = 0; i < (int)(NFPropertyGroup::NPG_ALL); i++)
    {
        if(i < pRecord->GetRows())
        {
            int nValue = pRecord->GetInt(i, nCol);
            nAllValue += nValue;
        }
    }

    m_pKernelModule->SetPropertyInt(self, pRecord->GetColTag(nCol), nAllValue);

    return 0;
}

int AFCPropertyModule::OnObjectClassEvent(const AFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const AFIDataList& var)
{
    if(strClassName == NFrame::Player::ThisName())
    {
        if(CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
        {
            NF_SHARE_PTR<AFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if(nullptr != pRecord)
            {
                for(int i = 0; i < NPG_ALL; i++)
                {
                    pRecord->AddRow(-1);
                }
            }

            m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &AFCPropertyModule::OnObjectLevelEvent);

            // TODO:һ�����Իص�
            m_pKernelModule->AddRecordCallBack(self, NFrame::Player::R_CommPropertyValue(), this, &AFCPropertyModule::OnRecordPropertyEvent);
        }
        else if(CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OnlineCount());
            if(nOnlineCount <= 0 && m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID()) > 0)
            {
                //��һ�γ��������û�������
                m_pKernelModule->SetPropertyInt(self, NFrame::Player::Level(), 1);
            }
        }
        else if(CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OnlineCount());
            m_pKernelModule->SetPropertyInt(self, NFrame::Player::OnlineCount(), (nOnlineCount + 1));

        }
    }

    return 0;
}

int AFCPropertyModule::RefreshBaseProperty(const AFGUID& self)
{
    NF_SHARE_PTR<AFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
    if(nullptr == pRecord)
    {
        return 1;
    }

    //��ʼ����+�ȼ�����(ְҵ����)
    int eJobType = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Job());
    int nLevel = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level());

    for(int i = 0; i < pRecord->GetCols(); ++i)
    {
        const std::string& strColTag = pRecord->GetColTag(i);
        int nValue = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, strColTag);
        SetPropertyValue(self, strColTag, NFPropertyGroup::NPG_JOBLEVEL, nValue);
    }

    return 1;
}

bool AFCPropertyModule::FullHPMP(const AFGUID& self)
{
    AFINT64 nMaxHP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());
    if(nMaxHP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nMaxHP);
    }

    AFINT64 nMaxMP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXMP());
    if(nMaxMP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nMaxMP);
    }

    return true;
}

bool AFCPropertyModule::AddHP(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    AFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());

    if(nCurValue > 0)
    {
        nCurValue += nValue;
        if(nCurValue > nMaxValue)
        {
            nCurValue = nMaxValue;
        }

        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);
    }

    return true;
}

bool AFCPropertyModule::EnoughHP(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool AFCPropertyModule::ConsumeHP(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);

        return true;
    }

    return false;
}

bool AFCPropertyModule::AddMP(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    AFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXMP());

    nCurValue += nValue;
    if(nCurValue > nMaxValue)
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

    return true;
}

bool AFCPropertyModule::ConsumeMP(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

        return true;
    }

    return false;
}

bool AFCPropertyModule::EnoughMP(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool AFCPropertyModule::FullSP(const AFGUID& self)
{
    AFINT64 nMAXCSP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXSP());
    if(nMAXCSP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nMAXCSP);

        return true;
    }

    return false;
}

bool AFCPropertyModule::AddSP(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
    AFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXSP());

    nCurValue += nValue;
    if(nCurValue > nMaxValue)
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nCurValue);

    return true;
}

bool AFCPropertyModule::ConsumeSP(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCSP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
    if((nCSP > 0) && (nCSP - nValue >= 0))
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nCSP);

        return true;
    }

    return false;
}

bool AFCPropertyModule::EnoughSP(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool AFCPropertyModule::AddMoney(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), nCurValue);

    return false;
}

bool AFCPropertyModule::ConsumeMoney(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    nCurValue -= nValue;
    if(nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), nCurValue);

        return true;
    }

    return false;
}

bool AFCPropertyModule::EnoughMoney(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool AFCPropertyModule::AddDiamond(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), nCurValue);

    return false;
}

bool AFCPropertyModule::ConsumeDiamond(const AFGUID& self, const AFINT64& nValue)
{
    if(nValue <= 0)
    {
        return false;
    }

    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    nCurValue -= nValue;
    if(nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), nCurValue);

        return true;
    }

    return false;
}

bool AFCPropertyModule::EnoughDiamond(const AFGUID& self, const AFINT64& nValue)
{
    AFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    if((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}