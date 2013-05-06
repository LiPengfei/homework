#ifndef __BLAZER_SABRE_DATABASETABLE_H__
#define __BLAZER_SABRE_DATABASETABLE_H__

#include "Public.h"
#include "Structure/SimpleString.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

enum BZ_DB_VALUE_TYPE
{
    BZ_DB_NUMBER,
    BZ_DB_CHARACTERS,
    BZ_DB_BYTES,
    BZ_DB_BOOLEAN,
    BZ_DB_TIME,
    BZ_DB_DATE,
    BZ_DB_DATETIME,
    BZ_DB_TIMESTAMP,
    BZ_DB_NULL,
    BZ_DB_UNKNOWN
};

class BDatabaseColumn
{
    friend class BDatabaseTable;
    friend class BMysql;
	friend class BMysqlTable;

private:

    BZ_DB_VALUE_TYPE m_emType;
    BSimpleString    m_bssName;

public:
    BDatabaseColumn();
    BDatabaseColumn(BZ_DB_VALUE_TYPE type, const BSimpleString &bssName);
    virtual ~BDatabaseColumn();

public:
    void Clean();
    const char *Data2NewChar() const;

    BZ_DB_VALUE_TYPE GetType() const 
    {
        return m_emType;
    }
};

typedef BDatabaseColumn BDatabaseCell;

class BDatabaseTuple
{
    friend class BDatabaseTable;
    friend class BMysql;
	friend class BMysqlTable;

private:
    size_t		   m_nTupleIndex;
    size_t		   m_nCellCount;
    BDatabaseCell *m_aCells;

public:
    BDatabaseTuple();
    BDatabaseTuple(size_t nIndex, size_t nCellCount, BDatabaseCell *const cells);
    virtual ~BDatabaseTuple();

public:
    BDatabaseTuple(const BDatabaseTuple &rhs);
    BDatabaseTuple &operator=(const BDatabaseTuple &rhs);

    void Clean();
};

class BDatabaseTable
{
    friend class BMysql;
	friend class BMysqlTable;

private:
    size_t           m_nColCount;
    size_t           m_nTupleCount;
    BDatabaseColumn *m_columns;
    BDatabaseTuple  *m_tuples;

public:
    BDatabaseTable();
    virtual ~BDatabaseTable();

public:
    BDatabaseTable(const BDatabaseTable &rhs);
    BDatabaseTable &operator=(const BDatabaseTable &rhs);

public:

    int GetTuple(size_t nTupleIndex, 
        OUT BDatabaseTuple &tuple) const;

    int GetValue(size_t nTupleIndex, size_t nColumnIndex, 
        OUT BDatabaseCell &value) const;

    int GetValue(size_t nTupleIndex, const char *cpColumnName, 
        OUT BDatabaseCell &value) const;

    void Clean();

protected:
    int GetColIndexFromName(const char *cpName) const;
    int GetColNameFromIndex(size_t nIndex, OUT BSimpleString &bssName) const;
};

BZ_DECLARE_NAMESPACE_END
#endif