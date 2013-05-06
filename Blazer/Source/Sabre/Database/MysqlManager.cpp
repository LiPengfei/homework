#include "Database/DatabaseTable.h"
#include "Database/MysqlManager.h"

#include <string.h>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    /*
    DB_VALUE_TYPE
    {
    DB_NUMBER,
    DB_CHARACTERS,
    DB_BYTES,
    DB_BOOLEAN,
    DB_TIME,
    DB_DATE,
    DB_DATETIME,
    DB_TIMESTAMP,
    DB_NULL,
    DB_UNKNOWN
    }

    mysql_type
    enum enum_field_types { MYSQL_TYPE_DECIMAL, MYSQL_TYPE_TINY,
    MYSQL_TYPE_SHORT,  MYSQL_TYPE_LONG,
    MYSQL_TYPE_FLOAT,  MYSQL_TYPE_DOUBLE,
    MYSQL_TYPE_NULL,   MYSQL_TYPE_TIMESTAMP,
    MYSQL_TYPE_LONGLONG,MYSQL_TYPE_INT24,
    MYSQL_TYPE_DATE,   MYSQL_TYPE_TIME,
    MYSQL_TYPE_DATETIME, MYSQL_TYPE_YEAR,
    MYSQL_TYPE_NEWDATE, MYSQL_TYPE_VARCHAR,
    MYSQL_TYPE_BIT,
    MYSQL_TYPE_NEWDECIMAL=246,
    MYSQL_TYPE_ENUM=247,
    MYSQL_TYPE_SET=248,
    MYSQL_TYPE_TINY_BLOB=249,
    MYSQL_TYPE_MEDIUM_BLOB=250,
    MYSQL_TYPE_LONG_BLOB=251,
    MYSQL_TYPE_BLOB=252,
    MYSQL_TYPE_VAR_STRING=253,
    MYSQL_TYPE_STRING=254,
    MYSQL_TYPE_GEOMETRY=255

    };
    */

BZ_DB_VALUE_TYPE l_Mysqltp2DBtp(enum_field_types tp)
{
    switch(tp)
    {
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
    case MYSQL_TYPE_LONGLONG:
    case MYSQL_TYPE_INT24:
        return BZ_DB_NUMBER;

    case MYSQL_TYPE_TIMESTAMP:
        return BZ_DB_TIMESTAMP;

    case MYSQL_TYPE_DATE:
        return BZ_DB_DATE;

    case MYSQL_TYPE_DATETIME:
        return BZ_DB_DATETIME;

    case MYSQL_TYPE_TIME:
        return BZ_DB_TIME;

    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_VAR_STRING:
        return BZ_DB_CHARACTERS;

    case MYSQL_TYPE_NULL:
        return BZ_DB_NULL;

    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_BLOB:
        return BZ_DB_BYTES;

    case MYSQL_TYPE_BIT:
        return BZ_DB_BOOLEAN;
    }
    return BZ_DB_UNKNOWN;
}

BMysql::BMysql()
    : m_nId(0), m_bConnected(false)
{
    mysql_init(&m_mysql);
}

BMysql::~BMysql()
{
    if (m_bConnected)
    {
        DisConnect();
    }
}

int BMysql::Connect(const char *cpHost, 
                    const char *cpUser, 
                    const char *cpPwd, 
                    const char *cpDb, 
                    unsigned nPort)
{
    BZ_CHECK_C_STRING_RETURN_CODE(cpHost, -1);
    BZ_CHECK_C_STRING_RETURN_CODE(cpUser, -1);
    BZ_CHECK_C_STRING_RETURN_CODE(cpPwd, -1);
    BZ_CHECK_C_STRING_RETURN_CODE(cpDb, -1);
    BZ_CHECK_RETURN_CODE(nPort >= 0, -1);

    if (m_bConnected == true)
    {
        DisConnect();
    }

    if(!mysql_real_connect(&m_mysql, cpHost, cpUser, cpPwd, cpDb, nPort, NULL, 0))
    {
        return -1;
    }

    m_bConnected = true;
    return 0;
}

int BMysql::Connect(const char *cpHost, 
                    const char *cpUser, 
                    const char *cpPwd, 
                    const char *cpDb, 
                    const char *cpPort)
{
    unsigned nPort = (unsigned)atoi(cpPort);
    return Connect(cpHost, cpUser, cpPwd, cpDb, nPort);
}

int BMysql::DisConnect()
{
    if (!m_bConnected)
    {
        return 0;
    }

    m_bConnected = false;
    mysql_close (&m_mysql);
    return 0;
}

int BMysql::Query(const char *cpSql, OUT BDatabaseTable &dbTable )
{
    dbTable.Clean();

    BZ_CHECK_C_STRING_RETURN_CODE(cpSql, -1);

    if (-1 == DoSql(cpSql))
    {
        return -1;
    }

    MYSQL_RES * mysql_res = mysql_store_result(&m_mysql);
    if (!mysql_res)
    {
        return -1;
    }

    dbTable.m_nColCount   = mysql_num_fields(mysql_res);
    dbTable.m_nTupleCount = mysql_num_rows(mysql_res);

    if (0 == dbTable.m_nColCount || 0 == dbTable.m_nTupleCount)
    {
        mysql_free_result(mysql_res);
        return 0;
    }

    // set fields 
    MYSQL_FIELD   *mysql_fields = mysql_fetch_fields(mysql_res);
    size_t         nIdx         = 0;
    dbTable.m_columns           = new BDatabaseColumn[dbTable.m_nColCount];
    for (nIdx = 0; nIdx != dbTable.m_nColCount; ++nIdx)
    {
        BSimpleString ssTemp(mysql_fields[nIdx].name);
        BZ_DB_VALUE_TYPE vTemp =l_Mysqltp2DBtp(mysql_fields[nIdx].type);
        dbTable.m_columns[nIdx] = BDatabaseColumn(vTemp,ssTemp);
    }

    // set rows
    MYSQL_ROW mysql_row;
    dbTable.m_tuples = new BDatabaseTuple[dbTable.m_nTupleCount];
    nIdx             = 0;
    unsigned long *lengths;
    while (NULL != (mysql_row = mysql_fetch_row(mysql_res)))
    {
        lengths    = mysql_fetch_lengths(mysql_res);
        dbTable.m_tuples[nIdx].m_aCells = 
            new BDatabaseCell[dbTable.m_nColCount];
        for (size_t i = 0; i != dbTable.m_nColCount; ++i)
        {
            if (mysql_row[i])
            {
                BSimpleString ssTemp(mysql_row[i], lengths[i]);
                BDatabaseCell cTemp= BDatabaseCell(
                    dbTable.m_columns[i].m_emType,
                    ssTemp);
                dbTable.m_tuples[nIdx].m_aCells[i] = cTemp;
            }
            dbTable.m_tuples[nIdx].m_aCells[i].m_emType = dbTable.m_columns[i].m_emType;
        }
        ++nIdx;
    }

    mysql_free_result(mysql_res);
    return 0;
}

int BMysql::AddTuple(const char *cpSql)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return DoSql(cpSql);
}

int BMysql::DeleteTuple(const char *cpSql)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return DoSql(cpSql);
}

int BMysql::ModifyTuple(const char *cpSql)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return DoSql(cpSql);
}

int BMysql::ModifyTable(const char *cpSql)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return DoSql(cpSql);
}

int BMysql::SetAutoCommit(bool bMode)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return bMode ? mysql_autocommit(&m_mysql, 1) : mysql_autocommit(&m_mysql, bMode);
}

int BMysql::Commit()
{
    if (!m_bConnected)
    {
        return -1;
    }
    return mysql_commit(&m_mysql);
}

int BMysql::Rollback()
{
    if (!m_bConnected)
    {
        return -1;
    }
    return mysql_rollback(&m_mysql);
}

int BMysql::ChangeUser(const char *cpUser, const char *cpPwd)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return mysql_change_user(&m_mysql, cpUser, cpPwd, NULL);
}

int BMysql::SetCurrentDb(const char *cpDbName)
{
    if (!m_bConnected)
    {
        return -1;
    }
    if ((!cpDbName) || (!cpDbName[0]))
    {
        return -1;
    }
    char buffer[1024];
    sprintf(buffer, "use %s", cpDbName);
    return mysql_real_query(&m_mysql, buffer, strlen(buffer));
}

int BMysql::CraeteNewDb(const char *cpDbName)
{
    if (!m_bConnected)
    {
        return -1;
    }

    if ((!cpDbName) || (!cpDbName[0]))
    {
        return -1;
    }

    char buffer[1024];
    sprintf(buffer, "create database %s", cpDbName);
    return mysql_query(&m_mysql, cpDbName);
}

int BMysql::DropDb(const char *cpDbName)
{
    if (!m_bConnected)
    {
        return -1;
    }

    if ((!cpDbName) || (!cpDbName[0]))
    {
        return -1;
    }

    char buffer[1024];
    sprintf(buffer, "drop database %s", cpDbName);
    return mysql_query(&m_mysql, buffer);
}

int BMysql:: Ping()
{
    if (!m_bConnected)
    {
        return -1;
    }
    return mysql_ping(&m_mysql);
}

int BMysql:: GetLastError()
{
    return mysql_errno(&m_mysql);
}

long long BMysql:: GetLastSqlEffectRows()
{
    if (!m_bConnected)
    {
        return -1;
    }
    return mysql_affected_rows(&m_mysql);
}

const char *BMysql::GetCharSetName()
{
    if (!m_bConnected)
    {
        return NULL;
    }
    return mysql_character_set_name(&m_mysql);
}

int BMysql::ConfigMysql(mysql_option typeOption, void *valueOption)
{
    if (!m_bConnected)
    {
        return -1;
    }
    return mysql_options(&m_mysql, typeOption, " ");
}

int BMysql::IsThreadSafe()
{
    return mysql_thread_safe();
}

/************************************************************************/
/* class BMysqlTable                                                    */
/************************************************************************/
int BMysqlTable::Query(const char *cpCond, OUT BDatabaseTable &dbTable)
{
    char cpTemp[1024];
    sprintf(cpTemp, "select * from %s where %s",
        m_strTableName.c_str(), cpCond);

    return BMysql::Query(cpTemp, dbTable);
}

int BMysqlTable::AddTuple(const char *cpData)
{
    char cpTemp[1024];
    sprintf(cpTemp, "insert into %s values(%s)", 
        m_strTableName.c_str(), cpData);

    return BMysql::AddTuple(cpTemp);
}

int BMysqlTable::DeleteTuple(const char *cpCond)
{
    char cpTemp[1024];
    sprintf(cpTemp, "delete from %s where %s", 
        m_strTableName.c_str(), cpCond);
    
    return BMysql::DeleteTuple(cpTemp);
}

BZ_DECLARE_NAMESPACE_END
