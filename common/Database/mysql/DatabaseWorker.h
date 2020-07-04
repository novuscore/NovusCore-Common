#pragma once
#include <NovusTypes.h>
#include <Utils/ConcurrentQueue.h>
#include <functional>
#include "Mysql.h"
#include <iostream>

typedef std::function<void(std::shared_ptr<QueryResult>)> AsyncQueryCallback;
typedef std::function<void()> AsyncExecuteCallback;
struct AsyncQueryJob
{
    char* stmt;
    AsyncQueryCallback handler;
}; 
struct AsyncExecuteJob
{
    char* stmt;
    AsyncExecuteCallback handler;
};

class DatabaseWorker
{
public:
    DatabaseWorker(std::shared_ptr<Mysql> connector, moodycamel::ConcurrentQueue<AsyncQueryJob>* queryQueue, moodycamel::ConcurrentQueue<AsyncExecuteJob>* executeQueue) : _asyncQueryQueue(queryQueue), _asyncExecuteQueue(executeQueue)
    {
        _running = true;
        _connector = std::move(connector);
        _thread = std::thread(&DatabaseWorker::Run, this);

        _thread.detach();
    }

    void Close()
    {
        _running = false;
        _thread.join();
        _connector->Close();
    }

    void Run()
    {
        while (_running)
        {
            AsyncQueryJob queryJob;
            while (_asyncQueryQueue->try_dequeue(queryJob))
            {
                std::shared_ptr<QueryResult> result = _connector->Query(queryJob.stmt);
                if (queryJob.handler)
                {
                    queryJob.handler(result);
                }
            }

            AsyncExecuteJob executeJob;
            while (_asyncExecuteQueue->try_dequeue(executeJob))
            {
                _connector->Execute(executeJob.stmt);
                if (executeJob.handler)
                {
                    executeJob.handler();
                }
            }

            Sleep(1);
        }
    }

private:
    bool _running;
    std::thread _thread;
    std::shared_ptr<Mysql> _connector;
    moodycamel::ConcurrentQueue<AsyncQueryJob>* _asyncQueryQueue;
    moodycamel::ConcurrentQueue<AsyncExecuteJob>* _asyncExecuteQueue;
};