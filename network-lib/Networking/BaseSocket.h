/*
# MIT License

# Copyright(c) 2018-2019 NovusCore

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/
#pragma once

#include "BaseSocket.h"
#include "Defines.h"

#include <asio.hpp>
#include <Utils/ByteBuffer.h>

class BaseSocket : public std::enable_shared_from_this<BaseSocket>
{
public:
    using tcp = asio::ip::tcp;  
    BaseSocket(tcp::socket* socket) : _isClosed(false), _socket(socket) { Init(); }
    ~BaseSocket() { }

    void Init()
    {
        _receiveBuffer = Bytebuffer::Borrow<NETWORK_BUFFER_SIZE>();
        _sendBuffer = Bytebuffer::Borrow<NETWORK_BUFFER_SIZE>();
    }

    std::shared_ptr<Bytebuffer> GetReceiveBuffer() { return _receiveBuffer; }
    void _internalRead(asio::error_code errorCode, size_t bytesRead)
    {
        if (errorCode)
        {
            Close(errorCode);
            return;
        }

        _receiveBuffer->writtenData += bytesRead;

        if (_readHandler)
            _readHandler(this);
    }
    void _internalWrite(asio::error_code errorCode, std::size_t bytesWritten)
    {
        if (errorCode)
        {
            Close(errorCode);
        }
    }
    void AsyncRead()
    {
        if (!_socket->is_open())
            return;

        _receiveBuffer->Reset();
        _socket->async_read_some(asio::buffer(_receiveBuffer->GetReadPointer(), _receiveBuffer->GetReadSpace()),
            std::bind(&BaseSocket::_internalRead, this, std::placeholders::_1, std::placeholders::_2));
    }
    void Send(std::shared_ptr<Bytebuffer>& buffer)
    {
        if (!buffer->IsEmpty() || buffer->IsFull())
        {
            _socket->async_write_some(asio::buffer(buffer->GetDataPointer(), buffer->writtenData),
                std::bind(&BaseSocket::_internalWrite, this, std::placeholders::_1, std::placeholders::_2));
        }
    }

    bool IsClosed() { return _isClosed || !_socket->is_open(); }
    void Close(asio::error_code error)
    {
        if (!_isClosed)
        {
            if (_disconnectHandler)
                _disconnectHandler(this);

            _socket->close();
            _isClosed = true;
        }
    }
    void _internalConnected(bool connected)
    {
        if (_connectHandler)
            _connectHandler(this, connected);
    }

    tcp::socket* socket()
    {
        return _socket;
    }
    void SetReadHandler(std::function<void(BaseSocket*)> readHandler)
    {
        _readHandler = readHandler;
    }
    void SetConnectHandler(std::function<void(BaseSocket*, bool)> connectHandler)
    {
        _connectHandler = connectHandler;
    }
    void SetDisconnectHandler(std::function<void(BaseSocket*)> disconnectHandler)
    {
        _disconnectHandler = disconnectHandler;
    }
private:
    std::shared_ptr<Bytebuffer> _receiveBuffer;
    std::shared_ptr<Bytebuffer> _sendBuffer;

    bool _isClosed = false;
    tcp::socket* _socket;
    std::function<void(BaseSocket*)> _readHandler;
    std::function<void(BaseSocket*, bool)> _connectHandler;
    std::function<void(BaseSocket*)> _disconnectHandler;
};