#pragma once
#include <NovusTypes.h>
#include <Utils/SharedPool.h>

#include <mutex>
#include <mysql.h>
#include "Field.h"


struct QueryResult
{
public:
    ~QueryResult()
    {
        Free();
    }
    bool GetNextRow()
    {
        if (_currentRowNum > _rowNum)
        {
            Free();
            return false;
        }

        row = mysql_fetch_row(_result);
        for (i32 i = 0; i < _fieldNum; i++)
        {
            MYSQL_FIELD* field = &_mysqlFields[i];

            /* Convert MYSQL_FIELD to Field */
            _fields[i] = Field(field, row[i]);
        }

        _currentRowNum++;
        return true;
    }
    u64 GetColumns() { return _fieldNum; }
    u64 GetAffectedRows() { return _rowNum; }
    Field const& GetField(size_t index)
    {
        assert(index < _fieldNum);
        return _fields[index];
    }
    void Free()
    {
        if (_result)
        {
            mysql_free_result(_result);
            _result = nullptr;
            _mysqlFields = nullptr;
            row = nullptr;
        }
    }

    static std::shared_ptr<QueryResult> Borrow(MYSQL_RES* result = nullptr, u64 inFieldNum = 0, u64 inRowNum = 0)
    {
        _borrowMutex.lock();

        if (_queryResults.empty())
        {
            _queryResults.add(std::unique_ptr<QueryResult>(new QueryResult()));
        }

        std::shared_ptr<QueryResult> queryResult = _queryResults.acquire();
        _borrowMutex.unlock();

        // Ensure we free the mysql resources IF we never finished reading all rows on previous acquire
        queryResult->Free();

        queryResult->_result = result;
        queryResult->_mysqlFields = result ? mysql_fetch_fields(result) : nullptr;
        queryResult->row = nullptr;
        queryResult->_fieldNum = inFieldNum;
        queryResult->_rowNum = inRowNum;
        queryResult->_currentRowNum = 1;

        return queryResult;
    }

private:
    QueryResult() : _result(nullptr), _mysqlFields(nullptr), row(nullptr), _fieldNum(0), _rowNum(0) { }
    QueryResult(MYSQL_RES* inResult, u64 inFieldNum, u64 inRowNum) : _mysqlFields(nullptr), row(nullptr)
    {
        _result = inResult;
        _fieldNum = inFieldNum;
        _rowNum = inRowNum;
    }

public:
private:
    MYSQL_RES* _result;
    MYSQL_FIELD* _mysqlFields;
    MYSQL_ROW row;
    u64 _fieldNum;
    u64 _rowNum;
    u64 _currentRowNum = 1;

    /*
        The reason for choosing 32 here as a static value is that
        "Field Num" can vary from query to query, we would be forced 
        to allocate memory each time we fetch a new query. The solution
        here assumes that our max amount of columns ever fetched will be
        32. I(Nix) Feel 32 is a big enough number of columns, if you have
        more columns and absolutely need the data at once, increase this value
    */
    Field _fields[32];

    static std::mutex _borrowMutex;
    static SharedPool<QueryResult> _queryResults;
};