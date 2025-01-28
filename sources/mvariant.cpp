/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mvariant.h"

MCORELIB_NAMESPACE_BEGING

MVariant::operator mchar_t() const
{
    return m_data.c;
}

mchar_t MVariant::toChar(mbool_t *ok) const
{
    bool ret = (type() == MVariant::Char);
    if (ok)
    {
        *ok = ret;
    }
    if (ret)
    {
        return m_data.c;
    }
    return '\0';
}

MVariant::operator muchar_t() const
{
    return m_data.uc;
}

muchar_t MVariant::toUChar(mbool_t *ok) const
{
    return '\0';
}

MVariant::operator mbool_t() const
{
    return m_data.b;
}

mbool_t MVariant::toBool(mbool_t *ok) const
{
    return false;
}

MVariant::operator mint_t() const
{
    return m_data.i;
}

mint_t MVariant::toInt(mbool_t *ok) const
{
    return m_data.i;
}

MVariant::operator muint_t() const
{
    return m_data.u;
}

muint_t MVariant::toUInt(mbool_t *ok) const
{
    return m_data.u;
}

MVariant::operator mllong_t() const
{
    return m_data.ll;
}

mllong_t MVariant::toLLong(bool *ok) const
{
    return m_data.ll;
}

MVariant::operator mullong_t() const
{
    return m_data.ull;
}

mullong_t MVariant::toULLong(mbool_t *ok) const
{
    return m_data.ull;
}

MVariant::operator mdouble_t() const
{
    return m_data.d;
}

mdouble_t MVariant::toDouble(mbool_t *ok) const
{
    return m_data.d;
}

MVariant::operator mstring_t() const
{
    bool ret = (type() == MVariant::String);
    if (ret)
    {
        return m_str;
    }
    return mstring_t();
}

mstring_t MVariant::toString(mbool_t *ok) const
{
    bool ret = (type() == MVariant::String);
    if (ok)
    {
        *ok = ret;
    }
    if (ret)
    {
        return m_str;
    }
    return mstring_t();
}

MCORELIB_NAMESPACE_END
