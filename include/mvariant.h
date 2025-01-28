/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MVARIANT_H__
#define _MVARIANT_H__
#include "mmacroutils.h"
#include "mplatform.h"
#include <memory>
#include <vector>
#include <string.h>

MCORELIB_NAMESPACE_BEGING

// 通用数据类型

template <class T> struct VariantDeleter 
{
    inline void operator()(void *pData)
    {
        delete static_cast<T *>(pData);
        //printf("delete: %p\n", pData);
    }
};

class MVariant
{
public:
#define PTR_MARK 0x8000
    enum Type {
        Invalid = 0x0,       // 无效
        Char = 0x01,         // 字符
        UChar = 0x02,        // 字符
        Int = 0x03,          // 整数
        UInt = 0x04,         // 整数
        LongLong = 0x05,     // 长整数
        ULongLong = 0x06,    // 长整数
        Bool = 0x07,         // bool
        Float = 0x08,        // 浮点数
        Double = 0x09,       // 浮点数
        String = 0x0a,       // 字符串
        UData = PTR_MARK | 0x01, // 用户数据
    };

private:
    union Data {
        inline Data()
            : ull(0)
        {
        }
        inline Data(mchar_t v)
            : c(v)
        {
        }
        inline Data(muchar_t v)
            : uc(v)
        {
        }
        inline Data(mint_t v)
            : i(v)
        {
        }
        inline Data(muint_t v)
            : u(v)
        {
        }
        inline Data(mllong_t v)
            : ll(v)
        {
        }
        inline Data(mullong_t v)
            : ull(v)
        {
        }
        inline Data(mbool_t v)
            : b(v)
        {
        }
        inline Data(mfloat_t v)
            : f(v)
        {
        }
        inline Data(mdouble_t v)
            : d(v)
        {
        }
        inline ~Data()
        {
        }
        
        mchar_t c;
        muchar_t uc;
        mint_t i;
        muint_t u;
        mbool_t b;
        mfloat_t f;
        mdouble_t d;
        mllong_t ll;
        mullong_t ull;
    } m_data;
    
    std::shared_ptr<void> m_ptr;
    std::string m_str;

    Type m_type = Invalid;

public:
    inline MVariant()
        : m_type(Invalid)
    {
    }
    inline ~MVariant()
    {
        m_ptr.reset();
    }

    inline MVariant(mchar_t c)
        : m_data(c)
        , m_type(Char)
    {
    }
    inline MVariant(muchar_t uc)
        : m_data(uc)
        , m_type(UChar)
    {
    }
    inline MVariant(mint_t i)
        : m_data(i)
        , m_type(Int)
    {
    }
    inline MVariant(muint_t ui)
        : m_data(ui)
        , m_type(UInt)
    {
    }
    inline MVariant(mllong_t ll)
        : m_data(ll)
        , m_type(LongLong)
    {
    }
    inline MVariant(mullong_t ull)
        : m_data(ull)
        , m_type(ULongLong)
    {
    }
    inline MVariant(mbool_t b)
        : m_data(b)
        , m_type(Bool)
    {
    }
    inline MVariant(mfloat_t d)
        : m_data(d)
        , m_type(Float)
    {
    }
    inline MVariant(mdouble_t d)
        : m_data(d)
        , m_type(Double)
    {
    }

    inline MVariant(const mstring_t &str)
        : m_str(str)
        , m_type(String)
    {
    }

    inline MVariant(const MVariant &other)
    {       
        m_data = other.m_data;
        m_str = other.m_str;
        m_ptr = other.m_ptr;
        m_type = other.m_type;
    }

    MVariant &operator=(const MVariant &other)
    {
        if (this != &other)
        {
            m_data = other.m_data;
            m_str = other.m_str;
            m_ptr = other.m_ptr;
            m_type = other.m_type;
        }
        return *this;
    }

    inline MVariant &operator=(MVariant &&other)
    {
        if (this != &other)
        {
            std::swap(m_data, other.m_data);
            std::swap(m_str, other.m_str);
            std::swap(m_ptr, other.m_ptr);
            std::swap(m_type, other.m_type);           
        }
        return *this;
    }

    template <typename T> 
    inline void setValue(const T& value)
    {
        m_type = UData;
        m_ptr.reset();  
        m_ptr = std::shared_ptr<void>(reinterpret_cast<void *>(new T(value)),
                                    VariantDeleter<T>());
        // 使用typeid，在单片机程序中编译时通常需要手动打开-frtti选项
        m_str = typeid(T).name(); // 用户自定义数据时，m_str存储T的名称
    }

    template <typename T> 
    inline bool value(T& t) const
    {  
        if (m_type & UData)
        {
            if (m_str == typeid(T).name())
            {
                T *p = reinterpret_cast<T *>(m_ptr.get());
                if (p != NULL)
                {
                    t = *p;
                    return true;
                }
            }
        }
        return false;
    }

    inline Type type() const
    {
        return m_type;
    }

    operator mchar_t() const;
    mchar_t toChar(mbool_t *ok = nullptr) const;

    operator muchar_t() const;
    muchar_t toUChar(mbool_t *ok = nullptr) const;

    operator mint_t() const;
    mint_t toInt(mbool_t *ok = nullptr) const;

    operator muint_t() const;
    muint_t toUInt(mbool_t *ok = nullptr) const;

    operator mllong_t() const;
    mllong_t toLLong(mbool_t *ok = nullptr) const;

    operator mullong_t() const;
    mullong_t toULLong(mbool_t *ok = nullptr) const;

    operator mbool_t() const;
    mbool_t toBool(mbool_t *ok = nullptr) const;

    operator mdouble_t() const;
    mdouble_t toDouble(mbool_t *ok = nullptr) const;

    operator mstring_t() const;
    mstring_t toString(mbool_t *ok = nullptr) const;

};

typedef std::vector<MVariant> MVariantList;

MCORELIB_NAMESPACE_END
#endif
