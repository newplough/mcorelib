#include "mapplication.h"
#include "mlogger.h"
#include "msigslot.h"
#include "mthread.h"
#include "mtimer.h"
#include "mvariant.h"
#include "mwaitqueue.h"
#include "mmutex.h"

#if (MOS_TYPE == MOS_RTOS)
#include "board_log.h"
#endif

USE_MCORELIB_NAMESPACE

#define TEST_TRACE(msg) LogInfo(">>> %s: %s", __FUNCTION__, msg);

// Hello类中使用整数变量
class Hello
{
public:
    Hello(int v = 0)
            : val(v)
    {
    }

    // 放入Hello对象到MVariant
    static MVariant getHello(int val)
    {
        MVariant v;
        v.setValue<Hello>(Hello(val));
        return v;
    }

    int val;
};

// World类中使用字符串变量
class World
{
public:
    World() : val("")
    {}
    World(const std::string &s)
            : val(s)
    {
    }
    ~World()
    {
    }

    // 放入World对象到MVariant
    static MVariant getWorld(const std::string &str)
    {
        MVariant v;
        v.setValue<World>(World(str));
        return v;
    }

    std::string val;
};

// 测试variant中放入自定义对象并取出
void testVariant()
{
    TEST_TRACE("object");

    // MVariant放入Hello对象
    MVariant variant_hello = Hello::getHello(123);

    // MVariant放入World对象
    MVariant variant_world = World::getWorld("abc");

    Hello hello;
    World world;

    // 正常提取输出
    if (variant_hello.value<Hello>(hello))
    {
        LogInfo("Hello: %d\n", hello.val);
    }

    if (variant_world.value<World>(world))
    {
        LogInfo("world: %s", world.val.c_str());
    }

    // 异常提取, 类型匹配错误
    if (!variant_hello.value<World>(world))
    {
        LogInfo("variant_hello -> World failed\n");
    }

    if (!variant_world.value<Hello>(hello))
    {
        LogInfo("variant_world -> hello failed\n");
    }
}

//! 测试信号接收类
class TestReceiveClass: public MObject
{
public:
    TestReceiveClass(const mstring_t &name = "")
            : MObject(name)
    {
    }

    // 声明槽函数，无参数
    DECLARE_SLOT(TestReceiveClass, testSlot0);

    // 声明槽函数，1个参数
    DECLARE_SLOT(TestReceiveClass, testSlot1, mint_t);
};

// 定义槽函数，无参数
DEFINE_SLOT(TestReceiveClass, testSlot0)
{
    static int counter = 0;
    ++counter;
    LogInfo("%s: %d", __FUNCTION__, counter);
    if (counter >= 10)
    {
        mApp->quit(); // 执行10次后退出app程序
    }
}

// 定义槽函数，1个参数
DEFINE_SLOT(TestReceiveClass, testSlot1, mint_t d)
{
    LogInfo("%s: %d", __FUNCTION__, d);
    return;
}

//! 测试信号接收类2，派生于TestReceiveClass
//! 测试是否存在信号响应是否存在错乱
class TestReceiveClass2: public TestReceiveClass
{
public:
    TestReceiveClass2(const mstring_t &name = "")
            : TestReceiveClass(name)
    {
    }

    // 声明无参数槽函数，名称与基类一样
    DECLARE_SLOT(TestReceiveClass2, testSlot0);

    // 声明1个参数槽函数，名称与基类一样
    DECLARE_SLOT(TestReceiveClass2, testSlot1, mint_t);

    // 声明MVariant类型参数槽函数
    DECLARE_SLOT(TestReceiveClass2, testSlot1v, const MVariant &);
};

// 定义无参数槽函数，名称与基类一样
DEFINE_SLOT(TestReceiveClass2, testSlot0)
{
    static int counter = 0;
    LogInfo("%s: %d", __FUNCTION__, ++counter);
}

// 定义1个参数槽函数，名称与基类一样
DEFINE_SLOT(TestReceiveClass2, testSlot1, mint_t d)
{
    LogInfo("%s: %d", __FUNCTION__, d);
    return;
}

// 定义MVariant类型参数槽函数
DEFINE_SLOT(TestReceiveClass2, testSlot1v, const MVariant &d)
{
    LogInfo("%s: type=%d", __FUNCTION__, d.type());

    switch (d.type())
    {
    case MVariant::Int:
        LogInfo("value: = %d", d.toInt(0));
        break;
    default:
        break;
    }
}

//! 定义信号发送类
class TestSignalClass: public MObject
{
public:
    // 声明无参数信号
    DECLARE_SIG(s0);

    // 声明1参数信号
    DECLARE_SIG(s1, int);

    // 声明MVariant参数信号
    DECLARE_SIG(s1v, MVariant);
};

class TestTask: public MThread
{
public:
    TestTask(TestSignalClass *_s)
            : MThread("TestTask"), s(_s)
    {
    }
    ~TestTask()
    {
        delete r1; // 此处销毁与创建不在同一线程
        delete r2; // 销毁时会有一个警告输出
        r1 = NULL;
        r2 = NULL;
    }

protected:
    virtual int init() override
    {
        MThread::init();

        r1 = new TestReceiveClass("TestClass");
        r2 = new TestReceiveClass2("TestClass2");

        // 关联信号于槽函数，若参数个数不匹配会编译报错，
        // 类型不是特别重要, 都会转换为MVariant类型，若不能转换编译会报错
        CONNECT_SLOT(s, TestSignalClass::s0, r1, TestReceiveClass::testSlot0);

        // 关联信号于槽函数，若参数个数不匹配会编译报错
        CONNECT_SLOT(s, TestSignalClass::s1, r2, TestReceiveClass2::testSlot1,
                int);

        // 关联信号于槽函数，若参数个数不匹配会编译报错
        CONNECT_SLOT(s, TestSignalClass::s1v, r2, TestReceiveClass2::testSlot1v,
                MVariant);

        LogInfo("TestTask init");
        return 0;
    }

private:
    TestReceiveClass *r1;
    TestReceiveClass2 *r2;
    TestSignalClass *s;
};

void testSlot()
{
    TestSignalClass sender;
    TestReceiveClass reciever;

    // 此处是同线程关联信号
    CONNECT_SLOT(&sender, TestSignalClass::s1, &reciever, TestReceiveClass::testSlot1, int);

    // 启动测试线程，进行跨线程关联信号
    TestTask task(&sender);
    task.start(true);

    TEST_TRACE("EMIT_SIG");
    // 投递信号，
    EMIT_SIG(&sender, TestSignalClass::s0);

    // 投递信号，分别在TestReceiveClass::testSlot1 和 TestReceiveClass2::testSlot1中执行
    // TestReceiveClass::testSlot1是同线程，
    // TestReceiveClass2::testSlot1是跨线程执行
    EMIT_SIG(&sender, TestSignalClass::s1, 32);

    // 投递MVariant信号
    EMIT_SIG(&sender, TestSignalClass::s1v, MVariant(56));

    SleepMsec(1000);
}

extern "C" void test_main(void)
{
#if (MOS_TYPE == MOS_RTOS)
    RigsterLogWriter(BoardLog_Write);
#endif

    MApplication app;

    testVariant();

    testSlot();

    app.exec();
}

#if (MOS_TYPE == MOS_WIN) || (MOS_TYPE == MOS_LINUX)
int main()
{
    test_main();
    return 0;
}
#endif
