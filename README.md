# MCoreLib���

ʵ������Qt���Ŀ��һЩ���ܣ������ź���ۡ����Է������ֲ����Ƭ��ϵͳ�С��������Կ�����linux��windows��stm32f407��Ƭ��ϵͳ��ʹ�á�
It implements some functions similar to the Qt core library, including signals and slots. It can be easily ported to microcontroller systems. After testing, it can be used on Linux, Windows, and STM32F407 microcontroller systems.

## MCoreLibһЩ�ص�

* ֧���ź���ۣ�����Ҫ����moc���߸���
* ֧��ͬ�߳�����߳�Ͷ���ź�
* �ɱ���������MVariant֧�ַ����Զ�����������
* ʹ��ʱ���ʵ�ֵĶ�ʱ������
* ֧����windows��linux�͵�Ƭ����ϵͳ��ʹ��

## MCoreLibʹ��ʾ��

### ������

`��Qtһ����������������Ҫʹ��application����`

* ��������дʾ��
  
      extern "C" void test_main(void)
      {
      #if (MOS_TYPE == MOS_RTOS)
          RigsterLogWriter(BoardLog_Write);
      #endif
      
          MApplication app;
      
          testSlot();
      
          app.exec();
      }

### �ź���ۺ���

`����ʹ�ú�ķ�ʽʵ�֣�û��������Qt���һ��`

* ��DECLARE_SIG�������ź�
  
      //! �����źŷ�����
      class TestSignalClass: public MObject
      {
      public:
          // �����޲����ź�
          DECLARE_SIG(s0)
          // ����1�����ź�
          DECLARE_SIG(s1, int)
          // ����MVariant�����ź�
          DECLARE_SIG(s1v, MVariant)
      };

* ��DECLARE_SLOT�������ۺ���
  
      class TestReceiveClass: public MObject
      {
      public:  
          TestReceiveClass(const mstring_t &name = "")
                  : MObject(name)
          {
          }
          // �����ۺ������޲���
          DECLARE_SLOT(TestReceiveClass, testSlot0);
          // �����ۺ�����1������
          DECLARE_SLOT(TestReceiveClass, testSlot1, mint_t);
      
      };

* ��DEFINE_SLOT�궨��ۺ���    
  
      DEFINE_SLOT(TestReceiveClass, testSlot0)
      {
          static int counter = 0;
          ++counter;
          LogInfo("%s: %d", __FUNCTION__, counter);
          if (counter >= 10)
          {
              mApp->quit(); // ִ��10�κ��˳�app����
          }
      }
      
      DEFINE_SLOT(TestReceiveClass, testSlot1, mint_t d)
      {
          LogInfo("%s: %d", __FUNCTION__, d);
          return;
      }

### �ź��������źŷ���

* ʹ��CONNECT_SLOT�����ź���ۣ�ʹ��EMIT_SIG�����źţ�ʾ�����£�
  
      void testSlot()
      {
          TestSignalClass sender;
          TestReceiveClass reciever;
      
          // �˴���ͬ�̹߳����ź�
          CONNECT_SLOT(&sender, TestSignalClass::s1, &reciever, TestReceiveClass::testSlot1, int);
      
          // ���������̣߳����п��̹߳����ź�
          TestTask task(&sender);
          task.start(true);
      
          // Ͷ���ź�
          EMIT_SIG(&sender, TestSignalClass::s0);
          // Ͷ���ź�
          EMIT_SIG(&sender, TestSignalClass::s1, 32);
          // Ͷ��MVariant�ź�
          EMIT_SIG(&sender, TestSignalClass::s1v, MVariant(56));
      }

***

## ����ʾ��

### ��ο�����

[test_timer_slot.cpp](testor/test_timer_slot.cpp)
[test_variant_slot.cpp](tester/test_variant_slot.cpp)
[��Ƭ��stm32f407](project/stm32f407)

***

## ����һЩ˵��

` vs2022 ����ͨ����windows��` 
` g++ 9.4.0 ����ͨ����linux��` 
` STM32CubeIDE 1.17.0 ����ͨ����stm32f407��` 

` ������δ�����ϸ���ԣ��������BUG! ` 