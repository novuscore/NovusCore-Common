#pragma once
#include <Utils/ConcurrentQueue.h>
#include <Utils/Message.h>

class InputQueue
{
public:
    InputQueue() : queue(256) { }
    moodycamel::ConcurrentQueue<Message> queue;
};