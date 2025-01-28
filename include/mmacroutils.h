/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef M_MACRO_UTILS_H
#define M_MACRO_UTILS_H
#include <atomic>
#include "mtypes.h"
#include "mnamespace.h"


//[*] 编译期常数
constexpr unsigned long long _Hash(char const *str, unsigned long long seed)
{
    return 0 == *str
               ? seed
               : _Hash(str + 1, seed
                                    ^ (*str + (unsigned long long)0x9e3779b9
                                       + (seed << 6) + (seed >> 2)));
}

template <unsigned long long N> struct _EnsureConst {
    static const unsigned long long value = N;
};
//[1] 字符串转数字
#define CONST_NUMBER(str) (_EnsureConst<_Hash(str, 0)>::value)

constexpr const char *str_end(const char *str)
{
    return *str ? str_end(str + 1) : str;
}

constexpr bool str_slant(const char *str)
{
    return *str == '/' || *str == '\\' ? true
                                       : (*str ? str_slant(str + 1) : false);
}

constexpr const char *r_slant(const char *str)
{
    return *str == '/' || *str == '\\' ? (str + 1) : r_slant(str - 1);
}
//[2] 获取文件名
constexpr const char *_BASENAME(const char *filename)
{
    return str_slant(filename) ? r_slant(str_end(filename)) : filename;
}

//[*] 一些辅助工具
// 数组个数
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

// 展开
#define __TOKEN_PASTE_HELP(x, y) x##y
#define TOKEN_PASTE(x, y) __TOKEN_PASTE_HELP(x, y)
#define __EXPAND(...) __VA_ARGS__

//[*] 宏参数个数统计
#ifdef _MSC_VER

#define __NARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, VAL, ...) VAL
#define __NARGS_1(...) __EXPAND(__NARGS(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define __AUGMENTER(...) unused, __VA_ARGS__
#define ARGS_NUM(...) __NARGS_1(__AUGMENTER(__VA_ARGS__))

#else // Others

#define ARGS_NUM(...)                                                          \
    __INNER_ARG_COUNT(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __INNER_ARG_COUNT(_0, _1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

#endif

//[*] 检查类中是否有某个函数
#define CHECK_MEMBER_FUNC(CLASS_FUNC)                                          \
    static_assert(                                                             \
        std::is_member_function_pointer<decltype(&CLASS_FUNC)>::value,         \
        #CLASS_FUNC "is not a member function.");

//[*] 单例
// 声明单例，放在类内部
#define DECLARE_SINGLETON(_class)                                              \
private:                                                                       \
    std::atomic<int> instRefer;                                                \
    _class();                                                                  \
    inline int derefInstance()                                                 \
    {                                                                          \
        return --instRefer;                                                    \
    }                                                                          \
    inline int refInstance()                                                   \
    {                                                                          \
        return ++instRefer;                                                    \
    }                                                                          \
                                                                               \
public:                                                                        \
    static _class *instance();                                                 \
    static void destroy();
// 单例定义，放在类实现外部
#define DEFINE_SINGLETON(_class)                                               \
    static _class *_inst_##_class = NULL;                                      \
    _class *_class::instance()                                                 \
    {                                                                          \
        if (_inst_##_class == NULL)                                            \
        {                                                                      \
            _inst_##_class = new _class();                                     \
            _inst_##_class->instRefer = 1;                                     \
            mDebug("Instance_" #_class ": %d",                                 \
                    (int)_inst_##_class->instRefer);                           \
        }                                                                      \
        return _inst_##_class;                                                 \
    }                                                                          \
    void _class::destroy()                                                     \
    {                                                                          \
        if (_inst_##_class)                                                    \
        {                                                                      \
            mDebug("destroy " #_class ": %d",                                  \
                    (int)_inst_##_class->instRefer);                           \
            if (--_inst_##_class->instRefer <= 0)                              \
            {                                                                  \
                delete _inst_##_class;                                         \
                _inst_##_class = NULL;                                         \
            }                                                                  \
        }                                                                      \
    }

//[*] 禁止类拷贝
#define DISABLE_CLASS_COPY(_class)                                             \
    _class(const _class &) = delete;                                           \
    _class &operator=(const _class &) = delete;

// 声明私有数据
#define DECLARE_PRIVATE_DATA(_class)                                           \
private:                                                                       \
    class _class##_Private;                                                    \
    friend class _class##_Private;                                             \
    _class##_Private *privData;

// 声明平台数据
#define DECLARE_PLATFORM_DATA(_class) \
private:                                                                       \
    class _class##_PlatformData;                                               \
    friend class _class##_PlatformData;                                        \
    _class##_PlatformData* platData;




#endif
