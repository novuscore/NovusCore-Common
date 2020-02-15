#include "DBConnection.h"

bool DBConnection::Connect(std::string address, u16 port, std::string username, std::string password, std::string database, u32 flags /* = 0 */)
{
    if (_isConnected)
        return false;

    _isConnected = _connector->Connect(address, port, username, password, database, flags);

    if (_isConnected)
    {
        for (DatabaseWorkerPair& pair : _workers)
        {
            pair.connector->Connect(address, port, username, password, database, flags);
        }
    }

    return _isConnected;
}
void DBConnection::Close()
{
    _isConnected = false;
    _connector->Close();

    for (DatabaseWorkerPair& pair : _workers)
    {
        pair.connector->Close();
    }
}

bool DBConnection::Execute(const std::string stmt)
{
    return _connector->Execute(stmt);
}
std::shared_ptr<QueryResult> DBConnection::Query(const std::string stmt)
{
    return _connector->Query(stmt);
}
std::shared_ptr<QueryResult> DBConnection::RealQuery(const char* stmt, i32 length)
{
    return _connector->RealQuery(stmt, length);
}

// Returns if the job was successfully queued
bool DBConnection::QueryAsync(char* stmt, AsyncQueryCallback handler /* = nullptr */)
{
    AsyncQueryJob job;
    job.stmt = stmt;
    job.handler = handler;

    return QueryAsync(job);
}
// Returns if the job was successfully queued
bool DBConnection::QueryAsync(AsyncQueryJob& job)
{
    return _asyncQueryQueue.enqueue(job);
}

// Returns if the job was successfully queued
bool DBConnection::ExecuteAsync(char* stmt, AsyncExecuteCallback handler /* = nullptr */)
{
    AsyncExecuteJob job;
    job.stmt = stmt;
    job.handler = handler;

    return ExecuteAsync(job);
}
// Returns if the job was successfully queued
bool DBConnection::ExecuteAsync(AsyncExecuteJob& job)
{
    return _asyncExecuteQueue.enqueue(job);
}

std::shared_ptr<Mysql> DBConnection::CreateMysql()
{
    std::shared_ptr<Mysql> connector = std::make_shared<Mysql>();
    connector->Initialize();

    return connector;
}
void DBConnection::SetupWorkers(i32 workers)
{
    _workers.reserve(workers);
    for (i32 i = 0; i < workers; i++)
    {
        DatabaseWorkerPair databaseWorkerPair;
        databaseWorkerPair.connector = CreateMysql();
        databaseWorkerPair.worker = std::make_shared<DatabaseWorker>(databaseWorkerPair.connector, &_asyncQueryQueue, &_asyncExecuteQueue);

        _workers.push_back(databaseWorkerPair);
    }
}