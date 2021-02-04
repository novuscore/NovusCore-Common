#include "Mysql.h"
#include <Utils/DebugHandler.h>

bool Mysql::_mysqlInitialized = false;
bool Mysql::Initialize(std::string charset /* = "utf8" */)
{
    if (IsInitialized())
        return false;

    if (!_mysqlInitialized)
    {    
        // Require MYSQL 5.6 or higher
        if (MYSQL_VERSION_ID < 50600)
        {
            DebugHandler::PrintFatal("MYSQL_VERSION_ID(%i) needs to be at least 50600 or higher", MYSQL_VERSION_ID);
        }

        if (mysql_library_init(0, nullptr, nullptr))
        {
            DebugHandler::PrintFatal("Failed to initialize MySQL client");
        }

        _mysqlInitialized = true;
    }

    // mysql_init is not thread_safe on first call unless we manually invoke (mysql_library_init())
    _mysql = mysql_init(nullptr);
    if (!_mysql)
        return false;

    _charset = charset;
    _mysql->reconnect = 1;
    mysql_options(_mysql, MYSQL_SET_CHARSET_NAME, _charset.c_str());
    return true;
}

bool Mysql::Connect(std::string address, u16 port, std::string username, std::string password, std::string database, u32 flags /* = 0 */)
{
    if (!IsInitialized())
        return false;

    MYSQL* conn = mysql_real_connect(_mysql, address.c_str(), username.c_str(), password.c_str(), database.c_str(), port, nullptr, 0);
    if (!conn)
        return false;

    mysql_autocommit(_mysql, 1);
    mysql_set_character_set(_mysql, _charset.c_str());
    return true;
}

bool Mysql::RawQuery(const std::string stmt)
{
    if (mysql_query(_mysql, stmt.c_str()))
    {
         DebugHandler::PrintError("SQL(%u): %s", mysql_errno(_mysql), mysql_error(_mysql));
        return false;
    }

    return true;
}
bool Mysql::RawRealQuery(const char* stmt, i32 length)
{
    if (mysql_real_query(_mysql, stmt, length))
    {
         DebugHandler::PrintError("SQL(%u): %s", mysql_errno(_mysql), mysql_error(_mysql));
        return false;
    }

    return true;
}
bool Mysql::Execute(const std::string stmt)
{
    return RawQuery(stmt);
}

std::shared_ptr<QueryResult> Mysql::Query(const std::string stmt)
{
    std::shared_ptr<QueryResult> queryResult = nullptr;

    if (RawQuery(stmt))
    {
        MYSQL_RES* result = mysql_store_result(_mysql);
        u64 rowNum = mysql_affected_rows(_mysql);
        if (result)
        {
            if (!rowNum)
            {
                mysql_free_result(result);
            }
        }

        queryResult = QueryResult::Borrow(result, mysql_field_count(_mysql), rowNum);
    }

    return queryResult;
}
std::shared_ptr<QueryResult> Mysql::RealQuery(const char* stmt, i32 length)
{
    std::shared_ptr<QueryResult> queryResult = nullptr;

    if (RawRealQuery(stmt, length))
    {
        MYSQL_RES* result = mysql_store_result(_mysql);
        if (result)
        {
            u64 rowNum = mysql_affected_rows(_mysql);
            if (!rowNum)
            {
                mysql_free_result(result);
            }
            else
            {
                queryResult = QueryResult::Borrow(result, mysql_field_count(_mysql), rowNum);
            }
        }
    }

    return queryResult;
}
