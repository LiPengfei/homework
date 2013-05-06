/********************************************************************/
/*  Copyright(c): LiPengfei											*/
/*  FileName	: MysqlManager.h 									*/
/*  Creator 	: LiPengfei											*/
/*  Date		: 03/01/2013										*/						
/*  Comment	: 														*/		
/********************************************************************/

#ifndef __BLAZER_SABRE_MYSQLMANAGER_H__
#define __BLAZER_SABRE_MYSQLMANAGER_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"
#include "DesignPattern/Singleton.h"

#include <Winsock2.h>
#include <mysql.h>
#include <my_global.h>
#include <stdexcept>
#include <map>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BDatabaseTuple;
class BDatabaseTable;

class BMysql : public BUnCopyable
{
private:
    DWORD m_nId;
    MYSQL m_mysql;
    bool  m_bConnected;

public:
    BMysql();
    virtual ~BMysql();

public:
    DWORD GetId() const
    {
        return m_nId;
    }

    void SetId(DWORD nId)
    {
        m_nId = nId;
    }

    int Connect(const char *cpHost, const char *cpUser,
        const char *cpPwd, const char *cpDb, unsigned nPort);

    int Connect(const char *cpHost, const char *cpUser,
        const char *cpPwd, const char *cpDb, const char *cpPort);
    int DisConnect();
    virtual int Query(const char *cpSql, OUT BDatabaseTable &dbTable);
    virtual int AddTuple(const char *cpSql);
    virtual int DeleteTuple(const char *cpSql);
    virtual int ModifyTuple(const char *cpSql);
    virtual int ModifyTable(const char *cpSql);

    int SetAutoCommit(bool bMode);
    int Commit();
    int Rollback();

    int ChangeUser(const char *cpUser, const char *cpPwd);
    int SetCurrentDb(const char *cpDbName);
    int CraeteNewDb(const char *cpDbName);
    int DropDb(const char *cpDbName);

    int Ping();
    int GetLastError();
    long long GetLastSqlEffectRows();

    const char *GetCharSetName();

public:
    static BOOL Init()
    {
        return TRUE;
    }

    static void UnInit()
    {
        mysql_thread_end();
        mysql_library_end();
    }

protected:
    inline int BMysql::DoSql(const char *cpSql)
    {
        if ((!cpSql) || !(cpSql[0]))
        {
            return -1;
        }

        return mysql_real_query(&m_mysql, cpSql, strlen(cpSql));
    }

    int ConfigMysql(mysql_option typeOption, void *valueOption);
    int IsThreadSafe(); 
};

class BMysqlTable : public BMysql
{
private:
    typedef std::string STRING;

private:
    STRING m_strTableName;

public:
    BMysqlTable(){ }
    BMysqlTable(const STRING &strTableName)
        : m_strTableName(strTableName)
    { }
    virtual ~BMysqlTable() { }

public:
    virtual int Query(const char *cpSql, OUT BDatabaseTable &dbTable);
    virtual int AddTuple(const char *cpSql);
    virtual int DeleteTuple(const char *cpSql);

    void SetName(const STRING &strName)
    {
        m_strTableName = strName;
    }
    STRING GetName() const
    {
        return m_strTableName;
    }
};

typedef BSharedPtr<BMysql>      BSPMysql;
typedef BSharedPtr<BMysqlTable> BSPMysqlTable;

class BMysqlTableManager
{
public:
    DECLARE_SINGLETON_PATTERN(BMysqlTableManager);

    typedef std::map<DWORD, BSPMysqlTable> Map_SPMysqlTable;
private:
    Map_SPMysqlTable m_mapMysqlTable;

public:
    BMysqlTableManager()
    {
        m_mapMysqlTable.clear();
    }

    ~BMysqlTableManager()
    {
        m_mapMysqlTable.clear();
    }

public:
    BOOL Add (DWORD nId, const BSPMysqlTable &spMysqlTable)
    {
        BZ_CHECK_RETURN_BOOL(spMysqlTable);
        
        // check if exsits
        BOOL bRet = Get(nId, NULL);
        BZ_CHECK_RETURN_BOOL(FALSE == bRet);

        m_mapMysqlTable[nId] = spMysqlTable;
        return TRUE;
    }

    BOOL Add (DWORD nId, BMysqlTable *pMysqlTable)
    {
        BZ_CHECK_RETURN_BOOL(pMysqlTable);

        // check if exsist;
        BOOL bRet = Get(nId, NULL);
        BZ_CHECK_RETURN_BOOL(FALSE == bRet);

        m_mapMysqlTable[nId] = BSPMysqlTable(pMysqlTable);
        return TRUE;
    }

    BOOL Del (DWORD nId)
    {
        Map_SPMysqlTable::const_iterator iter = m_mapMysqlTable.find(nId);

        if (m_mapMysqlTable.end() != iter)
        {
            m_mapMysqlTable.erase(iter);
            return TRUE;
        }

        return FALSE;
    }

    BOOL Get(DWORD nId, BSPMysqlTable *pspMysqlTable) const
    {
        Map_SPMysqlTable::const_iterator iter = m_mapMysqlTable.find(nId);

        if (m_mapMysqlTable.end() != iter)
        {
            if (pspMysqlTable)
            {
                *pspMysqlTable = iter->second;
            }

            return TRUE;
        }

        return FALSE;
    }
};

typedef BSharedPtr<BMysqlTableManager> BSPMysqlTableManager;

BZ_DECLARE_NAMESPACE_END

#endif
