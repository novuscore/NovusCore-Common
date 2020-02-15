#pragma once
#include <NovusTypes.h>
#include "mysql/Mysql.h"
#include "mysql/DatabaseWorker.h"

#define AsyncQueueSize 256
struct DatabaseWorkerPair
{
    std::shared_ptr<Mysql> connector;
    std::shared_ptr<DatabaseWorker> worker;
};

class DBConnection
{
public:
    DBConnection(i32 workers = 1) : _asyncQueryQueue(AsyncQueueSize), _asyncExecuteQueue(AsyncQueueSize), _isConnected(false)
    {
        _connector = CreateMysql();
        SetupWorkers(workers);
    }
    DBConnection(std::string address, u16 port, std::string username, std::string password, std::string database, u32 flags = 0, i32 workers = 1) : _asyncQueryQueue(AsyncQueueSize), _asyncExecuteQueue(AsyncQueueSize), _isConnected(false)
    {
        _connector = CreateMysql();
        SetupWorkers(workers);
        Connect(address, port, username, password, database, flags);
    }

    bool Connect(std::string address, u16 port, std::string username, std::string password, std::string database, u32 flags = 0);
    void Close();

    bool Execute(const std::string stmt);
    std::shared_ptr<QueryResult> Query(const std::string stmt);
    std::shared_ptr<QueryResult> RealQuery(const char* stmt, i32 length);

    // Returns if the job was successfully queued
    bool QueryAsync(char* stmt, AsyncQueryCallback handler = nullptr);
    // Returns if the job was successfully queued
    bool QueryAsync(AsyncQueryJob& job);

    // Returns if the job was successfully queued
    bool ExecuteAsync(char* stmt, AsyncExecuteCallback handler = nullptr);
    // Returns if the job was successfully queued
    bool ExecuteAsync(AsyncExecuteJob& job);

    // Rough guess at if the connection is open
    bool IsOpen()
    {
        return _isConnected;
    }
private:
    std::shared_ptr<Mysql> CreateMysql();
    void SetupWorkers(i32 workers);

public:
private:
    moodycamel::ConcurrentQueue<AsyncQueryJob> _asyncQueryQueue;
    moodycamel::ConcurrentQueue<AsyncExecuteJob> _asyncExecuteQueue;
    std::vector<DatabaseWorkerPair> _workers;
    std::shared_ptr<Mysql> _connector;
    bool _isConnected;
};
