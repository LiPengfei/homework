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
#include <Winsock2.h>
#include <mysql.h>
#include <my_global.h>
#include <stdexcept>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    class BDatabaseTuple;
    class BDatabaseTable;

    class BMysqlManager : public BUnCopyable
    {
#ifdef _DEBUG
    public:
#else
    private:
#endif // DEBUG
        MYSQL m_mysql;
        bool  m_bConnected;

    public:
        BMysqlManager() throw(std::runtime_error);
        virtual ~BMysqlManager();

    public:
        int Connect(const char *cszHost, const char *cszUser,
            const char *cszPwd, const char *cszDb, unsigned nPort);
        int DisConnect();
        int Query(const char *cszSql, OUT BDatabaseTable &dbTable);
        int AddTuple(const char *cszSql);
        int DeleteTuple(const char *cszSql);
        int ModifyTuple(const char *cszSql);
        int ModifyTable(const char *cszSql);

        int SetAutoCommit(bool bMode);
        int Commit();
        int Rollback();

        int ChangeUser(const char *cszUser, const char *cszPwd);
        int SetCurrentDb(const char *cszDbName);
        int CraeteNewDb(const char *cszDbName);
        int DropDb(const char *cszDbName);

        int Ping();
        int GetLastError();
        long long GetLastSqlEffectRows();

        const char *GetCharSetName();

    protected:

        inline int BMysqlManager::DoSql(const char *cszSql)
        {
            if ((!cszSql) || !(cszSql[0]))
            {
                return -1;
            }

            return mysql_real_query(&m_mysql, cszSql, strlen(cszSql));
        }

        int ConfigMysql(mysql_option typeOption, void *valueOption);
        int IsThreadSafe(); 
    };

BZ_DECLARE_NAMESPACE_END

#endif
