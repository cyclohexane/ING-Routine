/*pv操作*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef HANDLE Semaphore;                      //   信号量的Windows原型
#define P(S) WaitForSingleObject(S, INFINITE)  //   定义Windows下的P操作
#define V(S) ReleaseSemaphore(S, 1, NULL)      //   定义Windows下的V操作

#define rate 1000

#define CONSUMER_NUM 10 /*   消费者个数   */
#define PRODUCER_NUM 10 /*   生产者个数   */
#define BUFFER_NUM 4    /*   缓冲区个数   */

char *thing[10] = {"牙膏", "毛巾", "方便面", "茶杯",     "大米",
                   "衣服", "鞋子", "电脑",   "数码相机", "柜子"};

struct Buffer {
  int product[BUFFER_NUM];  //   缓冲区
  int start, end;           //   两个指针
} g_buf;

Semaphore g_semBuffer, g_semProduct, g_mutex;

DWORD WINAPI Consumer(LPVOID para) {         //消费者线程
  int i = *(int *)para;                      // i表示第i个消费者
  int ptr;                                   //待消费的内容的指针
  printf("    消费者-%03d:   我要：\n", i);  //被创建的消费者打印出这句
  Sleep(300);                                //然后休眠
  while (true) {
    printf("    消费者-%03d:   卖给我!\n", i);  //消费者醒来辽
    P(g_semProduct);                            //等待产品
    P(g_mutex);         //有产品，先锁住缓冲区   g_buf
    ptr = g_buf.start;  //记录消费的物品 //start指向
    g_buf.start =
        (g_buf.start + 1) %
        BUFFER_NUM;  //再移动缓冲区指针（+1，如果超过缓冲区范围就用%归零）
    V(g_mutex);  //让其他消费者或生产者使用   g_buf
    printf("     消费者-%03d:   我要   buf[%d]   =   %s\n", i, ptr,
           thing[g_buf.product[ptr]]);
    Sleep(rate * rand() % 10 + 110);  //搬运中
    printf("     消费者-%03d:   我需要   buf[%d]   =   %s\n", i, ptr,
           thing[g_buf.product[ptr]]);
    //消费完毕，并释放一个缓冲
    V(g_semBuffer);
  }
  return 0;
}

DWORD WINAPI Producer(LPVOID para) {  //   生产者线程
  int i = *(int *)para - CONSUMER_NUM;
  int ptr;
  int data;                                //产品
  printf("生产者-%03d:   我来啦！\n", i);  //被创建的生产者打印出这句
  Sleep(300);                              //然后休眠
  while (true) {
    printf("生产者-%03d:   我努力生产…………\n", i);  //生产者醒来辽，开始生产
    Sleep(rate * rand() % 10 + 110);               //生产中，随机休眠
    data = rand() % 10;                            //产品为10以内随机数
    printf("生产者-%03d:   生产一个东西   data   =   %s!\n", i,
           thing[data]);  //随机生产出一个产品
    P(g_semBuffer);       //等待存放空间
    P(g_mutex);           //有地方，先锁住缓冲区   g_buf
    ptr = g_buf.end;      //记录消费的物品
    g_buf.end =
        (g_buf.end + 1) %
        BUFFER_NUM;  //再移动缓冲区指针 （+1，如果超过缓冲区范围就用%归零）
    V(g_mutex);  //让其他消费者或生产者使用缓冲区   g_buf
    printf("生产者-%03d:   放到仓库   buf[%d]   =   %s\n", i, ptr, thing[data]);
    g_buf.product[ptr] = data;
    Sleep(rate / 2 * rand() % 10 + 110);  //放置中，随机休眠
    printf("生产者-%03d:   buf[%d]   =   %s   做好了，大家买吧！\n", i, ptr,
           thing[g_buf.product[ptr]]);
    V(g_semProduct);  //放好了完毕，释放一个产品
  }
  return 0;
}

int main(int argc, char *argv[]) {
  HANDLE hThread[CONSUMER_NUM + PRODUCER_NUM];  //线程计数
  int totalThreads = CONSUMER_NUM + PRODUCER_NUM;
  int i = 0;
  //初始化信号量 未知，初始计数，最大计数，信号量名称
  g_mutex = CreateSemaphore(NULL, BUFFER_NUM, BUFFER_NUM,
                            "mutexOfConsumerAndProducer");
  g_semBuffer =
      CreateSemaphore(NULL, BUFFER_NUM, BUFFER_NUM, "BufferSemaphone");
  g_semProduct = CreateSFemaphore(NULL, 0, BUFFER_NUM, "ProductSemaphone");

  printf("消费者购买！\n");  //   开启消费者线程
  for (i = 0; i < CONSUMER_NUM; i++) {
    //未知，进程初始大小，线程函数，向线程函数传递的参数，立即激活，线程id
    hThread[i] = CreateThread(NULL, 0, Consumer, &i, 0, &tid);
    if (hThread[i])
      WaitForSingleObject(hThread[i], 10);  //每个新消费者线程都等待10ms
  }

  printf("生产者生产！\n");
  for (; i < totalThreads; i++) {
    hThread[i] = CreateThread(NULL, 0, Producer, &i, 0, &tid);
    if (hThread[i])
      WaitForSingleObject(hThread[i], 10);  //每个新生产者线程都等待10ms
  }

  //生产者和消费者的执行
  WaitForMultipleObjects(totalThreads, hThread, TRUE, INFINITE);
  return 0;
}