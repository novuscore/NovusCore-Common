#pragma once
#include <NovusTypes.h>
#include <Utils/ByteBuffer.h>
#include <entity/fwd.hpp>
#include "Opcode.h"
#include "AddressType.h"

namespace PacketUtils
{
    inline bool Write_MSG_REQUEST_ADDRESS(std::shared_ptr<Bytebuffer>& buffer, AddressType type, entt::entity entity, u8* extraData = nullptr, size_t extraDataSize = 0)
    {
        if (!buffer->Put(Opcode::MSG_REQUEST_ADDRESS))
            return false;

        size_t currentWriteIndex = buffer->writtenData;
        if (!buffer->SkipWrite(sizeof(u16)))
            return false;

        if (!buffer->Put(type))
            return false;

        if (!buffer->PutEnttId(entity))
            return false;

        if (extraData != nullptr && extraDataSize > 0)
        {
            if (!buffer->PutBytes(extraData, extraDataSize))
                return false;
        }

        size_t packetSize = (buffer->writtenData - currentWriteIndex) - 2;
        if (!buffer->Put<u16>(static_cast<u16>(packetSize), currentWriteIndex))
            return false;

        return true;
    }
    inline bool Write_SMSG_SEND_ADDRESS(std::shared_ptr<Bytebuffer>& buffer, u8 status, u32 address = 0, u16 port = 0, u8* extraData = nullptr, size_t extraDataSize = 0)
    {
        if (!buffer->Put(Opcode::SMSG_SEND_ADDRESS))
            return false;

        size_t currentWriteIndex = buffer->writtenData;
        if (!buffer->SkipWrite(sizeof(u16)))
            return false;

        if (!buffer->PutU8(status))
            return false;

        if (status > 0)
        {
            if (!buffer->PutU32(address))
                return false;

            if (!buffer->PutU16(port))
                return false;
        }

        if (extraData != nullptr && extraDataSize > 0)
        {
            if (!buffer->PutBytes(extraData, extraDataSize))
                return false;
        }

        size_t packetSize = (buffer->writtenData - currentWriteIndex) - 2;
        if (!buffer->Put<u16>(static_cast<u16>(packetSize), currentWriteIndex))
            return false;

        return true;
    }
    inline bool Write_SMSG_SEND_FULL_INTERNAL_SERVER_INFO(std::shared_ptr<Bytebuffer>& buffer, const u8* serverInfoData, size_t numServers)
    {
        if (!buffer->Put(Opcode::SMSG_SEND_FULL_INTERNAL_SERVER_INFO))
            return false;

        size_t packetSize = (sizeof(entt::entity) + sizeof(AddressType) + sizeof(u32) + sizeof(u16)) * numServers;
        if (!buffer->PutU16(static_cast<u16>(packetSize)))
            return false;
        
        /*
            The data here is packed as following inside of a struct called "ServerInformation", however
            the struct is not located in common, thus we cannot reference it here.

            It is not in "common" because only 2 servers needs to know the structure.

            Entt::Entity, AddressType, u32, u16
        */

        if (!buffer->PutBytes(serverInfoData, packetSize))
            return false;

        return true;
    }
    inline bool Write_SMSG_SEND_ADD_INTERNAL_SERVER_INFO(std::shared_ptr<Bytebuffer>& buffer, entt::entity entity, AddressType type, u32 address, u16 port)
    {
        if (!buffer->Put(Opcode::SMSG_SEND_ADD_INTERNAL_SERVER_INFO))
            return false;

        if (!buffer->PutU16(sizeof(entt::entity) + sizeof(AddressType) + sizeof(u32) + sizeof(u16)))
            return false;
        
        if (!buffer->PutEnttId(entity))
            return false;

        if (!buffer->Put(type))
            return false;

        if (!buffer->PutU32(address))
            return false;

        if (!buffer->PutU16(port))
            return false;

        return true;
    }
    inline bool Write_SMSG_SEND_REMOVE_INTERNAL_SERVER_INFO(std::shared_ptr<Bytebuffer>& buffer, entt::entity entity, AddressType type)
    {
        if (!buffer->Put(Opcode::SMSG_SEND_REMOVE_INTERNAL_SERVER_INFO))
            return false;

        if (!buffer->PutU16(sizeof(entt::entity) + sizeof(AddressType)))
            return false;
        
        if (!buffer->PutEnttId(entity))
            return false;

        if (!buffer->Put(type))
            return false;

        return true;
    }
}