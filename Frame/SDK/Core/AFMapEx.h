// -------------------------------------------------------------------------
//    @FileName         :    AFMapEx.h
//    @Author           :    Ark Game Tech
//    @Date             :    2012-03-01
//    @Module           :    ���������ָ��ר��,�мǣ�������ѭ�����Լ�ɾ���Լ�
//
// -------------------------------------------------------------------------

#ifndef NF_MAPEX_H
#define NF_MAPEX_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "SDK/Interface/AFPlatform.h"

template <typename T , typename TD>
class AFMapEx
{
public:
    typedef std::map<T, NF_SHARE_PTR<TD> > NFMapOBJECT;

    AFMapEx() {};
    virtual ~AFMapEx()
    {
    };

    virtual bool AddElement(const T& name, const NF_SHARE_PTR<TD> data)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename NFMapOBJECT::value_type(name, data));
            return true;
        }

        return false;
    }

    virtual bool SetElement(const T& name, const NF_SHARE_PTR<TD> data)
    {
        mObjectList[name] = data;

        return true;
    }

    virtual bool RemoveElement(const T& name)
    {
        NF_SHARE_PTR<TD> pData;
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            pData = itr->second;
            mObjectList.erase(itr);

            return true;
        }

        return false;
    }

    virtual TD* GetElementNude(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second.get();
        }
        
        return NULL;
    }

    virtual NF_SHARE_PTR<TD> GetElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        
        return nullptr;
    }
    virtual TD* FirstNude(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        
        return NULL;
    }

    virtual TD* NextNude(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        
        return NULL;
    }

    virtual TD* FirstNude()
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        
        return NULL;
    }

    virtual TD* NextNude()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        
        return NULL;
    }

    virtual NF_SHARE_PTR<TD> First()
    {
        if (mObjectList.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }

        return nullptr;
    }

    virtual NF_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>();
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }

        return nullptr;
    }

    virtual NF_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NF_SHARE_PTR<TD>();
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }

        return nullptr;
    }

    virtual NF_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NF_SHARE_PTR<TD>();
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }

        return nullptr;
    }

    int Count()
    {
        return (int)mObjectList.size();
    }

    bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }

private:
    NFMapOBJECT     mObjectList;
    typename NFMapOBJECT::iterator mObjectCurIter;
};

template <typename T , typename TD>
class AFConcurrentMap : public AFMapEx<T, TD>
{
public:
protected:
private:
};
#endif