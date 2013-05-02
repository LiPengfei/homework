#include "Database/DatabaseTable.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

//BDatabaseColumn
BDatabaseColumn::BDatabaseColumn() { }

BDatabaseColumn::BDatabaseColumn(BZ_DB_VALUE_TYPE type, 
                                 const BSimpleString &kszName)
                                 : m_emType(type), m_bssName(kszName)
{ }

BDatabaseColumn::~BDatabaseColumn() { }

void BDatabaseColumn::Clean()
{
    m_emType = BZ_DB_NULL;
    m_bssName.Clean();
}

const char *BDatabaseColumn::Data2NewChar() const
{
    if (0 == m_bssName.m_nLength)
    {
        return NULL;
    }

    char *pc = new char[m_bssName.m_nLength];
    memcpy(pc, m_bssName.m_cpStr, m_bssName.m_nLength);
    return pc;
}

//BDatabaseTuple
BDatabaseTuple::BDatabaseTuple() 
    : m_nTupleIndex(0),
    m_nCellCount(0),
    m_aCells(NULL)
{ }

BDatabaseTuple::BDatabaseTuple(size_t nIndex, 
                               size_t nCellCount, BDatabaseCell *const pcSStr)
                               : m_nTupleIndex(nIndex), m_nCellCount(nCellCount), 
                               m_aCells(pcSStr)
{ }


BDatabaseTuple::BDatabaseTuple(const BDatabaseTuple &rhs)
    : m_nTupleIndex(rhs.m_nTupleIndex), 
    m_nCellCount(rhs.m_nCellCount),
    m_aCells(NULL)
{
    if (0 == m_nCellCount)
    {
        return;
    }

    m_aCells = new BDatabaseCell[m_nCellCount];
    for (size_t i = 0; i != m_nCellCount; ++i)
    {
        m_aCells[i] = rhs.m_aCells[i];
    }
}

BDatabaseTuple& BDatabaseTuple::operator=(const BDatabaseTuple &rhs)
{
    if (&rhs == this)
    {
        return *this;
    }

    m_nTupleIndex = rhs.m_nTupleIndex;
    m_nCellCount  = rhs.m_nCellCount;

    delete []m_aCells;
    if (0 == rhs.m_nCellCount)
    {
        m_aCells = NULL;
        return *this;
    }

    m_aCells = new BDatabaseCell[rhs.m_nCellCount];
    for (size_t i = 0; i != rhs.m_nCellCount; ++i)
    {
        m_aCells[i] = rhs.m_aCells[i];
    }
    return *this;
}

BDatabaseTuple::~BDatabaseTuple()
{
    Clean();
}

void BDatabaseTuple::Clean()
{
    BZ_SafelyDeleteArrayPtr(m_aCells);

    m_nTupleIndex = 0;
    m_aCells      = NULL;
    m_nCellCount  = 0;
}

//KDatabaseTable
BDatabaseTable::BDatabaseTable() : m_columns(NULL), 
    m_tuples(NULL), m_nColCount(0), m_nTupleCount(0)
{ }

BDatabaseTable::~BDatabaseTable()
{
    Clean();
}

BDatabaseTable::BDatabaseTable(const BDatabaseTable &rhs)
    : m_nColCount(rhs.m_nColCount), m_nTupleCount(rhs.m_nTupleCount),
    m_columns(NULL), m_tuples(NULL)
{
    m_columns = new BDatabaseColumn[rhs.m_nColCount];

    if ((!rhs.m_columns) || (!rhs.m_columns))
    {
        return ;
    }

    for(size_t i = 0; i != m_nColCount; ++i)
    {
        m_columns[i] = rhs.m_columns[i];
    }

    m_tuples = new BDatabaseTuple[rhs.m_nTupleCount];
    for(size_t i = 0; i != m_nTupleCount; ++i)
    {
        m_tuples[i] = rhs.m_tuples[i];
    }
}

BDatabaseTable& BDatabaseTable::operator=(const BDatabaseTable &rhs)
{
    if (&rhs == this)
    {
        return *this;
    }

    m_nColCount   = rhs.m_nColCount;
    m_nTupleCount = rhs.m_nTupleCount;

    delete[]m_tuples;
    delete[]m_columns;

    if ((!m_columns) || (!m_tuples))
    {
        m_tuples  = NULL;
        m_columns = NULL;
    }

    m_columns = new BDatabaseColumn[rhs.m_nColCount];
    for(size_t i = 0; i != m_nColCount; ++i)
    {
        m_columns[i] = rhs.m_columns[i];
    }

    m_tuples = new BDatabaseTuple[rhs.m_nTupleCount];
    for (size_t i = 0; i != m_nTupleCount; ++i)
    {
        m_tuples[i] = rhs.m_tuples[i];
    }

    return *this;
}

int BDatabaseTable::GetColIndexFromName(const char *pcName) const
{
    BZ_CHECK_C_STRING_RETURN_CODE(pcName, -1);

    size_t iTemp = 0;
    for (; iTemp != m_nColCount 
        && BSimpleString(pcName) != m_columns[iTemp].m_bssName; ++iTemp)
    { }

    return (m_nColCount == iTemp) ? -1 : iTemp;
}

int BDatabaseTable::GetColNameFromIndex(size_t nIndex, OUT BSimpleString &bssName) const
{
    BZ_CHECK_RETURN_CODE(nIndex < m_nColCount, -1);

    bssName = m_columns[nIndex].m_bssName;
    return 0;
}

int BDatabaseTable::GetTuple(size_t nTupleIndex, OUT BDatabaseTuple &tuple) const
{
    BZ_CHECK_RETURN_CODE(nTupleIndex < m_nTupleCount, -1);

    tuple = m_tuples[nTupleIndex];
    return 0;
}

int BDatabaseTable::GetValue(size_t nTupleIndex, size_t nColumnIndex, 
                             OUT BDatabaseCell &value) const
{
    value.Clean();

    BZ_CHECK_RETURN_CODE((nTupleIndex < m_nTupleCount)
        && (nColumnIndex < m_nColCount), -1);
    
    value = m_tuples[nTupleIndex].m_aCells[nColumnIndex];
    return 0;
}

int BDatabaseTable::GetValue(size_t nTupleIndex, const char *cpColumnName, 
                             OUT BDatabaseCell &value) const
{
    int iTemp = GetColIndexFromName(cpColumnName);
    if (-1 == iTemp)
    {
        return -1;
    }

    return GetValue(nTupleIndex, iTemp, value);
}

void BDatabaseTable::Clean()
{
    BZ_SafelyDeleteArrayPtr(m_columns);

    if (m_tuples)
    {
        delete[] m_tuples;
    }

    m_columns     = NULL;
    m_tuples      = NULL;
    m_nTupleCount = 0;
    m_nColCount   = 0;
}

BZ_DECLARE_NAMESPACE_END