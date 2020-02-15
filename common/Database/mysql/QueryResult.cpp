#include "QueryResult.h"

std::mutex QueryResult::_borrowMutex;
SharedPool<QueryResult> QueryResult::_queryResults;