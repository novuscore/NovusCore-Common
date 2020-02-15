#pragma once
#include <NovusTypes.h>
#include <mysql.h>
#include <memory>
#include "QueryResult.h"

class Mysql
{
public:
    Mysql() : _mysql(nullptr), _charset() { }
    ~Mysql() { Close(); }

    bool Initialize(std::string charSet = "utf8");
    bool IsInitialized() { return _mysql != nullptr; }
    bool Connect(std::string address, u16 port, std::string username, std::string password, std::string database, u32 flags = 0);
    void Close()
    {
        if (_mysql)
        {
            mysql_close(_mysql);
            mysql_library_end();
            _mysql = nullptr;
        }
    }

    bool RawQuery(const std::string stmt);
    bool RawRealQuery(const char* stmt, i32 length);

    bool Execute(const std::string stmt);
    std::shared_ptr<QueryResult> Query(const std::string stmt);
    std::shared_ptr<QueryResult> RealQuery(const char* stmt, i32 length);

public:
private:
    MYSQL* _mysql;
    std::string _charset;
    static bool _mysqlInitialized;
};