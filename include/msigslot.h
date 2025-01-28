/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MSIGSLOT_H__
#define _MSIGSLOT_H__
#include "mvariant.h"

MCORELIB_NAMESPACE_BEGING


// 定义信号名称类型
typedef const char *MSIG;

// 定义槽名称类型
typedef const char *MSLOT;

// 信号与槽ID
#define SIG_SLOT_ID(x) (x)
#define SLOT_ID(CLASS_FUNC, M) SIG_SLOT_ID(#CLASS_FUNC #M)
#define SIGNAL_ID(CLASS_FUNC, M) SIG_SLOT_ID(#CLASS_FUNC #M)

// 定义信号与槽
#ifdef _MSC_VER
#pragma section(".CRT$XCU", read)
#define DECLARE_INITER(CLASS, FUNC, M)                                         \
public:                                                                        \
    static void __init_##CLASS##FUNC##M()                                      \
    {                                                                          \
        registerSlot(#CLASS "::" #FUNC #M, (SlotFun)&CLASS::__on_##FUNC##M);   \
    }

#elif defined(__GNUC__)
#define DECLARE_INITER(CLASS, FUNC, M)                                         \
    static void __init_##CLASS##FUNC##M() __attribute__((constructor))         \
    {                                                                          \
        registerSlot(#CLASS "::" #FUNC #M, (SlotFun)&CLASS::__on_##FUNC##M);   \
    }
#endif

//[!] 宏定义槽实现
#ifdef _MSC_VER
// vc无法在类中直接"(constructor)",只好定义宏DEFINE_SLOT,调用registerSlot
#define INIT_SLOT(CLASS, FUNC, M)                                              \
    __declspec(allocate(".CRT$XCU")) void(__cdecl                              \
                                          * TOKEN_PASTE(f_, __LINE__))(void)   \
        = CLASS::__init_##CLASS##FUNC##M;
#define MAKE_SLOT_0(CLASS, FUNC, ...)                                          \
    INIT_SLOT(CLASS, FUNC, 0)                                                  \
    void CLASS::FUNC##0()
#define MAKE_SLOT_1(CLASS, FUNC, P1)                                           \
    INIT_SLOT(CLASS, FUNC, 1)                                                  \
    void CLASS::FUNC##1(P1)
#define MAKE_SLOT_2(CLASS, FUNC, P1, P2)                                       \
    INIT_SLOT(CLASS, FUNC, 2)                                                  \
    void CLASS::FUNC##2(P1, P2)
#define MAKE_SLOT_3(CLASS, FUNC, P1, P2, P3)                                   \
    INIT_SLOT(CLASS, FUNC, 3)                                                  \
    void CLASS::FUNC##3(P1, P2, P3)
#define MAKE_SLOT_4(CLASS, FUNC, P1, P2, P3, P4)                               \
    INIT_SLOT(CLASS, FUNC, 4)                                                  \
    void CLASS::FUNC##4(P1, P2, P3, P4)

#else //!_MSC_VER

#define MAKE_SLOT_0(CLASS, FUNC, ...) void CLASS::FUNC##0()
#define MAKE_SLOT_1(CLASS, FUNC, P1) void CLASS::FUNC##1(P1)
#define MAKE_SLOT_2(CLASS, FUNC, P1, P2) void CLASS::FUNC##2(P1, P2)
#define MAKE_SLOT_3(CLASS, FUNC, P1, P2, P3) void CLASS::FUNC##3(P1, P2, P3)
#define MAKE_SLOT_4(CLASS, FUNC, P1, P2, P3, P4)                               \
    void CLASS::FUNC##4(P1, P2, P3, P4)

#endif //_MSC_VER

//[!] 宏定义信号(0参数)
#define DECLARE_SIG_0(FUNC, ...) inline void FUNC##0() {};
//[!] 宏定义槽(0参数)
#define DECLARE_SLOT_0(CLASS, FUNC, ...)                                       \
    inline void FUNC##_SLOT##0() {} /*for check*/                              \
    void FUNC##0();                                                            \
    void __on_##FUNC##0(const MVariantList &)                                  \
    {                                                                          \
        FUNC##0();                                                             \
    }                                                                          \
    DECLARE_INITER(CLASS, FUNC, 0)

//[!] 宏定义信号(1参数)
#define DECLARE_SIG_1(FUNC, PT1) inline void FUNC##1(PT1) {};
//[!] 宏定义槽(1参数)
#define DECLARE_SLOT_1(CLASS, FUNC, PT1)                                       \
    inline void FUNC##_SLOT##1(PT1) {} /*for check*/                           \
    void FUNC##1(PT1);                                                         \
    void __on_##FUNC##1(const MVariantList &t)                                 \
    {                                                                          \
        FUNC##1(t[0]);                                                         \
    }                                                                          \
    DECLARE_INITER(CLASS, FUNC, 1);

//[!] 宏定义信号(2参数)
#define DECLARE_SIG_2(FUNC, PT1, PT2) inline void FUNC##2(PT1, PT2) {};
//[!] 宏定义槽(2参数)
#define DECLARE_SLOT_2(CLASS, FUNC, PT1, PT2)                                  \
    inline void FUNC##_SLOT##2(PT1, PT2) {} /*for check*/                      \
    void FUNC##2(PT1, PT2);                                                    \
    void __on_##FUNC##2(const MVariantList &t)                                 \
    {                                                                          \
        FUNC##2(t[0], t[1]);                                                   \
    }                                                                          \
    DECLARE_INITER(CLASS, FUNC, 2);

//[!] 宏定义信号(3参数)
#define DECLARE_SIG_3(FUNC, PT1, PT2, PT3)                                     \
    inline void FUNC##3(PT1, PT2, PT3) {};
//[!] 宏定义槽(3参数)
#define DECLARE_SLOT_3(CLASS, FUNC, PT1, PT2, PT3)                             \
    inline void FUNC##_SLOT##3(PT1, PT2, PT3) {} /*for check*/                 \
    void FUNC##3(PT1, PT2, PT3);                                               \
    void __on_##FUNC##3(const MVariantList &t)                                 \
    {                                                                          \
        FUNC##3(t[0], t[1], t[2]);                                             \
    }                                                                          \
    DECLARE_INITER(CLASS, FUNC, 3);

//[!] 宏定义信号(4参数)
#define DECLARE_SIG_4(FUNC, PT1, PT2, PT3, PT4)                                \
    inline void FUNC##4(P1, PT2, PT3, PT4) {};
//[!] 宏定义槽(4参数)
#define DECLARE_SLOT_4(CLASS, FUNC, PT1, PT2, PT3, PT4)                        \
    inline void FUNC##_SLOT##4(PT1, PT2, PT3, PT4) {} /*for check*/            \
    void FUNC##4(PT1, PT2, PT3, PT4);                                          \
    void __on_##FUNC##4(const MVariantList &t)                                 \
    {                                                                          \
        FUNC##4(t[0], t[1], t[2], t[3]);                                       \
    }                                                                          \
    DECLARE_INITER(CLASS, FUNC, 4);

//[!] 宏定义槽连接(0参数)
#define CONNECT_SLOT_0(s, sCLASS_FUNC, r, rCLASS_FUNC, ...)                    \
    (s)->connect(SIGNAL_ID(sCLASS_FUNC, 0), *(r), SLOT_ID(rCLASS_FUNC, 0));    \
    (r)->rCLASS_FUNC##_SLOT##0(); /*check*/                                    \
    CHECK_MEMBER_FUNC(rCLASS_FUNC##0)
//[!] 宏定义槽连接(1参数)
#define CONNECT_SLOT_1(s, sCLASS_FUNC, r, rCLASS_FUNC, PT1)                    \
    (s)->connect(SIGNAL_ID(sCLASS_FUNC, 1), *(r), SLOT_ID(rCLASS_FUNC, 1));    \
    (r)->rCLASS_FUNC##_SLOT##1(PT1(0)); /*check*/                              \
    CHECK_MEMBER_FUNC(rCLASS_FUNC##1)
//[!] 宏定义槽连接(2参数)
#define CONNECT_SLOT_2(s, sCLASS_FUNC, r, rCLASS_FUNC, PT1, PT2)               \
    (s)->connect(SIGNAL_ID(sCLASS_FUNC, 2), *(r), SLOT_ID(rCLASS_FUNC, 2));    \
    (r)->rCLASS_FUNC##_SLOT##2(PT1(0), PT2(0)); /*check*/                      \
    CHECK_MEMBER_FUNC(rCLASS_FUNC##2)
//[!] 宏定义槽连接(3参数)
#define CONNECT_SLOT_3(s, sCLASS_FUNC, r, rCLASS_FUNC, PT1, PT2, PT3)          \
    (s)->connect(SIGNAL_ID(sCLASS_FUNC, 3), *(r), SLOT_ID(rCLASS_FUNC, 3));    \
    (r)->rCLASS_FUNC##_SLOT##3(PT1(0), PT2(0), PT3(0)); /*check*/              \
    CHECK_MEMBER_FUNC(rCLASS_FUNC##3)
//[!] 宏定义槽连接(4参数)
#define CONNECT_SLOT_4(s, sCLASS_FUNC, r, rCLASS_FUNC, PT1, PT2, PT3, PT4)     \
    (s)->connect(SIGNAL_ID(sCLASS_FUNC, 4), *(r), SLOT_ID(rCLASS_FUNC, 4));    \
    (r)->rCLASS_FUNC##_SLOT##4(PT1(0), PT2(0), PT3(0), PT4(0)); /*check*/      \
    CHECK_MEMBER_FUNC(rCLASS_FUNC##4)

//[!] 宏定义信号发射(0参数)
#define EMIT_SIG_0(s, CLASS_FUNC, ...)                                         \
    do                                                                         \
    {                                                                          \
        (s)->CLASS_FUNC##0(); /*check*/                                        \
        (s)->emitSig(SIGNAL_ID(CLASS_FUNC, 0), MVariantList());                \
    } while (0)
//[!] 宏定义信号发射(1参数)
#define EMIT_SIG_1(s, CLASS_FUNC, Arg1)                                        \
    do                                                                         \
    {                                                                          \
        (s)->CLASS_FUNC##1(Arg1); /*check*/                                    \
        (s)->emitSig(SIGNAL_ID(CLASS_FUNC, 1),                                 \
                     MVariantList { MVariant(Arg1) });                         \
    } while (0)
//[!] 宏定义信号发射(2参数)
#define EMIT_SIG_2(s, CLASS_FUNC, Arg1, Arg2)                                  \
    do                                                                         \
    {                                                                          \
        (s)->CLASS_FUNC##2(Arg1, Arg2); /*check*/                              \
        (s)->emitSig(SIGNAL_ID(CLASS_FUNC, 2),                                 \
                     MVariantList { MVariant(Arg1), MVariant(Arg2) });         \
    } while (0)
//[!] 宏定义信号发射(3参数)
#define EMIT_SIG_3(s, CLASS_FUNC, Arg1, Arg2, Arg3)                            \
    do                                                                         \
    {                                                                          \
        (s)->CLASS_FUNC##3(Arg1, Arg2, Arg3); /*check*/                        \
        (s)->emitSig(                                                          \
            SIGNAL_ID(CLASS_FUNC, 3),                                          \
            MVariantList { MVariant(Arg1), MVariant(Arg2), MVariant(Arg3) });  \
    } while (0)
//[!] 宏定义信号发射(3参数)
#define EMIT_SIG_4(s, CLASS_FUNC, Arg1, Arg2, Arg3, Arg4)                      \
    do                                                                         \
    {                                                                          \
        (s)->CLASS_FUNC##4(Arg1, Arg2, Arg3, Arg4); /*check*/                  \
        (s)->emitSig(SIGNAL_ID(CLASS_FUNC, 4),                                 \
                     MVariantList { MVariant(Arg1), MVariant(Arg2),            \
                                    MVariant(Arg3), MVariant(Arg4) });         \
    } while (0)

#if (ARGS_NUM() == 0)
//[*] 简化宏定义信号与槽
//[$]
//[-] 辅助宏
#define __DECLARE_SIG_HELP(x, FUNC, ...)                                       \
    __EXPAND(DECLARE_SIG_##x(FUNC, __VA_ARGS__))
#define __DECLARE_SIG(x, FUNC, ...) __DECLARE_SIG_HELP(x, FUNC, __VA_ARGS__)
//[!] 定义信号
#define DECLARE_SIG(FUNC, ...)                                                 \
    __DECLARE_SIG(ARGS_NUM(__VA_ARGS__), FUNC, __VA_ARGS__)

//[-] 辅助宏
#define __DECLARE_SLOT_HELP(x, CLASS, FUNC, ...)                               \
    __EXPAND(DECLARE_SLOT_##x(CLASS, FUNC, __VA_ARGS__))
#define __DECLARE_SLOT(x, CLASS, FUNC, ...)                                    \
    __DECLARE_SLOT_HELP(x, CLASS, FUNC, __VA_ARGS__)
//[!] 宏定义槽
#define DECLARE_SLOT(CLASS, FUNC, ...)                                         \
    __DECLARE_SLOT(ARGS_NUM(__VA_ARGS__), CLASS, FUNC, __VA_ARGS__)

//[-] 辅助宏
#define __MAKE_SLOT_HELP(x, CLASS, FUNC, ...)                                  \
    __EXPAND(MAKE_SLOT_##x(CLASS, FUNC, __VA_ARGS__))
#define __MAKE_SLOT(x, CLASS, FUNC, ...)                                       \
    __MAKE_SLOT_HELP(x, CLASS, FUNC, __VA_ARGS__)
//[!] 宏定义槽实现
#define DEFINE_SLOT(CLASS, FUNC, ...)                                            \
    __MAKE_SLOT(ARGS_NUM(__VA_ARGS__), CLASS, FUNC, __VA_ARGS__)

//[-] 辅助宏
#define __CONNECT_SLOT_HELP(x, s, sCLASS_FUNC, r, rCLASS_FUNC, ...)            \
    __EXPAND(CONNECT_SLOT_##x(s, sCLASS_FUNC, r, rCLASS_FUNC, __VA_ARGS__))
#define __CONNECT_SLOT(x, s, sCLASS_FUNC, r, rCLASS_FUNC, ...)                 \
    __CONNECT_SLOT_HELP(x, s, sCLASS_FUNC, r, rCLASS_FUNC, __VA_ARGS__)
//[!] 宏定义槽连接
#define CONNECT_SLOT(s, sCLASS_FUNC, r, rCLASS_FUNC, ...)                      \
    __CONNECT_SLOT(ARGS_NUM(__VA_ARGS__), s, sCLASS_FUNC, r, rCLASS_FUNC,      \
                   __VA_ARGS__)

//[-] 辅助宏
#define __EMIT_SIG_HELP(x, s, CLASS_FUNC, ...)                                 \
    __EXPAND(EMIT_SIG_##x(s, CLASS_FUNC, __VA_ARGS__))
#define __EMIT_SIG(x, s, CLASS_FUNC, ...)                                      \
    __EMIT_SIG_HELP(x, s, CLASS_FUNC, __VA_ARGS__)
//[!] 宏定义信号发射
#define EMIT_SIG(s, CLASS_FUNC, ...)                                           \
    __EMIT_SIG(ARGS_NUM(__VA_ARGS__), s, CLASS_FUNC, __VA_ARGS__)

#else
// 无法简化,则只能使用原始的信号与槽的宏定义
// DECLARE_SIG_1, DECLARE_SLOT_1, EMIT_SIG_1
#endif


MCORELIB_NAMESPACE_END
#endif
