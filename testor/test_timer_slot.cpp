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

// 测试类
class TestClass : public MObject
{
public:
    TestClass(const mstring_t &name = "")
        : MObject(name)
    {
    }
    // 声明slot函数
    DECLARE_SLOT(TestClass, testSlotFunc);
};

// 定义slot函数
DEFINE_SLOT(TestClass, testSlotFunc)
{
    static int counter = 0;
    ++counter;
    LogInfo("TestClass::testSlotFunc: %d", counter);
    if (counter >= 20)
    {
        mApp->quit();  // 执行10次后退出app程序
    }
}

// 定时测试任务，启动100ms的定时器，通过信号触发执行slot函数testSlotFunc
class TestTask : public MThread
{
public:
    TestTask()
        : MThread("TestTask")
        , m_testor(nullptr)
    {
    }

    ~TestTask()
    {
        LogInfo("~TestTask");
        if (m_testor)
            delete m_testor;
        m_testor = nullptr;
    }

protected:
    virtual int init()
    {
        MThread::init();
        m_testor = new TestClass("testClass");
        CONNECT_SLOT(&m_timer, MTimer::timeout, m_testor,
                     TestClass::testSlotFunc);
        m_timer.start(200);
        return 0;
    }

private:
    MTimer m_timer;
    TestClass *m_testor;
};


extern "C" void test_main(void)
{
#if (MOS_TYPE == MOS_RTOS)
    RigsterLogWriter(BoardLog_Write);
#endif
    MApplication app;

    TestTask task;
    task.start();

    app.exec();
}

#if (MOS_TYPE == MOS_WIN) || (MOS_TYPE == MOS_LINUX)
int main()
{
    LogInfo("test_main Begin:");
    test_main();
    LogInfo("test_main End!");
    return 0;
}
#endif
