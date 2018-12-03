// <ORACLIZE_API>
/*
Copyright (c) 2015-2016 Oraclize SRL
Copyright (c) 2016 Oraclize LTD
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#if __cplusplus < 201103
#warning "To enable all features you must compile with -std=c++11"
#endif

#include <map>
#include <stdint.h>
#include <vector>

#if __cplusplus >= 201103
#include <initializer_list>
#endif

#include <stdio.h>
#include <functional>

#include <cmath>
#include <cstdio>
#include <sstream>

#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>

using namespace eosio;
using namespace std;

/**************************************************
 *                      CBOR                      *
 *                Ends at line ~1100              *
 **************************************************/
class cbor
{
  public:
    enum type_t
    {
        TYPE_UNSIGNED,
        TYPE_NEGATIVE,
        TYPE_BINARY,
        TYPE_STRING,
        TYPE_ARRAY,
        TYPE_MAP,
        TYPE_TAGGED,
        TYPE_SIMPLE,
        TYPE_FLOAT
    };
    typedef std::vector<unsigned char> binary;
    typedef std::string string;
    typedef std::vector<cbor> array;
    typedef std::map<cbor, cbor> map;
    enum simple
    {
        SIMPLE_FALSE = 20,
        SIMPLE_TRUE,
        SIMPLE_NULL,
        SIMPLE_UNDEFINED,
        null = SIMPLE_NULL,
        undefined = SIMPLE_UNDEFINED
    };

    cbor(unsigned short value);
    cbor(unsigned value);
    cbor(unsigned long value);
    cbor(unsigned long long value);
    cbor(short value);
    cbor(int value);
    cbor(long value);
    cbor(long long value);
    cbor(const cbor::binary &value);
    cbor(const cbor::string &value);
    cbor(const char *value);
    cbor(const cbor::array &value);
    cbor(const cbor::map &value);
    static cbor tagged(unsigned long long tag, const cbor &value);
    cbor(cbor::simple value = cbor::SIMPLE_UNDEFINED);
    cbor(bool value);
    cbor(float value);
    cbor(double value);
#if __cplusplus >= 201103
    cbor(std::nullptr_t);
#endif

    bool is_unsigned() const;
    bool is_signed() const;
    bool is_int() const;
    bool is_binary() const;
    bool is_string() const;
    bool is_array() const;
    bool is_map() const;
    bool is_tagged() const;
    bool is_simple() const;
    bool is_bool() const;
    bool is_null() const;
    bool is_undefined() const;
    bool is_float() const;
    bool is_number() const;

    uint64_t to_unsigned() const;
    int64_t to_signed() const;
    cbor::binary to_binary() const;
    cbor::string to_string() const;
    cbor::array to_array() const;
    cbor::map to_map() const;
    cbor::simple to_simple() const;
    bool to_bool() const;
    double to_float() const;

    operator unsigned short() const;
    operator unsigned() const;
    operator unsigned long() const;
    operator unsigned long long() const;
    operator short() const;
    operator int() const;
    operator long() const;
    operator long long() const;
    operator cbor::binary() const;
    operator cbor::string() const;
    operator cbor::array() const;
    operator cbor::map() const;
    operator cbor::simple() const;
    operator bool() const;
    operator double() const;

    uint64_t tag() const;
    cbor child() const;

    cbor::type_t type() const;

    bool read(std::istream &in);
    void write(std::ostream &out) const;

    static bool validate(const cbor::binary &in);
    static cbor decode(const cbor::binary &in);
    static cbor::binary encode(const cbor &in);
    static cbor::string debug(const cbor &in);

    bool operator==(const cbor &other) const;
    bool operator!=(const cbor &other) const;

    bool operator<(const cbor &other) const;

  private:
    cbor::type_t m_type;
    union {
        uint64_t m_value;
        double m_float;
    };
    cbor::binary m_binary;
    cbor::string m_string;
    cbor::array m_array;
    cbor::map m_map;
};

cbor::cbor(unsigned short value) : m_type(cbor::TYPE_UNSIGNED), m_value(value)
{
}
cbor::cbor(unsigned value) : m_type(cbor::TYPE_UNSIGNED), m_value(value)
{
}
cbor::cbor(unsigned long value) : m_type(cbor::TYPE_UNSIGNED), m_value(value)
{
}
cbor::cbor(unsigned long long value) : m_type(cbor::TYPE_UNSIGNED), m_value(value)
{
}
cbor::cbor(short value) : m_type(value < 0 ? cbor::TYPE_NEGATIVE : cbor::TYPE_UNSIGNED), m_value(value < 0 ? -1 - value : value)
{
}
cbor::cbor(int value) : m_type(value < 0 ? cbor::TYPE_NEGATIVE : cbor::TYPE_UNSIGNED), m_value(value < 0 ? -1 - value : value)
{
}
cbor::cbor(long value) : m_type(value < 0 ? cbor::TYPE_NEGATIVE : cbor::TYPE_UNSIGNED), m_value(value < 0 ? -1 - value : value)
{
}
cbor::cbor(long long value) : m_type(value < 0 ? cbor::TYPE_NEGATIVE : cbor::TYPE_UNSIGNED), m_value(value < 0 ? -1 - value : value)
{
}
cbor::cbor(const cbor::binary &value) : m_type(cbor::TYPE_BINARY), m_binary(value)
{
}
cbor::cbor(const cbor::string &value) : m_type(cbor::TYPE_STRING), m_string(value)
{
}
cbor::cbor(const char *value) : m_type(cbor::TYPE_STRING), m_string(value)
{
}
cbor::cbor(const cbor::array &value) : m_type(cbor::TYPE_ARRAY), m_array(value)
{
}
cbor::cbor(const cbor::map &value) : m_type(cbor::TYPE_MAP), m_map(value)
{
}
cbor cbor::tagged(unsigned long long tag, const cbor &value)
{
    cbor result;
    result.m_type = cbor::TYPE_TAGGED;
    result.m_value = tag;
    result.m_array = cbor::array(1, value);
    return result;
}
cbor::cbor(cbor::simple value) : m_type(cbor::TYPE_SIMPLE), m_value(value & 255)
{
}
cbor::cbor(bool value) : m_type(cbor::TYPE_SIMPLE), m_value(value ? cbor::SIMPLE_TRUE : cbor::SIMPLE_FALSE)
{
}
cbor::cbor(float value) : m_type(cbor::TYPE_FLOAT), m_float(value)
{
}
cbor::cbor(double value) : m_type(cbor::TYPE_FLOAT), m_float(value)
{
}
#if __cplusplus >= 201103
cbor::cbor(std::nullptr_t) : m_type(cbor::TYPE_SIMPLE), m_value(cbor::SIMPLE_NULL)
{
}
#endif
bool cbor::is_unsigned() const
{
    return this->m_type == cbor::TYPE_UNSIGNED;
}
bool cbor::is_signed() const
{
    return (this->m_type == cbor::TYPE_UNSIGNED || this->m_type == cbor::TYPE_NEGATIVE) && (this->m_value >> 63) == 0;
}
bool cbor::is_int() const
{
    return this->m_type == cbor::TYPE_UNSIGNED || this->m_type == cbor::TYPE_NEGATIVE;
}
bool cbor::is_binary() const
{
    return this->m_type == cbor::TYPE_BINARY;
}
bool cbor::is_string() const
{
    return this->m_type == cbor::TYPE_STRING;
}
bool cbor::is_array() const
{
    return this->m_type == cbor::TYPE_ARRAY;
}
bool cbor::is_map() const
{
    return this->m_type == cbor::TYPE_MAP;
}
bool cbor::is_tagged() const
{
    return this->m_type == cbor::TYPE_TAGGED;
}
bool cbor::is_simple() const
{
    return this->m_type == cbor::TYPE_SIMPLE;
}
bool cbor::is_bool() const
{
    return this->m_type == cbor::TYPE_SIMPLE && (this->m_value == cbor::SIMPLE_FALSE || this->m_value == cbor::SIMPLE_TRUE);
}
bool cbor::is_null() const
{
    return this->m_type == cbor::TYPE_SIMPLE && this->m_value == cbor::SIMPLE_NULL;
}
bool cbor::is_undefined() const
{
    return this->m_type == cbor::TYPE_SIMPLE && this->m_value == cbor::SIMPLE_UNDEFINED;
}
bool cbor::is_float() const
{
    return this->m_type == cbor::TYPE_FLOAT;
}
bool cbor::is_number() const
{
    return this->m_type == cbor::TYPE_UNSIGNED || this->m_type == cbor::TYPE_NEGATIVE || this->m_type == cbor::TYPE_FLOAT;
}
uint64_t cbor::to_unsigned() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_UNSIGNED:
        return this->m_value;
    case cbor::TYPE_NEGATIVE:
        return ~this->m_value;
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_unsigned();
    case cbor::TYPE_FLOAT:
        return this->m_float;
    default:
        return 0;
    }
}
int64_t cbor::to_signed() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_UNSIGNED:
        return int64_t(this->m_value);
    case cbor::TYPE_NEGATIVE:
        return -1 - int64_t(this->m_value);
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_signed();
    case cbor::TYPE_FLOAT:
        return this->m_float;
    default:
        return 0;
    }
}
cbor::binary cbor::to_binary() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_BINARY:
        return this->m_binary;
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_binary();
    default:
        return cbor::binary();
    }
}
cbor::string cbor::to_string() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_STRING:
        return this->m_string;
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_string();
    default:
        return cbor::string();
    }
}
cbor::array cbor::to_array() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_ARRAY:
        return this->m_array;
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_array();
    default:
        return cbor::array();
    }
}
cbor::map cbor::to_map() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_MAP:
        return this->m_map;
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_map();
    default:
        return cbor::map();
    }
}
cbor::simple cbor::to_simple() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_simple();
    case cbor::TYPE_SIMPLE:
        return cbor::simple(this->m_value);
    default:
        return cbor::SIMPLE_UNDEFINED;
    }
}
bool cbor::to_bool() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_bool();
    case cbor::TYPE_SIMPLE:
        return this->m_value == cbor::SIMPLE_TRUE;
    default:
        return false;
    }
}
double cbor::to_float() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_UNSIGNED:
        return double(this->m_value);
    case cbor::TYPE_NEGATIVE:
        return ldexp(-1 - int64_t(this->m_value >> 32), 32) + (-1 - int64_t(this->m_value << 32 >> 32));
    case cbor::TYPE_TAGGED:
        return this->m_array.front().to_float();
    case cbor::TYPE_FLOAT:
        return this->m_float;
    default:
        return 0.0;
    }
}
cbor::operator unsigned short() const
{
    return this->to_unsigned();
}
cbor::operator unsigned() const
{
    return this->to_unsigned();
}
cbor::operator unsigned long() const
{
    return this->to_unsigned();
}
cbor::operator unsigned long long() const
{
    return this->to_unsigned();
}
cbor::operator short() const
{
    return this->to_signed();
}
cbor::operator int() const
{
    return this->to_signed();
}
cbor::operator long() const
{
    return this->to_signed();
}
cbor::operator long long() const
{
    return this->to_signed();
}
cbor::operator cbor::binary() const
{
    return this->to_binary();
}
cbor::operator cbor::string() const
{
    return this->to_string();
}
cbor::operator cbor::array() const
{
    return this->to_array();
}
cbor::operator cbor::map() const
{
    return this->to_map();
}
cbor::operator cbor::simple() const
{
    return this->to_simple();
}
cbor::operator bool() const
{
    return this->to_bool();
}
cbor::operator double() const
{
    return this->to_float();
}
cbor::type_t cbor::type() const
{
    return this->m_type;
}
uint64_t cbor::tag() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_TAGGED:
        return this->m_value;
    default:
        return 0;
    }
}
cbor cbor::child() const
{
    switch (this->m_type)
    {
    case cbor::TYPE_TAGGED:
        return this->m_array.front();
    default:
        return cbor();
    }
}
bool cbor::operator<(const cbor &other) const
{
    if (this->m_type < other.m_type)
    {
        return true;
    }
    if (this->m_type > other.m_type)
    {
        return false;
    }
    switch (this->m_type)
    {
    case cbor::TYPE_BINARY:
        return this->m_binary < other.m_binary;
    case cbor::TYPE_STRING:
        return this->m_string < other.m_string;
    case cbor::TYPE_ARRAY:
        return this->m_array < other.m_array;
    case cbor::TYPE_MAP:
        return this->m_map < other.m_map;
    case cbor::TYPE_TAGGED:
        if (this->m_value < other.m_value)
        {
            return true;
        }
        if (this->m_value > other.m_value)
        {
            return false;
        }
        return this->m_array.front() < other.m_array.front();
    default:
        return this->m_value < other.m_value;
    }
}
bool cbor::operator==(const cbor &other) const
{
    if (this->m_type != other.m_type)
    {
        return false;
    }
    switch (this->m_type)
    {
    case cbor::TYPE_BINARY:
        return this->m_binary == other.m_binary;
    case cbor::TYPE_STRING:
        return this->m_string == other.m_string;
    case cbor::TYPE_ARRAY:
        return this->m_array == other.m_array;
    case cbor::TYPE_MAP:
        return this->m_map == other.m_map;
    case cbor::TYPE_TAGGED:
        if (this->m_value != other.m_value)
        {
            return false;
        }
        return this->m_array == other.m_array;
    default:
        return this->m_value == other.m_value;
    }
}
bool cbor::operator!=(const cbor &other) const
{
    return !(*this == other);
}
void read_uint(std::istream &in, int &major, int &minor, uint64_t &value)
{
    major = (in.peek() >> 5) & 7;
    minor = in.get() & 31;
    value = 0;
    switch (minor)
    {
    case 27:
        value |= (unsigned long long)in.get() << 56;
        value |= (unsigned long long)in.get() << 48;
        value |= (unsigned long long)in.get() << 40;
        value |= (unsigned long long)in.get() << 32;
    case 26:
        value |= in.get() << 24;
        value |= in.get() << 16;
    case 25:
        value |= in.get() << 8;
    case 24:
        value |= in.get();
        break;
    default:
        value = minor;
        break;
    }
}
bool cbor::read(std::istream &in)
{
    cbor item;
    int major, minor;
    uint64_t value;
    read_uint(in, major, minor, value);
    switch (major)
    {
    case 0:
        if (minor > 27)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_UNSIGNED;
        item.m_value = value;
        break;
    case 1:
        if (minor > 27)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_NEGATIVE;
        item.m_value = value;
        break;
    case 2:
        if (minor > 27 && minor < 31)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_BINARY;
        if (minor == 31)
        {
            while (in.good() && in.peek() != 255)
            {
                read_uint(in, major, minor, value);
                if (major != 2 || minor > 27)
                {
                    in.setstate(std::ios_base::failbit);
                    return false;
                }
                for (uint64_t i = 0; in.good() && i != value; ++i)
                {
                    item.m_binary.push_back(in.get());
                }
            }
            in.get();
        }
        else
        {
            for (uint64_t i = 0; in.good() && i != value; ++i)
            {
                item.m_binary.push_back(in.get());
            }
        }
        break;
    case 3:
        if (minor > 27 && minor < 31)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_STRING;
        if (minor == 31)
        {
            while (in.good() && in.peek() != 255)
            {
                read_uint(in, major, minor, value);
                if (major != 3 || minor > 27)
                {
                    in.setstate(std::ios_base::failbit);
                    return false;
                }
                for (uint64_t i = 0; in.good() && i != value; ++i)
                {
                    item.m_string.push_back(in.get());
                }
            }
            in.get();
        }
        else
        {
            for (uint64_t i = 0; in.good() && i != value; ++i)
            {
                item.m_string.push_back(in.get());
            }
        }
        break;
    case 4:
        if (minor > 27 && minor < 31)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_ARRAY;
        if (minor == 31)
        {
            while (in.good() && in.peek() != 255)
            {
                cbor child;
                child.read(in);
                item.m_array.push_back(child);
            }
            in.get();
        }
        else
        {
            for (uint64_t i = 0; in.good() && i != value; ++i)
            {
                cbor child;
                child.read(in);
                item.m_array.push_back(child);
            }
        }
        break;
    case 5:
        if (minor > 27 && minor < 31)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_MAP;
        if (minor == 31)
        {
            while (in.good() && in.peek() != 255)
            {
                cbor key, value;
                key.read(in);
                value.read(in);
                item.m_map.insert(std::make_pair(key, value));
            }
            in.get();
        }
        else
        {
            for (uint64_t i = 0; in.good() && i != value; ++i)
            {
                cbor key, value;
                key.read(in);
                value.read(in);
                item.m_map.insert(std::make_pair(key, value));
            }
        }
        break;
    case 6:
    {
        if (minor > 27)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        item.m_type = cbor::TYPE_TAGGED;
        item.m_value = value;
        cbor child;
        child.read(in);
        item.m_array.push_back(child);
        break;
    }
    case 7:
        if (minor > 27)
        {
            in.setstate(std::ios_base::failbit);
            return false;
        }
        switch (minor)
        {
        case 25:
        {
            item.m_type = cbor::TYPE_FLOAT;
            int sign = value >> 15;
            int exponent = value >> 10 & 31;
            int significand = value & 1023;
            if (exponent == 31)
            {
                if (significand)
                {
                    item.m_float = NAN;
                }
                else if (sign)
                {
                    item.m_float = -INFINITY;
                }
                else
                {
                    item.m_float = INFINITY;
                }
            }
            else if (exponent == 0)
            {
                if (sign)
                {
                    item.m_float = -ldexp(significand, -24);
                }
                else
                {
                    item.m_float = ldexp(significand, -24);
                }
            }
            else
            {
                if (sign)
                {
                    item.m_float = -ldexp(1024 | significand, exponent - 25);
                }
                else
                {
                    item.m_float = ldexp(1024 | significand, exponent - 25);
                }
            }
            break;
        }
        case 26:
        {
            union {
                float f;
                uint32_t i;
            };
            i = value;
            item.m_type = cbor::TYPE_FLOAT;
            item.m_float = f;
            break;
        }
        case 27:
            item.m_type = cbor::TYPE_FLOAT;
            item.m_value = value;
            break;
        default:
            item.m_type = cbor::TYPE_SIMPLE;
            item.m_value = value;
        }
        break;
    }
    if (!in.good())
    {
        in.setstate(std::ios_base::failbit);
        return false;
    }
    *this = item;
    return true;
}
void write_uint8(std::ostream &out, int major, uint64_t value)
{
    if (value < 24)
    {
        out.put(major << 5 | value);
    }
    else
    {
        out.put(major << 5 | 24);
        out.put(value);
    }
}
void write_uint16(std::ostream &out, int major, uint64_t value)
{
    out.put(major << 5 | 25);
    out.put(value >> 8);
    out.put(value);
}
void write_uint32(std::ostream &out, int major, uint64_t value)
{
    out.put(major << 5 | 26);
    out.put(value >> 24);
    out.put(value >> 16);
    out.put(value >> 8);
    out.put(value);
}
void write_uint64(std::ostream &out, int major, uint64_t value)
{
    out.put(major << 5 | 27);
    out.put(value >> 56);
    out.put(value >> 48);
    out.put(value >> 40);
    out.put(value >> 32);
    out.put(value >> 24);
    out.put(value >> 16);
    out.put(value >> 8);
    out.put(value);
}
void write_uint(std::ostream &out, int major, uint64_t value)
{
    if ((value >> 8) == 0)
    {
        write_uint8(out, major, value);
    }
    else if ((value >> 16) == 0)
    {
        write_uint16(out, major, value);
    }
    else if (value >> 32 == 0)
    {
        write_uint32(out, major, value);
    }
    else
    {
        write_uint64(out, major, value);
    }
}
void write_float(std::ostream &out, double value)
{
    if (double(float(value)) == value)
    {
        union {
            float f;
            uint32_t i;
        };
        f = value;
        write_uint32(out, 7, i);
    }
    else
    {
        union {
            double f;
            uint64_t i;
        };
        f = value;
        write_uint64(out, 7, i);
    }
}
void cbor::write(std::ostream &out) const
{
    switch (this->m_type)
    {
    case cbor::TYPE_UNSIGNED:
        write_uint(out, 0, this->m_value);
        break;
    case cbor::TYPE_NEGATIVE:
        write_uint(out, 1, this->m_value);
        break;
    case cbor::TYPE_BINARY:
        write_uint(out, 2, this->m_binary.size());
        out.write(reinterpret_cast<const char *>(this->m_binary.data()), this->m_binary.size());
        break;
    case cbor::TYPE_STRING:
        write_uint(out, 3, this->m_string.size());
        out.write(this->m_string.data(), this->m_string.size());
        break;
    case cbor::TYPE_ARRAY:
        write_uint(out, 4, this->m_array.size());
        for (cbor::array::const_iterator it = this->m_array.begin(); it != this->m_array.end(); ++it)
        {
            it->write(out);
        }
        break;
    case cbor::TYPE_MAP:
        write_uint(out, 5, this->m_map.size());
        for (cbor::map::const_iterator it = this->m_map.begin(); it != this->m_map.end(); ++it)
        {
            it->first.write(out);
            it->second.write(out);
        }
        break;
    case cbor::TYPE_TAGGED:
        write_uint(out, 6, this->m_value);
        this->m_array.front().write(out);
        break;
    case cbor::TYPE_SIMPLE:
        write_uint8(out, 7, this->m_value);
        break;
    case cbor::TYPE_FLOAT:
        write_float(out, this->m_float);
        break;
    }
}
bool cbor::validate(const cbor::binary &in)
{
    std::istringstream buf1(std::string(in.begin(), in.end()));
    cbor buf2;
    return buf2.read(buf1) && buf1.peek() == EOF;
}
cbor cbor::decode(const cbor::binary &in)
{
    std::istringstream buf1(std::string(in.begin(), in.end()));
    cbor buf2;
    if (buf2.read(buf1) && buf1.peek() == EOF)
    {
        return buf2;
    }
    return cbor();
}
cbor::binary cbor::encode(const cbor &in)
{
    std::ostringstream buf1;
    in.write(buf1);
    std::string buf2 = buf1.str();
    return cbor::binary(buf2.begin(), buf2.end());
}
cbor::string cbor::debug(const cbor &in)
{
    std::ostringstream out;
    switch (in.m_type)
    {
    case cbor::TYPE_UNSIGNED:
        out << in.m_value;
        break;
    case cbor::TYPE_NEGATIVE:
        if (1 + in.m_value == 0)
        {
            out << "-18446744073709551616";
        }
        else
        {
            out << "-" << 1 + in.m_value;
        }
        break;
    case cbor::TYPE_BINARY:
        out << "h'";
        out << std::hex;
        out.fill('0');
        for (cbor::binary::const_iterator it = in.m_binary.begin(); it != in.m_binary.end(); ++it)
        {
            out.width(2);
            out << int(*it);
        }
        out << "'";
        break;
    case cbor::TYPE_STRING:
        out << "\"";
        out << std::hex;
        out.fill('0');
        for (cbor::string::const_iterator it = in.m_string.begin(); it != in.m_string.end(); ++it)
        {
            switch (*it)
            {
            case '\n':
                out << "\\n";
                break;
            case '\r':
                out << "\\r";
                break;
            case '\"':
                out << "\\\"";
                break;
            case '\\':
                out << "\\\\";
                break;
            default:
                if ((unsigned char)*it < '\x20')
                {
                    out << "\\u";
                    out.width(4);
                    out << (int)(unsigned char)*it;
                }
                else
                {
                    out << *it;
                }
                break;
            }
        }
        out << "\"";
        break;
    case cbor::TYPE_ARRAY:
        out << "[";
        for (cbor::array::const_iterator it = in.m_array.begin(); it != in.m_array.end(); ++it)
        {
            if (it != in.m_array.begin())
            {
                out << ", ";
            }
            out << cbor::debug(*it);
        }
        out << "]";
        break;
    case cbor::TYPE_MAP:
        out << "{";
        for (cbor::map::const_iterator it = in.m_map.begin(); it != in.m_map.end(); ++it)
        {
            if (it != in.m_map.begin())
            {
                out << ", ";
            }
            out << cbor::debug(it->first) << ": " << cbor::debug(it->second);
        }
        out << "}";
        break;
    case cbor::TYPE_TAGGED:
        out << in.m_value << "(" << cbor::debug(in.m_array.front()) << ")";
        break;
    case cbor::TYPE_SIMPLE:
        switch (in.m_value)
        {
        case cbor::SIMPLE_FALSE:
            out << "false";
            break;
        case cbor::SIMPLE_TRUE:
            out << "true";
            break;
        case cbor::SIMPLE_NULL:
            out << "null";
            break;
        case cbor::SIMPLE_UNDEFINED:
            out << "undefined";
            break;
        default:
            out << "simple(" << in.m_value << ")";
            break;
        }
        break;
    case cbor::TYPE_FLOAT:
        if (std::isinf(in.m_float))
        {
            if (in.m_float < 0)
            {
                out << "-";
            }
            out << "Infinity";
        }
        else if (std::isnan(in.m_float))
        {
            out << "NaN";
        }
        else
        {
            out << std::showpoint << in.m_float;
        }
        break;
    }
    return out.str();
}


/**************************************************
 *                    CONSTANTS                   *
 *                   Proof Types                  *
 **************************************************/
const uint8_t proofType_NONE = 0x00;
const uint8_t proofType_TLSNotary = 0x10;
const uint8_t proofType_Ledger = 0x30;
const uint8_t proofType_Android = 0x40;
const uint8_t proofType_Native = 0xF0;
const uint8_t proofStorage_IPFS = 0x01;

#ifndef CONSTANTS 
#define CONSTANTS
const uint8_t CODE_HASH_RANDOMDS[32] = {
    253, 148, 250, 113, 188, 11, 161, 13, 57, 212, 100, 208, 216, 244, 101, 239, 238, 240, 162, 118, 78, 56, 135, 252, 201, 223, 65, 222, 210, 15, 80, 92
};
const uint8_t LEDGERKEY[64] = {
    127, 185, 86, 70, 156, 92, 155, 137, 132, 13, 85, 180, 53, 55, 230, 106, 152, 221, 72, 17, 234, 10, 39, 34, 66, 114, 194, 229, 98, 41, 17, 232, 83, 122, 47, 142, 134, 164, 107, 174, 200, 40, 100, 233, 141, 208, 30, 156, 204, 47, 139, 197, 223, 201, 203, 229, 169, 26, 41, 4, 152, 221, 150, 228
};
#endif


/**************************************************
 *                     TABLES                     *
 *                   Definition                   *
 **************************************************/
struct [[eosio::table, eosio::contract("oraclizeconn")]] snonce
{
    name sender;
    uint32_t nonce;

    uint64_t primary_key() const { return sender.value; }
};

struct [[eosio::table, eosio::contract("oraclizeconn")]] cbaddr
{
    name sender;

    uint64_t  primary_key() const { return sender.value; }
};

struct [[eosio::table]] scommitment
{
    name shortqueryid;
    capi_checksum256 queryid;
    capi_checksum256 commitment;

    uint64_t primary_key() const { return shortqueryid.value; }
};

struct [[eosio::table, eosio::contract("oraclizeconn")]] spubkey
{
    name key;
    capi_checksum256 randomDS_lastSessionPubkeyHash;

    uint64_t primary_key() const { return key.value; }
    capi_checksum256 get_randomDS_lastSessionPubkeyHash() const { return randomDS_lastSessionPubkeyHash; }
};

struct [[eosio::table]] queryid
{
    name key;
    capi_checksum256 qid;
    uint8_t active;

    uint64_t primary_key() const { return key.value; }
};

typedef eosio::multi_index<name("snonce"), snonce> ds_snonce;
typedef eosio::multi_index<name("cbaddr"), cbaddr> ds_cbaddr;
typedef eosio::multi_index<name("scommitment"), scommitment> ds_scommitment;
typedef eosio::multi_index<name("spubkey"), spubkey> ds_spubkey;
typedef eosio::multi_index<name("queryid"), queryid> ds_queryid;


/**************************************************
 *                      MACRO                     *
 *                   Definitions                  *
 **************************************************/
#ifndef ORACLIZE_PAYER
#define ORACLIZE_PAYER _self
#endif // ORACLIZE_PAYER

#define oraclize_query(...) __oraclize_query(ORACLIZE_PAYER, __VA_ARGS__, _self)
#define oraclize_newRandomDSQuery(...) __oraclize_newRandomDSQuery(ORACLIZE_PAYER, __VA_ARGS__, _self)
#define oraclize_queryId_localEmplace(...) __oraclize_queryId_localEmplace(__VA_ARGS__, _self)
#define oraclize_queryId_match(...) __oraclize_queryId_match(__VA_ARGS__, _self)


/**************************************************
 *                PUBLIC FUNCTIONS                *
 *                 Implementation                 *
 **************************************************/
uint64_t oraclize_cbAddress()
{
    // go to the connector table which identify the sender 
    ds_cbaddr cb_addrs("oraclizeconn"_n, "oraclizeconn"_n.value);
    // point to the first element of the table: 1. if table is empty 
    auto itr = cb_addrs.begin();

    uint64_t cbaddr = (itr != cb_addrs.end()) ? itr->sender.value : 0;
    return cbaddr;
}

std::string vector_to_string(const std::vector<uint8_t> v)
{
    std::string v_str(v.begin(), v.end());
    return v_str;
}

std::string checksum256_to_string(const capi_checksum256 c)
{
    char hexstr[64];
    for (int i = 0; i < 32; i++)
    {  
        sprintf(hexstr + i * 2, "%02x", c.hash[i]);
    }
    std::string c_str = std::string(hexstr);
    return c_str;
}

std::string chara_to_hexstring(uint8_t *input, const int size)
{
    char hexstr[size * 2];
    for (int i = 0; i < size; i++)
    {
        sprintf(hexstr + i * 2, "%02x", input[i]);
    }
    std::string c_str = std::string(hexstr);
    return c_str;
}

std::string vector_to_hexstring(std::vector<uint8_t> *input)
{
    int size = input->size();
    char hexstr[size * 2];
    for (int i = 0; i < size; i++)
    {
        sprintf(hexstr + i * 2, "%02x", input->at(i));
    }
    std::string c_str = std::string(hexstr);
    return c_str;
}

std::vector<uint8_t> string_to_vector(const std::string s)
{
    std::vector<uint8_t> s_v(s.begin(), s.end());
    return s_v;
}

std::vector<uint8_t> uint32_to_vector8(uint32_t num)
{
    std::vector<uint8_t> ba(8);
    const uint32_t mask = 0xFF;
    for(int i = 0; i < 8; i++)
    {
	    ba[i] = num & mask;
	    num = num >> 8;
    }
    return ba;
}

std::vector<uint8_t> uint32_to_vector32_bigendian(uint32_t num)
{
    std::vector<uint8_t> ba(32);
    const uint32_t mask = 0xFF;
    for(int i = 31; i > -1; i--)
    {
	    ba[i] = num & mask;
	    num = num >> 8;
    }
    return ba;
}

std::vector<uint8_t> checksum256_to_vector32(const capi_checksum256 c)
{
    std::vector<uint8_t> ba(32);
    for (int i = 0; i < 32; i++)
    {
	    ba[i] = c.hash[i];
    }
    return ba;
}


/**************************************************
 *               INTERNAL FUNCTIONS               *
 *                  Definitions                   *
 **************************************************/
vector<uint8_t> __oraclize_cbor_encode(const vector<vector<uint8_t>> params)
{
    cbor::array item;
    for (int i = 0; i < params.size(); i++)
    {
        item.push_back(params[i]);
    }
    return cbor::encode(item);
}

capi_checksum256 __oraclize_randomDS_getSessionPubkeyHash()
{
    ds_spubkey spubkeys("oraclizeconn"_n, "oraclizeconn"_n.value);
    // just one value in the table with the key=1
    const auto itr = spubkeys.find(1);
    capi_checksum256 sessionPubkeyHash;
    if (itr != spubkeys.end())
    {
        sessionPubkeyHash = itr->get_randomDS_lastSessionPubkeyHash();
    }
    return sessionPubkeyHash;
}

uint32_t __oraclize_getSenderNonce(name sender)
{
    ds_snonce last_nonces("oraclizeconn"_n, "oraclizeconn"_n.value);
    const auto itr = last_nonces.find(sender.value);
    uint32_t nonce = 0;
    if (itr != last_nonces.end())
    {
        nonce = itr->nonce;
    }
    return nonce;
}

capi_checksum256 __oraclize_getNextQueryId(const name sender)
{
    // Get values to generate the queryId
    const uint32_t nonce = __oraclize_getSenderNonce(sender);
    const size_t tx_size = transaction_size();
    // calculate the hash of the previous values
    uint8_t tbh[sizeof(sender) + sizeof(nonce) + sizeof(tx_size)];
    std::memcpy(tbh, &sender, sizeof(sender));
    std::memcpy(tbh + sizeof(sender), &nonce, sizeof(nonce));
    std::memcpy(tbh + sizeof(sender) + sizeof(nonce), &tx_size, sizeof(tx_size));
    capi_checksum256 calc_hash;
    sha256((char *)tbh, sizeof(tbh), &calc_hash);
    return calc_hash;
}

// check that the queryId being passed matches with the one in the customer local table, return true/false accordingly
bool __oraclize_queryId_match(const capi_checksum256 queryId, const name sender)
{
    // retreive the short queryId from the full queryId
    name myQueryId_short;
    std::memcpy(&myQueryId_short, &queryId.hash, sizeof(myQueryId_short));
    
    // access the local table 
    ds_queryid queryids(sender, sender.value); 
    const auto itr = queryids.find(myQueryId_short.value);
    capi_checksum256 queryId_expected;
    std::memcpy(&queryId_expected, 0, sizeof(queryId_expected));
    if (itr != queryids.end())
    {
        queryId_expected = itr->qid;
    }
    
	// convert queryId and queryId_expecte to string to compare them
	std::string queryId_str__expected = checksum256_to_string(queryId_expected);
	std::string queryId_str = checksum256_to_string(queryId);
 
	// compare the queryids 
	if (queryId_str != queryId_str__expected)
	{
        return false;
	}
	else
	{
	    return true;
	}
}

void __oraclize_queryId_localEmplace(const capi_checksum256 myQueryId, const name sender)
{
    // retreive the short queryId to use it as an index
    name myQueryId_short;
    std::memcpy(&myQueryId_short, &myQueryId.hash, sizeof(myQueryId_short));
    
	// save the queryId in the local table
    ds_queryid queryids(sender, sender.value);
    queryids.emplace(sender, [&](auto& o) {
	    o.key = myQueryId_short;
	    o.qid = myQueryId;
	    o.active = true;
    });
}


/**************************************************
 *                  Oraclize Query                *
 *                    Strings                     *
 **************************************************/
capi_checksum256 __oraclize_query(const name user, const unsigned int timestamp, const std::string datasource, const std::string query, const uint8_t prooftype, const name sender)
{
    capi_checksum256 queryId = __oraclize_getNextQueryId(sender);
    action(permission_level{user, "active"_n},
	       "oraclizeconn"_n, 
	       "querystr"_n,
	       std::make_tuple(sender, (int8_t)1, (uint32_t)timestamp, queryId, datasource, query, prooftype)
    ).send();
    return queryId;
}

capi_checksum256 __oraclize_query(const name user, const std::string datasource, const std::string query, const name sender)
{
    return __oraclize_query(user, 0, datasource, query, 0, sender);
}

capi_checksum256 __oraclize_query(const name user, const unsigned int timestamp, const std::string datasource, const std::string query, const name sender)
{
    return __oraclize_query(user, timestamp, datasource, query, 0, sender);
}

capi_checksum256 __oraclize_query(const name user, const std::string datasource, const std::string query, const uint8_t prooftype, const name sender)
{
    return __oraclize_query(user, 0, datasource, query, prooftype, sender);
}


/**************************************************
 *                 Oraclize Query                 *
 *                   Bytearrays                   *
 **************************************************/
capi_checksum256 __oraclize_query(const name user, const unsigned int timestamp, const std::string datasource, const vector<vector<uint8_t>> query, const uint8_t prooftype, const name sender)
{
    const capi_checksum256 queryId = __oraclize_getNextQueryId(sender);
    printhex(__oraclize_cbor_encode(query).data(), __oraclize_cbor_encode(query).size());
    const auto n = name{user};
    const std::string str = n.to_string();
    action(permission_level{user, "active"_n},
	       "oraclizeconn"_n, 
	       "queryba"_n,
	       std::make_tuple(sender, (int8_t)1, (uint32_t)timestamp, queryId, datasource, __oraclize_cbor_encode(query), prooftype)
    ).send();
    return queryId;
}

capi_checksum256 __oraclize_query(const name user, const std::string datasource, const vector<vector<uint8_t>> query, const name sender)
{
    return __oraclize_query(user, 0, datasource, query, 0, sender);
}

capi_checksum256 __oraclize_query(const name user, const unsigned int timestamp, const std::string datasource, const vector<vector<uint8_t>> query, const name  sender)
{
    return __oraclize_query(user, timestamp, datasource, query, 0, sender);
}

capi_checksum256 __oraclize_query(const name user, const std::string datasource, const vector<vector<uint8_t>> query, const uint8_t prooftype, const name sender)
{
    return __oraclize_query(user, 0, datasource, query, prooftype, sender);
}


/**************************************************
 *                 Oraclize Query                 *
 *                   Random DS                    *
 **************************************************/
void __oraclize_randomDS_setCommitment(const capi_checksum256 queryId, const capi_checksum256 commitment, const name payer)
{
    name myQueryId_short; // Calculate the short queryId, to use it as a key of the table
    std::memcpy(&myQueryId_short, &queryId.hash[0], sizeof(myQueryId_short));
    ds_scommitment last_commitments(payer, payer.value); // Set the commitment in the eos table of the caller
    last_commitments.emplace(payer, [&](auto &o) { //key must be a uint64_t so a short query Id is used
        o.shortqueryid = myQueryId_short;
        o.queryid = queryId;
        o.commitment = commitment;
    });
}

capi_checksum256 __oraclize_newRandomDSQuery(const name user, const uint32_t _delay, const uint8_t _nbytes, const name sender)
{
    // 1. NBYTES - Convert nbytes to bytearray
    std::vector<uint8_t> nbytesBa(1);
    nbytesBa[0] = _nbytes;

    // 2. SESSIONKEYHASH - Get the sessionKeyHash from the ledger public key.
    const capi_checksum256 sessionPubkeyHash = __oraclize_randomDS_getSessionPubkeyHash();
    std::vector<uint8_t> sessionPubkeyHashBa(32);
    sessionPubkeyHashBa = checksum256_to_vector32(sessionPubkeyHash);

    // 3. UNONCE - Need something block dependent so we decided to perform the hash of those 4 block dependent fields. This value have to be unpredictable from Oraclize
    const size_t tx_size = transaction_size();
    const int tapos_block_num_ = tapos_block_num();
    const int tapos_block_prefix_ = tapos_block_prefix();
    uint8_t unonce[sizeof(tx_size) + sizeof(tapos_block_num_) + sizeof(tapos_block_prefix_)]; // Fill the unonce array: now() + transaction_size + tapos_block_num + tapos_block_prefix
    std::memcpy(unonce, &tx_size, sizeof(tx_size));
    std::memcpy(unonce + sizeof(tx_size), &tapos_block_num_, sizeof(tapos_block_num_));
    std::memcpy(unonce + sizeof(tx_size) + sizeof(tapos_block_num_), &tapos_block_prefix_, sizeof(tapos_block_prefix_));
    capi_checksum256 unonceHash; // Container for the unonce hash
    sha256((char *)unonce, sizeof(unonce), &unonceHash);
    std::vector<uint8_t> unonceHashBa(32); // Convert the unonce hash in bytearray
    unonceHashBa = checksum256_to_vector32(unonceHash);

    // 4. DELAY - Delay converted in a big endian bytearray
    const uint32_t delayLedgerTime = _delay * 10; // Convert from seconds to ledger timer ticks
    std::vector<uint8_t> delayBaBigEndian(32);
    delayBaBigEndian = uint32_to_vector32_bigendian(delayLedgerTime);
    // Set args to be passed as params of the oraclize "random" query
    vector<vector<uint8_t>> args;
    args.push_back(unonceHashBa);
    args.push_back(nbytesBa);
    args.push_back(sessionPubkeyHashBa);
    args.push_back(delayBaBigEndian);
    // Call the oraclize_query and get the queryId
    const capi_checksum256 queryId = __oraclize_query(user,"random", args, proofType_Ledger, sender); // proofType and datasource are always fixed in this function
    // Calculate the commitment and call a function to set it    
    std::vector<uint8_t> delayBa(8); // delay converted to  8 byte
    delayBa = uint32_to_vector8(delayLedgerTime);
    capi_checksum256 unonceHashBaHash; // unonce has to be passed hashed
    uint8_t* charArray = &unonceHashBa[0];
    sha256((char *) charArray, unonceHashBa.size(), &unonceHashBaHash);
    uint8_t commitmentTbh[8 + 1 + 32 + args[2].size()]; // Calculate the commitment to be hashed with the size of: 8 + 1 + 32 + 32
    std::memcpy(commitmentTbh, &delayBa[0], 8); // 8
    std::memcpy(commitmentTbh + 8, &args[1][0], 1); // 8 + 1 
    std::memcpy(commitmentTbh + 8 + 1, &unonceHashBaHash, 32); // 8 + 1 + 32 == commitmentSlice1
    std::memcpy(commitmentTbh + delayBa.size() + args[1].size() + sizeof(unonceHashBaHash), &args[2][0], args[2].size()); // 8 + 1 + 32 + 32 (commitmentSlice1 + sessionPubkeyHashBa)
    capi_checksum256 commitment; // Container for the commitment hash
    sha256((char *)commitmentTbh, sizeof(commitmentTbh), &commitment);
    const name payer = user; // Payer for setting the commitment
    __oraclize_randomDS_setCommitment(queryId, commitment, payer); // Call the function to set query Id and commitment in the table
    
    return queryId;
}

void __oraclize_randomDS_get_signature_component(uint8_t component[32], const uint8_t signature[], const uint8_t signature_len, const uint8_t length_idx)
{
    eosio_assert(signature_len > length_idx, "Invalid index");
    uint8_t component_len = signature[length_idx];
    uint8_t byte_to_jump = component_len % 32;
    std::memcpy(component, &signature[length_idx + 1 + byte_to_jump], component_len - byte_to_jump);
}

bool __oraclize_randomDS_matchBytes32Prefix(const capi_checksum256 content, const uint8_t prefix[], const uint8_t prefix_len, const uint8_t n_random_bytes)
{
    eosio_assert(prefix_len == n_random_bytes, "Prefix length and random bytes number should match.");
    for (int i = 0; i < n_random_bytes; ++i)
        if (content.hash[i] != prefix[i])
            return false;
    return true;
}

bool __oraclize_randomDS_test_pubkey_signature(const uint8_t whatever, const uint8_t v, const uint8_t r[32], const uint8_t s[32], const capi_checksum256 digest, const uint8_t pubkey[64])
{
    uint8_t signature[66];
    signature[0] = whatever; // 0;
    signature[1] = v;        // v;
    std::memcpy(signature + 2, r, 32);
    std::memcpy(signature + 2 + 32, s, 32);
    uint8_t compressed[34];
    const int compressed_size = recover_key(&digest, (char *)signature, sizeof(signature), (char *)compressed, sizeof(compressed));
    if (compressed_size != 34)
        return false;
    if (compressed[1] != 0x02 && compressed[1] != 0x03)
        return false;
    // Discard the first (0x00) and the second byte (0x02 or 0x03)
    return std::memcmp(compressed + 2, pubkey, 32) == 0;
    // Note: eosio doesn't provide a way to decompress the key obtained by recover_key
    // So here we compress the given pub key (by extracting the relevant x coord)
    // and then check if it's equal to the one returned by the function
}

bool __oraclize_randomDS_verifySig(const capi_checksum256 digest, const uint8_t der_signature[], const uint8_t der_signature_len, const uint8_t pubkey[64])
{
    uint8_t r[32];
    uint8_t s[32];
    __oraclize_randomDS_get_signature_component(r, der_signature, der_signature_len, 3);
    __oraclize_randomDS_get_signature_component(s, der_signature, der_signature_len, 4 + der_signature[3] + 1);
    // We try either with v=27 or with v=28
    bool test_v27 = __oraclize_randomDS_test_pubkey_signature(0, 27, r, s, digest, pubkey);
    bool test_v28 = __oraclize_randomDS_test_pubkey_signature(0, 28, r, s, digest, pubkey);
    return test_v27 || test_v28;
}

uint8_t oraclize_randomDS_proofVerify(const capi_checksum256 queryId, const std::vector<uint8_t> result, const std::vector<uint8_t> proof, const name payer)
{
    /*******************************************************************************************
     *                                                                                         *
     *   Step 1: the prefix has to match 'LP\x01' (Ledger Proof version 1)                     * 
     *                                                                                         *
     *******************************************************************************************/
    if (proof[0] != 'L' || proof[1] != 'P' || proof[2] != 1)
        return 1;


    /********************************************************************************************
     *                                                                                          *
     *   Step 2: the unique keyhash has to match with the sha256 of (context name = queryId)    *
     *                                                                                          *
     ********************************************************************************************/
    const uint8_t ledgerProofLength = 3 + 65 + (proof[3 + 65 + 1] + 2) + 32;
    uint8_t keyhash[32];
    std::memcpy(keyhash, &proof.data()[ledgerProofLength], 32);
    const char context_name[] = {'e','o','s','_','t','e','s','t','n','e','t','_','j','u','n','g','l','e'};
    capi_checksum256 calc_hash;
    uint8_t tbh2[sizeof(context_name) + sizeof(queryId.hash)];
    std::memcpy(tbh2, &context_name, sizeof(context_name));
    std::memcpy(tbh2 + sizeof(context_name), &queryId.hash, sizeof(queryId.hash));
    sha256((char *)tbh2, sizeof(tbh2), &calc_hash);
    std::memcmp(calc_hash.hash, keyhash, sizeof(keyhash));


    /********************************************************************************************
     *                                                                                          *
     *   Step 3: we assume sig1 is valid (it will be verified during step 5)                    *
     *           and we verify if 'result' is the prefix of sha256(sig1)                        *
     *                                                                                          *
     ********************************************************************************************/
    const uint8_t sig1_len = proof[ledgerProofLength + (32 + 8 + 1 + 32) + 1] + 2;
    uint8_t sig1[sig1_len];
    std::memcpy(sig1, &proof.data()[ledgerProofLength + (32 + 8 + 1 + 32)], sig1_len);
    capi_checksum256 sig1_hash;
    sha256((char *)sig1, sizeof(sig1), &sig1_hash);
    if (!__oraclize_randomDS_matchBytes32Prefix(sig1_hash, result.data(), result.size(), proof[ledgerProofLength + 32 + 8]))
        return 3;


    /********************************************************************************************
     *                                                                                          *
     *   Step 4: commitment match verification,                                                 *
     *           sha256(delay, nbytes, unonce, sessionKeyHash) == commitment in table.          *
     *                                                                                          *
     ********************************************************************************************/
    const uint8_t slice_offset = 8 + 1 + 32; // delay + nbytes + unonceHashBa
    uint8_t commitmentSlice1[slice_offset];
    std::memcpy(commitmentSlice1, &proof.data()[ledgerProofLength + 32], sizeof(commitmentSlice1));
    // Extract the session public key and calculate the session publick key hash
    uint8_t sessionPubKey[64];
    const uint16_t sig2offset = ledgerProofLength + 32 + (8 + 1 + 32) + sig1_len + 65; // ledgerProofLength+32+(8+1+32)+sig1.len+65
    std::memcpy(sessionPubKey, &proof.data()[sig2offset - 64], sizeof(sessionPubKey));
    capi_checksum256 sessionPubkeyHash; // Calculate the key hash
    sha256((char *)sessionPubKey, sizeof(sessionPubKey), &sessionPubkeyHash);
    vector<uint8_t> sessionPubkeyHashBa(32); // Convert to bytearray the public key hash 
    sessionPubkeyHashBa = checksum256_to_vector32(sessionPubkeyHash);
    // Recreate the lastCommitment to compare with the table one
    capi_checksum256 lastCommitment;
    uint8_t tbh[slice_offset + 32];
    std::memcpy(tbh, &commitmentSlice1, slice_offset);
    std::memcpy(tbh + slice_offset, &sessionPubkeyHashBa[0], 32);
    sha256((char *)tbh, sizeof(tbh), &lastCommitment);
    // Retrieve the table commitment
    ds_scommitment last_commitments(payer, payer.value);
    uint64_t myQueryId_short;
    std::memcpy(&myQueryId_short, &queryId.hash[0], sizeof(myQueryId_short));
    // Check the query id with the one in the table
    const auto itr = last_commitments.find(myQueryId_short);
    capi_checksum256 queryId_expected;
    if (itr == last_commitments.end())
        queryId_expected = itr->queryid;
    const std::string queryId_str__expected = checksum256_to_string(last_commitments.find(myQueryId_short)->queryid);
    const std::string queryId_str = checksum256_to_string(queryId);
    if (queryId_str != queryId_str__expected)
        return 4;
    // Check the commitment with the one in the table
    const std::string lastCommitment_str__expected = checksum256_to_string(last_commitments.find(myQueryId_short)->commitment);
    const std::string lastCommitment_str = checksum256_to_string(lastCommitment);
    if (lastCommitment_str != lastCommitment_str__expected)
        return 4;


    /********************************************************************************************
     *                                                                                          *
     *   Step 5: validity verification for sig1 (keyhash and args signed with the sessionKey)   *
     *                                                                                          *
     ********************************************************************************************/
    uint8_t toSign1[32 + 8 + 1 + 32];
    std::memcpy(toSign1, &proof.data()[ledgerProofLength], sizeof(toSign1));
    capi_checksum256 toSign1_hash;
    sha256((char *)toSign1, sizeof(toSign1), &toSign1_hash);
    if (!__oraclize_randomDS_verifySig(toSign1_hash, sig1, sizeof(sig1), sessionPubKey))
        return 5;


    /********************************************************************************************
     *                                                                                          *
     *   Step 6:  verify the attestation signature,                                             *
     *            APPKEY1 must sign the sessionKey from the correct ledger app (CODEHASH)       *
     *                                                                                          *
     ********************************************************************************************/
    uint8_t sig2[proof[sig2offset + 1] + 2];
    std::memcpy(sig2, &proof.data()[sig2offset], sizeof(sig2));
    uint8_t appkey_pubkey[64];
    std::memcpy(appkey_pubkey, &proof.data()[3 + 1], sizeof(appkey_pubkey));
    uint8_t toSign2[1 + 65 + 32];
    toSign2[0] = 1; // role
    std::memcpy(toSign2 + 1, &proof.data()[sig2offset - 65], 65);
    std::memcpy(toSign2 + 65 + 1, CODE_HASH_RANDOMDS, 32);
    capi_checksum256 toSign2_hash;
    sha256((char *)toSign2, sizeof(toSign2), &toSign2_hash);
    if (!__oraclize_randomDS_verifySig(toSign2_hash, sig2, sizeof(sig2), appkey_pubkey))
        return 6;


    /********************************************************************************************
     *                                                                                          *
     *   Step 7: verify the APPKEY1 provenance (must be signed by Ledger)                       *
     *                                                                                          *
     ********************************************************************************************/
    uint8_t toSign3[1 + 65];
    toSign3[0] = 0xfe;
    std::memcpy(toSign3 + 1, &proof.data()[3], 65);
    uint8_t sig3[proof[3 + 65 + 1] + 2];
    std::memcpy(sig3, &proof.data()[3 + 65], sizeof(sig3));
    capi_checksum256 toSign3_hash;
    sha256((char *)toSign3, sizeof(toSign3), &toSign3_hash);
    if (!__oraclize_randomDS_verifySig(toSign3_hash, sig3, sizeof(sig3), LEDGERKEY))
        return 7;


    // Erase the last commitment
    const auto itr2 = last_commitments.find(myQueryId_short);
    last_commitments.erase(itr2);
    return 0;
}
