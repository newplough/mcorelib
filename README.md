# MCoreLib简介

实现类似Qt核心库的一些功能，包括信号与槽。可以方便地移植到单片机系统中。经过测试，在linux、windows和stm32f407单片机系统中运行正常。

## MCoreLib一些特点

* 支持信号与槽，不需要类似moc工具辅助
* 支持同线程与跨线程投递信号
* 可变数据类型MVariant支持放入自定义数据类型
* 使用时间堆实现的定时器管理
* 支持在windows、linux和单片机等系统中使用

## MCoreLib使用示例

### 主函数

`和Qt一样，在主函数中需要使用application对象`

* 主函数编写示例
  
      extern "C" void test_main(void)
      {
      #if (MOS_TYPE == MOS_RTOS)
          RigsterLogWriter(BoardLog_Write);
      #endif
      
          MApplication app;
      
          testSlot();
      
          app.exec();
      }

### 信号与槽函数

`由于使用宏的方式实现，没能做到与Qt完成一样`

* 用DECLARE_SIG宏声明信号
  
      //! 定义信号发送类
      class TestSignalClass: public MObject
      {
      public:
          // 声明无参数信号
          DECLARE_SIG(s0)
      
          // 声明1参数信号
          DECLARE_SIG(s1, int)
      
          // 声明MVariant参数信号
          DECLARE_SIG(s1v, MVariant)
      };

* 用DECLARE_SLOT宏声明槽函数
  
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

* 用DEFINE_SLOT宏定义槽函数    
  
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
      
      DEFINE_SLOT(TestReceiveClass, testSlot1, mint_t d)
      {
          LogInfo("%s: %d", __FUNCTION__, d);
          return;
      }

### 信号连接与信号发出

* 使用CONNECT_SLOT连接信号与槽，使用EMIT_SIG发出信号，示例如下：
  
      void testSlot()
      {
          TestSignalClass sender;
          TestReceiveClass reciever;
      
          // 此处是同线程关联信号
          CONNECT_SLOT(&sender, TestSignalClass::s1, &reciever, TestReceiveClass::testSlot1, int);
      
          // 启动测试线程，进行跨线程关联信号
          TestTask task(&sender);
          task.start(true);
      
          // 投递信号
          EMIT_SIG(&sender, TestSignalClass::s0);
          // 投递信号
          EMIT_SIG(&sender, TestSignalClass::s1, 32);
          // 投递MVariant信号
          EMIT_SIG(&sender, TestSignalClass::s1v, MVariant(56));
      }

***

## 完整示例

### 请参考代码

[test_timer_slot.cpp](testor/test_timer_slot.cpp)
[test_variant_slot.cpp](testor/test_variant_slot.cpp)
[单片机stm32f407](project/stm32f407)

***

## 其他一些说明

` vs2022 编译通过（windows）` 
` g++ 9.4.0 编译通过（linux）` 
` STM32CubeIDE 1.17.0 编译通过（stm32f407）` 

` 本代码未经过严格测试，难免存在BUG! ` 
