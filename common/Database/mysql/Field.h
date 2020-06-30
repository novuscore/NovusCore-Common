#pragma once
#include <NovusTypes.h>
#include <mysql.h>
#include <Utils/ByteBuffer.h>
#include <Utils/DebugHandler.h>
#include <sstream>

// Fields are temporary objects that exists within a QueryResult for a Row.
struct Field
{
public:
    Field() : _data(nullptr), _field(nullptr) { }
    Field(MYSQL_FIELD* field, char* data) : _field(field) 
    { 
        _data = data;
    }

    bool IsNull() const
    {
        return !_data;
    }
    bool IsBinary() const
    {
        return _field->type == MYSQL_TYPE_TIMESTAMP || _field->type == MYSQL_TYPE_DATE ||
            _field->type == MYSQL_TYPE_TIME || _field->type == MYSQL_TYPE_DATETIME ||
            _field->type == MYSQL_TYPE_TINY_BLOB || _field->type == MYSQL_TYPE_MEDIUM_BLOB ||
            _field->type == MYSQL_TYPE_LONG_BLOB || _field->type == MYSQL_TYPE_BLOB ||
            _field->type == MYSQL_TYPE_STRING || _field->type == MYSQL_TYPE_VAR_STRING;
    }

    i8 GetI8() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__ );
        }

        i8 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }
    u8 GetU8() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__ );
        }

        u8 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }
    i16 GetI16() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__ );
        }

        i16 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }    
    u16 GetU16() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__ );
        }

        u16 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }
    i32 GetI32() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__);
        }

        i32 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }    
    u32 GetU32() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__);
        }

        u32 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }
    f32 GetF32() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__);
        }

        f32 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }
    f64 GetF64() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__);
        }

        f64 value = 0;
        std::stringstream ss(_data);
        ss >> value;

        if (!ss)
        {
            NC_LOG_ERROR("SQL Field: Conversion failed for (Table: %s, Column: %s) during %s", _field->table, _field->name, __func__);
        }

        return value;
    }
    std::string GetString() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__);
        }

        if (!IsBinary())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on non-binary field", __func__);
        }

        return std::string(_data);
    }
    std::shared_ptr<Bytebuffer> GetBinary() const
    {
        if (IsNull())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on null field", __func__);
        }

        if (!IsBinary())
        {
            NC_LOG_FATAL("SQL Field: Called Field::%s on non-binary field", __func__);
        }

        /* 
            I (Nix) have chosen to go with 8192 as max value for Binary
            SQL values, as I don't see us storing bigger values, feel
            free to up this value if problems occur in the future
        */
        std::shared_ptr<Bytebuffer> buffer = Bytebuffer::Borrow<8192>();
        buffer->size = _field->length;
        memcpy(buffer->GetDataPointer(), _data, buffer->size);

        return buffer;
    }

private:
    char* _data;
    MYSQL_FIELD* _field;
};